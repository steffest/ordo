#include <proto/graphics.h>
#include <intuition/intuition.h>
#include <exec/types.h>
#include "screen.h"

#define SCREENWIDTH 320
#define SCREENHEIGHT 240
#define TILESIZE 16
#define PLANES 5 // 16 colors


static struct RastPort spriteContext;
static struct RastPort maskContext;
static struct BitMap spriteBitmap;
struct BitMap maskBitmap;

const unsigned short __chip shape[] = {
	//Bitplane 0
	0xFFFF,0x0000,0xFFFF,0x0000,0xFFFF,0x0000,0xFFFF,0x0000,0xFFFF,0x0000,0xFFFF,0x0000,0xFFFF,0x0000,0xFFFF,0x0000
};


const UWORD __chip ball[] = {

   // Bit Plane #0 

   0x0000,
   0x0020,
   0x0410,
   0x0408,
   0x0234,
   0x0632,
   0x003A,
   0x0388,
   0x0382,
   0x0F6A,
   0x0C72,
   0x0C0C,
   0x0200,
   0x0710,
   0x0100,
   0x0000,

   // Bit Plane #1 

   0x0000,
   0x05C0,
   0x1F30,
   0x0834,
   0x38C0,
   0x40C0,
   0x4F80,
   0x5C44,
   0x7C44,
   0x6084,
   0x2030,
   0x1070,
   0x18F0,
   0x0C08,
   0x0400,
   0x0000,

   // Bit Plane #2 

   0x07E0,
   0x1898,
   0x2014,
   0x4102,
   0x610A,
   0x810D,
   0x804D,
   0x8033,
   0x8039,
   0x8071,
   0x838D,
   0x438E,
   0x400A,
   0x2464,
   0x1BF8,
   0x07E0,

   // Bit Plane #3 

   0x07E0,
   0x1A38,
   0x20CC,
   0x72CA,
   0x4636,
   0xBE33,
   0xB033,
   0xA3CD,
   0x83C7,
   0x9F8F,
   0xDC73,
   0x6C72,
   0x6776,
   0x339C,
   0x1818,
   0x07E0
};



const UWORD __chip ball2[] = {

   // Bit Plane #0 

   0x0000,
   0x0020,
   0x0410,
   0x0408,
   0x0234,
   0x0632,
   0x003A,
   0x0388,
   0x0382,
   0x0F6A,
   0x0C72,
   0x0C0C,
   0x0200,
   0x0710,
   0x0100,
   0x0000,

   // Bit Plane #1 

   0x0000,
   0x05C0,
   0x1F30,
   0x0834,
   0x38C0,
   0x40C0,
   0x4F80,
   0x5C44,
   0x7C44,
   0x6084,
   0x2030,
   0x1070,
   0x18F0,
   0x0C08,
   0x0400,
   0x0000,

   // Bit Plane #2 

   0x07E0,
   0x1898,
   0x2014,
   0x4102,
   0x610A,
   0x810D,
   0x804D,
   0x8033,
   0x8039,
   0x8071,
   0x838D,
   0x438E,
   0x400A,
   0x2464,
   0x1BF8,
   0x07E0,

   // Bit Plane #3 

   0x07E0,
   0x1A38,
   0x20CC,
   0x72CA,
   0x4636,
   0xBE33,
   0xB033,
   0xA3CD,
   0x83C7,
   0x9F8F,
   0xDC73,
   0x6C72,
   0x6776,
   0x339C,
   0x1818,
   0x07E0,

   // Bit Plane #4

   0xFFFF,
   0xFFFF,
   0xFFFF,
   0xFFFF,
   0xFFFF,
   0xFFFF,
   0xFFFF,
   0xFFFF,
   0xFFFF,
   0xFFFF,
   0xFFFF,
   0xFFFF,
   0xFFFF,
   0xFFFF,
   0xFFFF,
   0xFFFF
};

