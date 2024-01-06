#include "proto.h"
#include <stdio.h>
#include <exec/memory.h>
#include <intuition/intuition.h>

UBYTE speedRatio = 4;

char bearAsset;
char bearFightAsset;
char bearWalkingAsset;
char bearWildernessAsset;

int bstrokeStartX = 0;
int bstrokeStartY = 0;
int bstrokeEndX = 0;
int bstrokeEndY = 0;
int bstrokeSize = 0;
int bstrokeSpeed = 0;
int bstrokeProgress = 0;

const unsigned short bearStrokeCoordinates[] = {
    164,96, 0, 0,
    153,115, 10, 2,
    125,173, 10, 2,
    100,194, 10, 2,
    155,17, 0, 0,
    102,22, 10, 2,
    64,47, 10, 2,
    5,69, 10,2,
    27,56, 0,0,
    5,56, 10, 2,
    276,97, 0, 0,
    253,108, 10,4,
    199,147, 16, 6,
    173,201,16,6,
    196,18, 0,0,
    177,6, 6, 4,
    166,23, 10, 4,
    274,32, 0,0,
    249,22, 6, 6,
    196,12, 12, 6,
    284,31, 0, 0,
    290,44, 10, 4,
    258,69, 10, 4,
    257,83, 6, 4,
    273,91, 6,4,
    272,78, 12, 4,
    229,101, 0, 0,
    178,140, 16, 8,
    130,195, 10, 8,
    129,35, 0,0,
    133,121, 16,8,
    119,35, 0,0,
    123,121, 16,8,
    139,35, 0,0,
    143,121, 16,8,
    241,26,0,0,
    223,34,4,4,
    249,35, 0,0,
    234,46, 16,0,
    257,33, 0,0,
    221,42, 20,2,
    87,44,0,0,
    77,117,20,4,
    228,72,0,0,
    194,95,20,4,
};

char bears_preLoad(int index){
    switch (index){
    case 1:
        bearAsset = loadBitPlanes("data/bear.planes", 4, 296, 210);
        return bearAsset;
        break;
    case 2:
        bearFightAsset = loadBitPlanes("data/bears.planes", 4, 320, 240);
        return bearFightAsset;
        break;
    case 3:
        bearWalkingAsset = loadBitPlanes("data/bears_walking.planes", 4, 320, 240);
        return bearFightAsset;
        break;    
    case 4:
        bearWildernessAsset = loadBitPlanes("data/bear_wilderness.planes", 4, 320, 240);
        return bearWildernessAsset;
        break;      
    default:
        printf("bears_preLoad: unknown index %d\n",index);
        return -1;
    }
}

void drawbearTile(SHORT x,SHORT y, SHORT w,SHORT h,short sourceX){

   drawAssetRect(bearFightAsset, x, y, w, h, sourceX, y); 
   drawRectOutline(x-1, y-1, w+1, h+1, 10);


}

void drawBearLine(int x, int y, int x2, int y2, int size, int speed){
     bstrokeProgress = 0;
     while (bstrokeProgress<100){
            x = linear(bstrokeProgress, x, x2 - x);
            y = linear(bstrokeProgress, y, y2 - y);

            //drawTreeCross(x, y, 8, 4);
           
            drawAssetRect(bearAsset, x - size/2, y - size/2 + 30, size, size, x - size/2, y - size/2); 
            bstrokeProgress += speed;
            
            commitScreen();
        }
}

void bearPainting(){
    short max = 0;
    int x = 294;
    short lineIndex = 0;
    const unsigned short palette[16] = {0X000,0X000,0X100,0X111,0X211,0X411,0X422,0X611,0X811,0X732,0Xa32,0Xc42,0Xa65,0Xd63,0Xe84,0Xdb9};
    
    clearScreen();
    commitScreen();

    setPalette(palette);

    max = sizeof(bearStrokeCoordinates)/2-3;
    while (lineIndex<max){
        bstrokeStartX = bstrokeEndX;
        bstrokeStartY = bstrokeEndY;
        bstrokeEndX = bearStrokeCoordinates[lineIndex++];
        bstrokeEndY = bearStrokeCoordinates[lineIndex++];
        bstrokeSize = bearStrokeCoordinates[lineIndex++];
        bstrokeSpeed = bearStrokeCoordinates[lineIndex++];

         if (bstrokeSpeed>0){
            drawBearLine(bstrokeStartX, bstrokeStartY, bstrokeEndX, bstrokeEndY, bstrokeSize, bstrokeSpeed);
        }
    }

    while (x>=0){
        drawAssetRect(bearAsset, x, 30, 2, 210, x,0); 
        commitScreen();
        x-=2;
    }

    Delay(50);
    fadeToBlack(palette, 100);
    
    clearScreen();
    commitScreen();


    //drawAssetRect(bearAsset, 0, 30, 296, 210, 0,0); 
    //commitScreen();   
}

