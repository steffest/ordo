#include <proto/dos.h>
#include <proto/intuition.h>
#include <proto/graphics.h>
#include <intuition/screens.h>
#include <stdio.h>
#include <exec/types.h>
#include <exec/memory.h>
#include "screen.h"

#define SCREENWIDTH 320
#define SCREENHEIGHT 240
#define BITMAPWIDTH 320
#define BITMAPHEIGHT 240
#define PLANES 5 // 32 colors
#define LORES 0

BOOL useDoubleBuffer = TRUE;
BOOL hasOffScreenContext = FALSE;

volatile UBYTE *custom_vhposr= (volatile UBYTE *) 0xdff006;

const unsigned short colours_black[32] = {
    0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,0x000, 0x000, 0x000, 0x000, 0x000,
    0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,0x000, 0x000, 0x000, 0x000, 0x000
};


const SHORT linearray[] =
        {
         3, 3,
        15, 3,
        15,15,
         3,15,
         3, 3
        };


static struct View view;
static struct RastPort screenContext;
static struct BitMap screenBitMap;
static struct RastPort offScreenContext;
static struct BitMap offScreenBitMap;
static struct Window *window; // I guess the only we need a window is to be able to hide the mouse pointer?
static struct ViewPort *viewport;
static UWORD *cursor;
static struct RastPort ctx;


static struct NewScreen screenProperties = {
        0, 0, SCREENWIDTH, SCREENHEIGHT, PLANES, 0, 0, 0,
	    CUSTOMSCREEN | CUSTOMBITMAP,
	    NULL, NULL, NULL, NULL,
     };
     static struct NewWindow windowProperties = {
        0, 0, SCREENWIDTH, SCREENHEIGHT, 0, 0,
        IDCMP_RAWKEY | IDCMP_MOUSEBUTTONS,
        WFLG_SIMPLE_REFRESH | WFLG_BACKDROP | WFLG_BORDERLESS | WFLG_ACTIVATE | WFLG_RMBTRAP | WFLG_NOCAREREFRESH,
        0, 0, 0, 0, 0,
        0, 0, 0, 0,
        CUSTOMSCREEN,
    };


BOOL openScreen(void){
    int i;
    IntuitionBase = OpenLibrary("intuition.library", 33L);
    if ( IntuitionBase == NULL ){
        fprintf(stderr, "Failed to open intuition.library\n");
        return FALSE;
    }

    GfxBase = OpenLibrary("graphics.library", 33L);
    if ( GfxBase == NULL ){
        fprintf(stderr, "Failed to open graphics.library\n");
        return FALSE;
    }
        
    InitRastPort(&screenContext);
    screenContext.BitMap = &screenBitMap;
    InitBitMap(&screenBitMap, PLANES, BITMAPWIDTH, BITMAPHEIGHT);
    for(i = 0; i < PLANES; i++) if(screenBitMap.Planes[i] = AllocRaster(BITMAPWIDTH, BITMAPHEIGHT), screenBitMap.Planes[i] == 0) {
        fprintf(stderr, "%s (%dx%dx%d)\n","failed to allocate screen context", BITMAPWIDTH, BITMAPHEIGHT, PLANES);
        return FALSE;
    }
    screenProperties.CustomBitMap = screenContext.BitMap;

    if (useDoubleBuffer){
        InitRastPort(&offScreenContext);
        offScreenContext.BitMap = &offScreenBitMap;
        InitBitMap(&offScreenBitMap, PLANES, BITMAPWIDTH, BITMAPHEIGHT);
        for(i = 0; i < PLANES; i++) if(offScreenBitMap.Planes[i] = AllocRaster(BITMAPWIDTH, BITMAPHEIGHT), offScreenBitMap.Planes[i] == 0) {
            fprintf(stderr, "%s (%dx%dx%d)\n","failed to allocate offscreen context", BITMAPWIDTH, BITMAPHEIGHT, PLANES);
            return FALSE;
        }
        ctx = offScreenContext;
        hasOffScreenContext = TRUE;
    }else{
        ctx = screenContext;
    }

    if(windowProperties.Screen = OpenScreen((void *)(&screenProperties)), windowProperties.Screen == 0) {
        fprintf(stderr, "Failed to open screen\n");
        return FALSE;
    }
    ShowTitle(windowProperties.Screen, FALSE);

    if(window = OpenWindow(&windowProperties), window == 0) {
        fprintf(stderr, "Failed to open window\n");
        return FALSE;
    }
    viewport = &window->WScreen->ViewPort;
    LoadRGB4(viewport, (UWORD *) colours_black, 32);

    set_asset_screen();
    clearScreen();

    return TRUE;
    
}

void closeScreen(void){
    int i;
    if(window) { 
         if(cursor) {
		    ClearPointer(window);
		    FreeMem(cursor, 128);
	    }
        CloseWindow(window); window = 0; 
    }
    if(windowProperties.Screen) { CloseScreen(windowProperties.Screen); windowProperties.Screen = 0; }
    for(i = 0; i < PLANES; i++) if(screenBitMap.Planes[i]) { FreeRaster(screenBitMap.Planes[i], BITMAPWIDTH, BITMAPHEIGHT); screenBitMap.Planes[i] = 0; }
    
    if (hasOffScreenContext){
        for(i = 0; i < PLANES; i++) if(offScreenBitMap.Planes[i]) { FreeRaster(offScreenBitMap.Planes[i], BITMAPWIDTH, BITMAPHEIGHT); offScreenBitMap.Planes[i] = 0; }
    }

    if (GfxBase) CloseLibrary(GfxBase);
    if (IntuitionBase) CloseLibrary(IntuitionBase);
}

