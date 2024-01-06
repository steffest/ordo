#include "proto.h"
#include <stdio.h>
#include <exec/memory.h>
#include <intuition/intuition.h>

char introAsset;

static struct sparkle {
    int x;
    int y;
    int step;
    int size;
};

const unsigned short intro_palette[32] = {
    0X000,0X001,0X001,0X001,0X111,0X002,0X011,0X010,0X111,0X212,0X102,0X112,0X111,0X012,0X412,0X422,0X323,0X334,0X622,0X534,0X545,0Xa12,0X844,0Xa33,0X857,0Xb55,0Xe33,0X988,0Xf66,0Xf99,0X0d0,0Xfdd
    };


const unsigned short intro_palette_dark[32] = {
    0X000,0X001,0X001,0X001,0X101,0X002,0X011,0X010,0X111,0X212,0X102,0X112,0X111,0X012,0X612,0X422,0X323,0X334,0X622,0X534,0X545,0X912,0X744,0X933,0X756,0Xa55,0Xd33,0X978,0Xe66,0Xe99,0X000,0Xfdc
   };

   const unsigned short intro_palette_high[32] = 
   {0X000,0X001,0X001,0X001,0X101,0X002,0X011,0X010,0X111,0X212,0X102,0X112,0X111,0X012,0Xa24,0X533,0X445,0X545,0Xa44,0X746,0X63a,0Xe12,0Xd25,0Xb23,0Xb48,0Xf56,0Xf43,0Xc7a,0Xf76,0Xfaa,0X000,0Xfee };

const unsigned short intro_colours_black[32] = {
    0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,0x000, 0x000, 0x000, 0x000, 0x000,
    0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000, 0x000,0x000, 0x000, 0x000, 0x000, 0x000
};


static struct sparkle sparkles[20];
int sparkleCount = 0;

int sparkleIndex = 0;
int sparkleToggle = 0;
int maxSparkleIndex;
const unsigned short sparkleCoordinates[] = {
    92,79,
    98,81,
    104,85,
    107,90,
    108,96,
    107,101,
    105,105,
    103,107,
    100,111,
    95,113,
    89,113,
    83,110,
    79,105,
    76,100,
    74,94,
    77,88,
    81,83,
    87,81,
    129,112,
    129,106,
    129,101,
    128,95,
    130,89,
    129,82,
    132,79,
    139,79,
    145,82,
    150,85,
    151,89,
    150,93,
    148,96,
    144,98,
    143,101,
    148,106,
    151,111,
    173,112,
    172,107,
    173,101,
    174,94,
    174,89,
    172,83,
    175,79,
    181,79,
    186,80,
    192,83,
    198,87,
    200,93,
    200,100,
    198,106,
    194,109,
    190,112,
    185,112,
    181,113,
    177,113,
    229,79,
    236,82,
    240,85,
    244,91,
    244,96,
    244,103,
    240,109,
    233,112,
    226,111,
    219,109,
    213,103,
    211,96,
    213,90,
    216,86,
    220,81,
    224,80,
    230,80,
    237,82,
    242,87,
    244,92,
    244,99
};


//UBYTE speedRatio = 4;

void drawCross(int x, int y, int width, int height){
     drawAssetRect(introAsset, x - width/2, y+35, width, height, x - width/2, y+180); 
     drawAssetRect(introAsset, x, y+35 - width/2, height, width, x, y-width/2+180); 
}

void drawCross2(int x, int y, int width, int height){
     drawAssetRect(introAsset, x - width/2, y+15+20, width, height, x - width/2, y+15); 
     drawAssetRect(introAsset, x, y+15+20 - width/2, height, width, x, y-width/2+15); 
}

void createSparkle(int index){
    if (sparkleToggle==0 && sparkleIndex<maxSparkleIndex){
        sparkleToggle = 1;
        sparkles[index].x = sparkleCoordinates[sparkleIndex*2];
        sparkles[index].y = sparkleCoordinates[sparkleIndex*2+1] - 15;
        sparkles[index].step = 0;
        sparkles[index].size = 5 + random(10);
        sparkleIndex++;

    }else{
        sparkles[index].x = 10 + random(290);
        sparkles[index].y = 10 + random(120);
        sparkles[index].step = 0;
        sparkles[index].size = 5 + random(10);
        sparkleToggle = 0;
    }       
   
}

