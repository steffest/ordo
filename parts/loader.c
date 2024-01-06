#include "proto.h"
#include <exec/memory.h>
#include <intuition/intuition.h>

int loader_box_Width = 100;
int loader_box_Height = 10;
int loader_box_X;
int loader_box_Y;
static struct RastPort loader_ctx;
BOOL is_low_memory = FALSE;

void drawBox(){
    SetAPen(&loader_ctx, 1);
    Move(&loader_ctx, loader_box_X, loader_box_Y);
    Draw(&loader_ctx, loader_box_X+loader_box_Width, loader_box_Y);
    Draw(&loader_ctx, loader_box_X+loader_box_Width, loader_box_Y+loader_box_Height);
    Draw(&loader_ctx, loader_box_X, loader_box_Y+loader_box_Height);
    Draw(&loader_ctx, loader_box_X, loader_box_Y);
}

void drawProgress(char progress){
    SetAPen(&loader_ctx, 2);
    RectFill(&loader_ctx, loader_box_X+1, loader_box_Y+1, loader_box_X+progress, loader_box_Y+loader_box_Height-1);
    commitScreen();
}

void drawDisk(){
    int x = loader_box_X + 38;
    int y = loader_box_Y-30;
    SetAPen(&loader_ctx, 2);
    Move(&loader_ctx, x, y);
    Draw(&loader_ctx, x+20, y);
    Draw(&loader_ctx, x+20+3, y+3);
    Draw(&loader_ctx, x+20+3, y+3+20);
    Draw(&loader_ctx, x, y+3+20);
    Draw(&loader_ctx, x, y);
    RectFill(&loader_ctx, x+6, y, x+17, y+7);
    RectFill(&loader_ctx, x+4, y+14, x+19, y+22);
    SetAPen(&loader_ctx, 0);
    RectFill(&loader_ctx, x+15, y+1, x+16, y+6);
    Move(&loader_ctx, x+7, y+17);
    Draw(&loader_ctx, x+17, y+17);
    Move(&loader_ctx, x+7, y+20);
    Draw(&loader_ctx, x+17, y+20);
}

void check_free_memory(void){
    int chipMem = AvailMem(MEMF_CHIP);
    if (chipMem<500000){
        is_low_memory = TRUE;
        printf("----------------------\n");
        printf("Chip memory is low ...\n");
        printf("Switching to Low Quality Audio\n");
        printf("Some intermediate loading will take place\n");
        printf("Please run this demo on a machine with at least 1MB Chip Ram\n");
        printf("----------------------\n");
        Delay(100);
    }
}

void print_free_memory(void){
    int chipMem = AvailMem(MEMF_CHIP);
    int fastMem = AvailMem(MEMF_FAST);
    printf("Memory available: Chip %d Fast  %d\n", chipMem,fastMem);
}

void start_loader(void){
    //int chipMem;
    //int fastMem;
    char index;

    //printf("Loading...\n");
    //chipMem = AvailMem(MEMF_CHIP);
    //fastMem = AvailMem(MEMF_FAST);
    //printf("Memory available: Chip %d Fast  %d\n", chipMem,fastMem);

    if (is_low_memory){
        index = loadFile("data/ordo_low.mod", 1);
    }else{
        index = loadFile("data/ordo.mod", 1);
    }
    
    if (WRITE_DEBUG) printf("Music loaded -> %d\n",index);
    drawProgress(10);

    index = intro_preLoad();
    if (WRITE_DEBUG) printf("Intro loaded -> %d\n",index);
    drawProgress(20);
    //printf("Memory available: Chip %d Fast  %d\n", AvailMem(MEMF_CHIP),AvailMem(MEMF_FAST));

    index = winter_preLoad();
    if (WRITE_DEBUG) printf("Winter loaded -> %d\n",index);
    drawProgress(30);
    if (index<0) return;
    //printf("Memory available: Chip %d Fast  %d\n", AvailMem(MEMF_CHIP),AvailMem(MEMF_FAST));

    index = waves_preLoad();
    if (WRITE_DEBUG) printf("Waves loaded-> %d\n",index);
    drawProgress(50);
    if (index<0) return;

    index = bears_preLoad(1);
    if (WRITE_DEBUG) printf("Bears 1 loaded-> %d\n",index);
    drawProgress(60);
    if (index<0) return;

    index = bears_preLoad(2);
    if (WRITE_DEBUG) printf("Bears 2 loaded-> %d\n",index);
    drawProgress(70);
    if (index<0) return;

    index = bears_preLoad(3);
    if (WRITE_DEBUG) printf("Bears 3 loaded-> %d\n",index);
    drawProgress(80);
    if (index<0) return;

    index = bears_preLoad(4);
    if (WRITE_DEBUG) printf("Bears 4 loaded-> %d\n",index);
    drawProgress(90);
    if (index<0) return;

    index = greetings_preLoad();
    if (WRITE_DEBUG) printf("Greetings-> %d\n",index);
    drawProgress(99);

}

void show_loader(void){
    int chipMem;
    int fastMem;
    char index;

    setColor(0,0,0,0);
    setColor(1,255,255,255);
    setColor(2,8,8,8);
    clearScreen();
    loader_ctx = getScreenContext();
    loader_box_X = 160 - loader_box_Width/2;
    loader_box_Y = 120 - loader_box_Height/2;

    drawBox();
    drawDisk();
    commitScreen();

    start_loader();
}