void clearScreen(void){
    //SetAPen(&screenContext, 0);
    //RectFill(&screenContext, 0, 0, BITMAPWIDTH-1, BITMAPHEIGHT-1);
    SetRast(&ctx, 0);
}

void setColor(int color, UBYTE r, UBYTE g, UBYTE b){
    SetRGB4(viewport, color, r, g, b);
}

void setPalette(const unsigned short *palette){
    LoadRGB4(viewport, (UWORD *)palette, 32);
}

void setPaletteToBlack(void){
    setPalette(colours_black);
}

void fadePalette(unsigned short sourcePalette[], unsigned short targetPalette[],WORD duration){
    int i;
    UBYTE r,g,b;
    UBYTE tr,tg,tb;
    int counter = 0;

    while (counter<=duration){
        while (*custom_vhposr != 0xff) ; // wait for vblank
        for (i=0;i<32;i++){
            r = (sourcePalette[i]>>8)&0xF;
            g = (sourcePalette[i]>>4)&0xF;
            b = (sourcePalette[i])&0xF;

            tr = (targetPalette[i]>>8)&0xF;
            tg = (targetPalette[i]>>4)&0xF;
            tb = (targetPalette[i])&0xF;

            r += (tr-r)*counter/duration;
            g += (tg-g)*counter/duration;
            b += (tb-b)*counter/duration;
        

            SetRGB4(viewport, i, r, g, b);
        }

        counter++;
        WaitTOF();

     }
}

// step is from 1 to 100
void fadePaletteStep(unsigned short sourcePalette[], unsigned short targetPalette[],WORD step){
    int i;
    UBYTE r,g,b;
    UBYTE tr,tg,tb;
    int counter = 0;

    for (i=0;i<32;i++){
            r = (sourcePalette[i]>>8)&0xF;
            g = (sourcePalette[i]>>4)&0xF;
            b = (sourcePalette[i])&0xF;

            tr = (targetPalette[i]>>8)&0xF;
            tg = (targetPalette[i]>>4)&0xF;
            tb = (targetPalette[i])&0xF;

            r += (tr-r)*step/100;
            g += (tg-g)*step/100;
            b += (tb-b)*step/100;
        

            SetRGB4(viewport, i, r, g, b);
        }
}

void fadeColor(int colorIndex, UBYTE tr, UBYTE tg, UBYTE tb, WORD duration){
    int i;
    UBYTE r,g,b;
    int counter = 0;

    while (counter<=duration){
        while (*custom_vhposr != 0xff) ; // wait for vblank
        r = 0;
        g = 0;
        b = 0;

        r = tr*counter/duration;
        g = tg*counter/duration;
        b = tb*counter/duration;
        
        SetRGB4(viewport, colorIndex, r, g, b);

        counter++;
        WaitTOF();
     }
}

void fadeToBlack(unsigned short palette[],WORD duration){
    fadePalette(palette, colours_black, duration);
}

void fadeFromBlack(unsigned short palette[],WORD duration){
    fadePalette(colours_black, palette, duration);
}


void drawRect(int x, int y, int w, int h, int color){
    int right;
    SetAPen(&ctx, color);
    if (w<0) w=0;
    if (h<0) h=0;
    right = x + w - 1;
    if (right > BITMAPWIDTH - 1) right = BITMAPWIDTH - 1;
    RectFill(&ctx, x, y, right, y + h - 1);
}


void drawRectOutline(int x, int y, int w, int h, int color){
    SetAPen(&ctx, color);
    Move(&ctx, x, y);
    Draw(&ctx, x+w, y);
    Draw(&ctx, x+w, y+h);
    Draw(&ctx, x, y+h);
    Draw(&ctx, x, y);
}

void drawCirc(int x, int y, int r, int color){
    SetAPen(&ctx, color);
    DrawEllipse(&ctx, x, y, r, r);
    //DrawCircle(&ctx, x, y, r);
}

void drawPoly(int color){
     SetAPen(&ctx, color);
     PolyDraw(&ctx, 5, linearray);
}

void scrollScreen(int x, int y){
    viewport->RasInfo->RxOffset += x;
    viewport->RasInfo->RyOffset += y;
    ScrollVPort(viewport);
}

void commitScreen(){
    if (useDoubleBuffer){
        BltBitMap(&offScreenBitMap, 0, 0, &screenBitMap, 0, 0, BITMAPWIDTH, BITMAPHEIGHT, 0xC0, 0xFF, NULL);
    }
     WaitTOF();
}

void setDoubleBuffer(BOOL enabled){
    if (hasOffScreenContext){
        useDoubleBuffer = enabled;
        if (enabled){
            ctx = offScreenContext;
        }else{
            ctx = screenContext;
        }
        set_asset_screen();
    }else{
        fprintf(stderr, "setDoubleBuffer: no offscreen context\n");
    }
    
}

void waitVbl(void){
   while (*custom_vhposr != 0xff) ; 
}

void hideMouse(void){
    if (window) {
        cursor = AllocMem(128, MEMF_CHIP | MEMF_CLEAR);
        if (cursor) SetPointer(window, cursor, 1, 1, 0, 0);
    }
}

struct BitMap getScreenBitMap(void){
    return useDoubleBuffer ? offScreenBitMap : screenBitMap;
}

struct RastPort getScreenContext(void){
    return ctx;
}