void updateSparkles(void){
    int i;
    for (i=0; i<sparkleCount; i++){
        sparkles[i].step++;
        if (sparkles[i].step<sparkles[i].size+1){
             drawCross(sparkles[i].x, sparkles[i].y, sparkles[i].step, 1);
        }else if (sparkles[i].step<(sparkles[i].size*2)+1){
             drawCross2(sparkles[i].x, sparkles[i].y, sparkles[i].step-sparkles[i].size, 1);
        }else{
            createSparkle(i);
        } 
    }
}



void intro_drawBackground(){
    char h = 10;
    int y = 0;
    while (y<230){
        drawAssetRect(introAsset, 0, y, 320, h, 0, 0); 
        y += 10;
        
    }
    commitScreen();
}

void intro_appear(){
    BOOL done = FALSE;
    int tick = 0;
    int y = 70;
    int direction = -1;
    int boxY = 0;
    int boxH = 0;

    while(!done){
        if (tick<318){
            boxY = tick%4 < 2 ? 0 : 80;
            //drawAssetRect(introAsset, tick, 15+20, 3, 160,tick, 180);
            drawAssetRect(introAsset, tick, 15+20 + boxY, 3, 80,tick, 180 + boxY);
            y = y + 4 * direction;
            if (tick % 20 == 0){
                direction = direction * -1;
            }
            drawRect(tick, y, 1, 5, tick % 4 < 2 ? 31 : 29);
        }
         
        if (tick>2){
            drawAssetRect(introAsset, tick-3, 20, 3, 180, tick-3, 0); 
        }
        tick += 2;
        commitScreen();

        if (tick<202) fadePaletteStep(intro_palette_high, intro_palette_dark, tick/2);
        

        if (tick > 320) done = TRUE;
    }
}

void intro_sparkle(void){
    int x = 0;
    int y = 0;
    int prevX = 0;
    int prevY = 0;
    BOOL done = FALSE;
    int tick = 0;
    int r,g,b;

    maxSparkleIndex = sizeof(sparkleCoordinates)/4;

    //Delay(50);
    //drawAssetRect(introAsset, 0, 20+15, 320, 160, 0, 180);  
    //commitScreen();    



    while (tick<400){

        if (sparkleCount<10){
            createSparkle(sparkleCount);
            sparkleCount++;
        }
       
       updateSparkles();
       commitScreen();
       tick++;

       if (tick<100){
        //fadeColor(30, 2, 11, 8, 120);
        r = 2*tick/100;
        g = 11*tick/100;
        b = 8*tick/100;
        setColor(30, r, g, b);
       }

       if (tick>300){
        fadePaletteStep(intro_palette_dark, intro_colours_black, tick-300);
       }
    
    }
}

void intro_text(){
    drawAssetRect(introAsset, 109, 180, 102, 16, 26, 341); 
    drawAssetRect(introAsset, 111, 202, 102, 9, 188, 345); 
    commitScreen();
}

char intro_preLoad(){
    //introAsset = loadFile("data/intro.planes",1);
    //createBitMap(introAsset, 5, 320, 360);
    introAsset = loadBitPlanes("data/intro.planes", 5, 320, 360);
    return introAsset;

}

void intro(void){
    
    setPaletteToBlack();
    clearScreen();
    commitScreen();  


    intro_drawBackground();
    commitScreen();  
    Delay(10);
    fadeFromBlack(intro_palette_high, 20);
    Delay(50);


    intro_appear();
    intro_text();

    

    //drawAssetRect(introAsset, 0, 20, 320, 180, 0, 0);  
    //commitScreen();    

    intro_sparkle();

    //printf("intro\n");
    Delay(50);

    // print size
    //printf("size of asset: %d\n", size);
   
    
    asset_free(introAsset);

}