void bearFight(void){
    int x;
    int y;
    int p=10;
    const unsigned short palette[32] = {
        0X122,0X322,0X532,0X244,0X444,0X841,0Xb41,0Xe50,0Xc71,0X887,0Xe70,0Xd94,0Xf90,0Xccc,0Xfc8,0Xfff,
        0X122,0X322,0X532,0X244,0X444,0X841,0Xb41,0Xe50,0Xc71,0X887,0Xe70,0Xd94,0Xf90,0Xccc,0Xfc8,0Xfff
    };

    setPalette(palette);

    clearScreen();
    commitScreen();

    x=0;
    while (x<60){
        drawRect(x-2,29,2,102,0);
        drawbearTile(x,30,100,100,60+10);
        commitScreen();
        x+=1;
    }
    while (p>0){
        drawbearTile(x,30,100,100,60+p);
        commitScreen();
        p--;
    }
    x=162;
    while (x<220){
        drawAssetRect(bearFightAsset, x, 30, 4, 100, x+35+60, 0);
        commitScreen();
        x+=4; 
    }

    Delay(20);

    x=220;
    y=150;

   while(x>180){
        drawRect(x+81,y-1,2,82,0);
        drawAssetRect(bearFightAsset, x, y, 80, 80, 170-8, 62); 
        drawRectOutline(x-1, y-1, 80+1, 80+1, 10);
        commitScreen();
        x--;
   } 
    p=7;
    while (p>0){
        drawAssetRect(bearFightAsset, x+1, y, 80, 80, 170-p, 62); 
        commitScreen();
        p--;
    }

    x=178;
    while (x>100){
        drawAssetRect(bearFightAsset, x-4, y, 4, 80, x-90, 159);
        commitScreen();
        x-=4; 
    }

    Delay(50);
    fadeToBlack(palette, 40);

    clearScreen();
    commitScreen();

    drawAssetRectPlane(bearFightAsset, 0, 0, 320, 240, 0,0,0x02);
    commitScreen();

    fadeFromBlack(palette, 60);

    x=0;
    y=33;
    while (x<320){
        drawAssetRect(bearFightAsset, x, y, 2, 90, x, y); 
        commitScreen();
        x += 2;
    }
    y=32;
    x=122; 
    while(x<320){
        if (y>0) drawAssetRect(bearFightAsset, 0, y, 320, 1, 0, y); 
        if (x<240) drawAssetRect(bearFightAsset, 0, x, 320, 1, 0, x); 
        commitScreen();
        y--;
        x++;
    }

    fadeToBlack(palette, 40);
    
}

void bearWalk(){
    int x;
    int y;

    const unsigned short palette[32] = {
        0X111,0X322,0X643,0X952,0X766,0X976,0Xb74,0Xa87,0Xd70,0Xe83,0Xba9,0Xfa6,0Xdcb,0Xfc9,0Xeed,0Xfff,
        0X111,0X322,0X643,0X952,0X766,0X976,0Xb74,0Xa87,0Xd70,0Xe83,0Xba9,0Xfa6,0Xdcb,0Xfc9,0Xeed,0Xfff
        };
    clearScreen();
    setPaletteToBlack();   
    commitScreen(); 

    drawAssetRectPlane(bearFightAsset, 0, 0, 320, 240, 0,0,0x03);
    commitScreen();

    fadeFromBlack(palette, 60);


    x=0;
    y=33;
    while (x<320){
        drawAssetRect(bearWalkingAsset, x, y, 4, 90, x, y); 
        commitScreen();
        x += 4;
    }


    y=32;
    x=122; 
    while(x<320){
        if (y>0) drawAssetRect(bearWalkingAsset, 0, y, 320, 1, 0, y); 
        if (x<240) drawAssetRect(bearWalkingAsset, 0, x, 320, 1, 0, x); 
        commitScreen();
        y--;
        x++;
    }

    Delay(50);
    fadeToBlack(palette, 60);
    clearScreen();
    commitScreen();

}