const UWORD __chip ballmask[] = {
   0x07E0,
   0x1FF8,
   0x3FFC,
   0x7FFE,
   0x7FFE,
   0xFFFF,
   0xFFFF,
   0xFFFF,
   0xFFFF,
   0xFFFF,
   0xFFFF,
   0x7FFE,
   0x7FFE,
   0x3FFC,
   0x1FF8,
   0x07E0
};


    struct Image ballImage = {
			0, 0,     // LeftEdge, TopEdge
			TILESIZE, TILESIZE, 4,  // Width, Height, Depth
			&ball[0],
			0x0F,0,  // PlanePick, PlaneOnOff
			NULL  // NextImage pointer
	};

    struct Image ballImage2= {
			0, 0,     // LeftEdge, TopEdge
			TILESIZE, TILESIZE, 5,  // Width, Height, Depth
			&ball2[0],
			0x1F,0,  // PlanePick, PlaneOnOff
			NULL  // NextImage pointer
	};









void drawTile(UBYTE index, UBYTE x, UBYTE y, UBYTE bitPlanes){
    struct BitMap screenBitmap = getScreenBitMap();
    WORD planeMask = 0x0F;
    if (bitPlanes>4) planeMask = 0xFF;

    //ClipBlit(&spriteContext, 0, sourceY, &screenContext, x, y, 20, 20, ABC|ABNC);

   BltBitMap(
            &spriteBitmap,   // Source
            index*TILESIZE, 0,      // Position, source.
            &screenBitmap,   // Destination.
            x,          // Position, destination.
            y,
            TILESIZE, TILESIZE,          // Width and height.
            0xC0,            // Normal copy.
            planeMask,            // All bitplanes.
            NULL );          // No temporary storage.

}

/*void drawMaskedTile(UBYTE x, UBYTE y){
    struct BitMap screenBitmap = getScreenBitMap();


    // clear the area first with a mask
    BltBitMap(
            &maskBitmap,   // Source
            0, 0,      // Position, source.
            &screenBitmap,   // Destination.
            x,          // Position, destination.
            y,
            TILESIZE, TILESIZE,          // Width and height.
            ANBC,            // Copy inverted source to destination through mask
            0xFF,            // All bitplanes.
            NULL );          // No temporary storage.


    // then blit the image cookie cut out
    BltBitMap(
            &spriteBitmap,   // Source
            0, 0,      // Position, source.
            &screenBitmap,   // Destination.
            x,          // Position, destination.
            y,
            TILESIZE, TILESIZE,          // Width and height.
            0x60,           
            0xFF,            // All bitplanes.
            NULL );          // No temporary storage.


}*/

void drawTileBitmap(UBYTE x, UBYTE y){
    struct RastPort ctx = getScreenContext();
    DrawImage(&ctx,&ballImage2,x,y);

}

void drawMaskedTile(UBYTE x, UBYTE y){
    struct RastPort ctx = getScreenContext();

    //ClipBlit(&spriteContext, 0, sourceY, &screenContext, x, y, 20, 20, ABC|ABNC);

   BltMaskBitMapRastPort(
            &spriteBitmap,   // Source
            0, 0,      // srcx, srcy
            &ctx,   
            x, y,       //  destX, destY
            TILESIZE,TILESIZE,          // Width and height.
          ABC|ABNC|ANBC, ballmask);      

}

/*void drawMaskedTile(UBYTE index, UBYTE x, UBYTE y){
    struct RastPort ctx = getScreenContext();

    //ClipBlit(&spriteContext, 0, sourceY, &screenContext, x, y, 20, 20, ABC|ABNC);

   BltBitMapRastPort(
            &spriteBitmap,   // Source
            0, 0,      // srcx, srcy
            &ctx,   
            x, y,       //  destX, destY
            TILESIZE,TILESIZE,          // Width and height.
          0xC0);      

}*/

void drawPixelTile(UBYTE x, UBYTE y){
    int _x,_y;
    int width = TILESIZE;
    int height = TILESIZE;
    struct RastPort ctx = getScreenContext();
    for (_y = 0; _y < height; _y++) {
        for (_x = 0; _x < width; _x++) {
            int color = (_x ^ _y) & 31;
            if (color>0){
                SetAPen(&ctx,color);
                WritePixel(&ctx,_x+x,_y+y);
            }
        }
    }
}