void bearSunrise(){
    int x;
    int y;
    int p=0;
    const unsigned short palette[32] = {
        0X000,0X200,0X411,0X333,0X611,0X543,0X901,0X842,0Xc30,0X777,0Xb62,0Xf50,0Xf80,0Xfa0,0Xfc4,0Xffe,
        0X000,0X200,0X411,0X333,0X611,0X543,0X901,0X842,0Xc30,0X777,0Xb62,0Xf50,0Xf80,0Xfa0,0Xfc4,0Xffe,
    };

    const unsigned short palette_dark[32] = {
        0X000,0X100,0X211,0X111,0X211,0X321,0X201,0X321,0X410,0X532,0X531,0X723,0X935,0Xd44,0Xe52,0Xf75,
        0X000,0X100,0X211,0X111,0X211,0X321,0X201,0X321,0X410,0X532,0X531,0X723,0X935,0Xd44,0Xe52,0Xf75,
    };

    const unsigned short palette_night[32] = {
        0X000,0X000,0X101,0X111,0X111,0X122,0X212,0X112,0X111,0X232,0X333,0X323,0X535,0X944,0Xa53,0X875,
        0X000,0X000,0X101,0X111,0X111,0X122,0X212,0X112,0X111,0X232,0X333,0X323,0X535,0X944,0Xa53,0X875,
    };

    const unsigned short palette_bright[32] = {
        0X000,0X211,0X311,0X234,0X543,0X543,0X756,0X877,0X985,0X777,0Xa64,0X7ba,0Xda9,0Xfc8,0Xff8,0Xffe,
        0X000,0X211,0X311,0X234,0X543,0X543,0X756,0X877,0X985,0X777,0Xa64,0X7ba,0Xda9,0Xfc8,0Xff8,0Xffe
    };  

    const unsigned short palette_yellow[32] = {
        0Xfd8,0Xfd8,0Xfd8,0Xfd8,0Xfd8,0Xfd8,0Xfd8,0Xfd8,0Xfd8,0Xfd8,0Xfd8,0Xfd8,0Xfd8,0Xfd8,0Xfd8,0Xfd8,
        0Xfd8,0Xfd8,0Xfd8,0Xfd8,0Xfd8,0Xfd8,0Xfd8,0Xfd8,0Xfd8,0Xfd8,0Xfd8,0Xfd8,0Xfd8,0Xfd8,0Xfd8,0Xfd8,
    };  


    setPaletteToBlack();
    commitScreen();
    Delay(10);
    drawAssetRect(bearWildernessAsset, 216, 0, 104, 240,216,0);
    commitScreen();
    fadeFromBlack(palette_night, 200);

    y=0;
    while (y<240){
        drawAssetRect(bearWildernessAsset, 120, y, 116, 2,120,y);
        commitScreen();
        y+=4;
        p++;
        fadePaletteStep(palette_night, palette_dark, p);
    }
    y=238;
    while (y>0){
        drawAssetRect(bearWildernessAsset, 120, y, 116, 2,120,y);
        commitScreen();
        y-=4;
        p++;
        if (p<=100)fadePaletteStep(palette_night, palette_dark, p);
    }

    x=2;
    p=0;
    while(x<=120){
        drawAssetRect(bearWildernessAsset, 0, 0, x,240,120-x,0);
        fadePaletteStep(palette_dark, palette, p/2);
        commitScreen();
        x++;
        p++;
    }

    while (p<200){
        waitVbl();
        fadePaletteStep(palette_dark, palette, p/2);
        commitScreen();
        p+=2;
    }

    Delay(60);
    fadePalette(palette, palette_bright, 200);
    Delay(60);
    fadePalette(palette_bright, palette_yellow, 200);
    clearScreen();
    commitScreen();
    fadeToBlack(palette_yellow, 50);
}

void bears(void){
   
    bearPainting();
    bearFight();
    bearWalk();
    bearSunrise();   
    Delay(50);
}