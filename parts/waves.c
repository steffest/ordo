#include "proto.h"
#include <intuition/intuition.h>


char waveAsset;
char waves_offsetX = 16;
char waves_margin = 16;
BOOL waves_isMoving = FALSE;

BOOL isSpeedy = TRUE;


struct wave_block_movement {
    char index;
    short fromY;
    short toY;
    short delta;
    short speed;
    short tick;

};
struct wave_block_movement wave_block_movements[4];

int sinusTable[] = {
    0, 5, 10, 15, 20, 26, 31, 36, 41, 46, 51, 56, 60, 65, 70, 74, 78, 82, 86, 90, 93, 96, 99, 102, 104, 106, 108, 109, 110, 111, 112, 112, 112, 111, 110, 109, 108, 106, 104, 102, 99, 96, 93, 90, 86, 82, 78, 74, 70, 65, 60, 56, 51, 46, 41, 36, 31, 26, 20, 15, 10, 5, 0, -5, -10, -15, -20, -26, -31, -36, -41, -46, -51, -56, -60, -65, -70, -74, -78, -82, -86, -90, -93, -96, -99, -102, -104, -106, -108, -109, -110, -111, -112, -112, -112, -111, -110, -109, -108, -106, -104, -102, -99, -96, -93, -90, -86, -82, -78, -74, -70, -65, -60, -56, -51, -46, -41, -36, -31, -26, -20, -15, -10, -5
};

const unsigned short waves_palette[32] = {
    0X000,0X210,0X121,0X610,0X430,0X331,0X730,0X810,0X553,0X652,0X840,0Xa30,0X074,0Xc10,0X674,0X961,0Xc50,0Xd40,0X973,0X886,0Xd61,0X0a5,0Xc83,0Xd73,0Xa96,0Xd95,0Xcb9,0Xeb7,0Xdca,0Xfd7,0Xedc,0Xfff
};


char waves_preLoad(void){
    waveAsset = loadBitPlanes("data/waves.planes", 5, 241, 376);
    return waveAsset;
    //waveAsset = loadFile("data/waves.planes", 0);
}

void waves_drawBar(char index, short sourceY, char blendMode){
    int sourceX = index*60;
    int x = waves_offsetX + waves_margin*index + 60*index;
    int width = 60;
    int height = 240;
    int y = 0;

    if (index==1){
        sourceX = 61;
        width = 59; // WTF? no time anymore to fix this
    }

    if (sourceY<0){
        y = 0 - sourceY;
        height = height + sourceY;
        sourceY = 0;
    }
    if (sourceY>136){
        sourceY=136;
    }
    
    drawAssetRectBlend(waveAsset, x, y, width, height, sourceX, sourceY, blendMode);  
    //commitScreen(); 
}

void waves_moveBar(char index, short fromY, short toY){
    int steps;
    int step=2;
    int tick=0;
    steps = (toY - fromY)/step + 1;
    if (steps<0){
        steps = steps * -1;
        step = step * -1;
    }
    while (tick<steps){
        waves_drawBar(index, fromY, 0xC0);
        fromY += step;
        tick++;
    }
}

void waves_moveBar_easing(char index, short fromY, short toY, short speed){
    /*int delta = toY - fromY;
    int tick=0;
    int y;
    
    while (tick<100){
        y = easeInOutQuad(tick, fromY, delta);
        waves_drawBar(index, y);
        tick += speed;
    }*/

    wave_block_movements[index].fromY = fromY;
    wave_block_movements[index].toY = toY;
    wave_block_movements[index].delta = toY - fromY;
    wave_block_movements[index].speed = speed;
    wave_block_movements[index].tick = 0;
    waves_isMoving = TRUE;
}

void waves_updateBars(int blendMode){
    int i;
    int y;
    waves_isMoving = FALSE;
    for (i=0;i<4;i++){
        if (wave_block_movements[i].speed>0 && wave_block_movements[i].tick<=100){
             y = easeInOutQuad(wave_block_movements[i].tick, wave_block_movements[i].fromY, wave_block_movements[i].delta);
             wave_block_movements[i].tick += wave_block_movements[i].speed;
             waves_drawBar(i, y,blendMode);
             waves_isMoving = TRUE;
        }
    }
}

void waves_draw_char(int col,int width,int gap,int wOffset){
    int x = waves_offsetX + waves_margin*col + 60*col;
    int colWidth = 47 + wOffset*2;
    int right=colWidth-width;
    int color = 21;
    x -= wOffset;

    if (col == 0 || col == 3){
        if (col == 3) color = 20;
        x += 13;
        drawRect(x + gap, 1, colWidth-gap*2, width, color);
        drawRect(x, gap, width, 240-gap*2, color);
        drawRect(x + gap, 240-width, colWidth-gap*2, width, color);
        drawRect(x+right, gap, width, 240-gap*2, color);
    }

    if (col == 1){
        color = 29;
        x += 12;
        drawRect(x, 1, colWidth-gap, width, color);
        drawRect(x, width, width, 240-width, color);
        drawRect(x+width, 60+gap/2, colWidth-gap-width, width, color);
        drawRect(x+right, gap, width, 60-gap/2, color);
        drawRect(x+right, 60+gap+gap/2, width, 240-60-gap-gap/2,color);
    }

    if (col == 2){
        color = 13;
        x += 12;
        drawRect(x, 1, colWidth-gap, width, color);
        drawRect(x, width, width, 240-width,color);
        drawRect(x, 240-width, colWidth-gap, width, color);
        drawRect(x+right, gap, width, 240-gap*2, color);
    }
   
}

void waves_updateChars(void){
    int i;
    int y;
    int gap;
    int width;
    int wOffset = 0;
    int moveStep = 5;
    if (isSpeedy) moveStep = 2;
    waves_isMoving = FALSE;
    for (i=0;i<4;i++){
        if (wave_block_movements[i].speed>0 && wave_block_movements[i].tick<124){
            gap = 9 - sinusTable[wave_block_movements[i].tick]/10 * wave_block_movements[i].fromY;
            width = 14 + sinusTable[wave_block_movements[i].tick]/10 * wave_block_movements[i].toY;
            wOffset = sinusTable[wave_block_movements[i].tick]/10 * wave_block_movements[i].delta;
            if (gap<0) gap = 0;
            if (width<1) width = 1;
            waves_draw_char(i, width, gap, wOffset);
            wave_block_movements[i].tick += moveStep;
            waves_isMoving = TRUE;
        }else{
            waves_draw_char(i,14,9,0);
        }
    }
}

void waves_appear(int blendMode){
    int tick=0;
    waves_moveBar_easing(0,-235,0,4);
    while (waves_isMoving){
        waves_updateBars(blendMode);
        commitScreen();
        tick++;

        if (tick==10){
            waves_moveBar_easing(1,-235,0,4);
        }

        if (tick==20){
            waves_moveBar_easing(2,-235,0,4);
        }

        if (tick==30){
            waves_moveBar_easing(3,-235,0,4);
        }
    }
}

void waves_appear2(int blendMode){
    int tick=0;
    waves_moveBar_easing(0,0,100,4);
    while (waves_isMoving){
        waves_updateBars(blendMode);
        commitScreen();
        tick++;

        if (tick==10){
            waves_moveBar_easing(1,0,100,4);
        }

        if (tick==20){
            waves_moveBar_easing(2,0,100,4);
        }

        if (tick==30){
            waves_moveBar_easing(3,0,100,4);
        }
    }
}

void waves_ordo(){
    int tick=0;
    waves_moveBar_easing(0,0,48,2);
    while (waves_isMoving){
        waves_updateBars(0xc0);
        commitScreen();
        tick++;

        if (tick==10){
            waves_moveBar_easing(1,0,48,4);
        }

        if (tick==20){
            waves_moveBar_easing(2,0,48,4);
        }

        if (tick==30){
            waves_moveBar_easing(3,0,48,4);
        }
    }
}

void waves_peak(){
    int tick=0;
    waves_moveBar_easing(0,48,64,2);
    while (waves_isMoving){
        waves_updateBars(0xc0);
        commitScreen();
        tick++;
    }
    waves_moveBar_easing(3,48,90,2);
    while (waves_isMoving){
        waves_updateBars(0xc0);
        commitScreen();
        tick++;
    }
    Delay(100);
    tick=0;
    waves_moveBar_easing(0,64,48,2);
    while (waves_isMoving){
        waves_updateBars(0xc0);
        commitScreen();
        tick++;

        if (tick==10){
            waves_moveBar_easing(3,90,48,4);
        }
        if (tick==40){
            waves_moveBar_easing(1,48,136,1);
        }
    }
    Delay(50);
    waves_moveBar_easing(0,48,136,2);
    while (waves_isMoving){
        waves_updateBars(0xc0);
        commitScreen();
    }
    waves_moveBar_easing(2,48,136,2);
    while (waves_isMoving){
        waves_updateBars(0xc0);
        commitScreen();
    }
    waves_moveBar_easing(3,48,136,2);
    while (waves_isMoving){
        waves_updateBars(0xc0);
        commitScreen();
    }

    Delay(100);
}

void waves_intro(void){
    BOOL done = FALSE;
    int tick = 0;

    const unsigned short palette_white[32] = {
       0X000,0Xfff,0Xfff,0X222,0X222,0X333,0X333,0X333,0X444,0X555,0X444,0X444,0X444,0X444,0X555,0X555,0X555,0X666,0X666,0X777,0X777,0X555,0X777,0X777,0X888,0X888,0Xaaa,0Xaaa,0Xbbb,0Xccc,0Xeee,0Xfff
    };

    const unsigned short palette_colorful[32] ={0X000,0X210,0X121,0X610,0X430,0X331,0X730,0X810,0X553,0X652,0X840,0Xa30,0X074,0Xa1f,0X674,0X961,0Xc50,0Xd40,0X973,0X886,0X1db,0Xb9e,0Xc83,0Xd73,0Xa96,0Xd95,0Xcb9,0Xeb7,0Xdca,0X1a7,0Xedc,0Xfff};


    setDoubleBuffer(TRUE);
    setPalette(palette_colorful);
    WaitTOF();

    waves_draw_char(0,14,9,0);
    commitScreen(); 
    Delay(50);
    waves_draw_char(1,14,9,0);
    commitScreen(); 
    Delay(50);
    waves_draw_char(2,14,9,0);
    commitScreen(); 
    Delay(50);
    waves_draw_char(3,14,9,0);
    commitScreen(); 

    wave_block_movements[0].speed=1;
    wave_block_movements[0].tick=0;
    wave_block_movements[0].fromY=-1;
    wave_block_movements[0].toY=1;
    wave_block_movements[0].delta=-1;
    waves_isMoving = TRUE;

    while (waves_isMoving){
        if (tick<=100){
            fadePaletteStep(palette_colorful, waves_palette, tick);
        }
        if (tick<130){
            clearScreen();
        }
        waves_updateChars();
        commitScreen();
        tick++;

        if (tick==10){
             wave_block_movements[1].speed=1;
             wave_block_movements[1].tick=0;
             wave_block_movements[1].fromY=-1;
             wave_block_movements[1].toY=1;
             wave_block_movements[1].delta=1;
        }

        if (tick==20){
             wave_block_movements[2].speed=1;
             wave_block_movements[2].tick=0;
             wave_block_movements[2].fromY=1;
             wave_block_movements[2].toY=1;
             wave_block_movements[2].delta=1;
        }

         if (tick==30){
             wave_block_movements[3].speed=1;
             wave_block_movements[3].tick=0;
             wave_block_movements[3].fromY=1;
             wave_block_movements[3].toY=-1;
             wave_block_movements[3].delta=-1;
        }


        if (tick>150) waves_isMoving = FALSE;
    }
    setDoubleBuffer(FALSE);
    setPalette(waves_palette);

    Delay(50);

    //waves_drawBar(0, 0, 40);
    //commitScreen(); 
    //Delay(50);
    //waves_drawBar(1, 0, 40);
    //commitScreen(); 
    //Delay(50);


    //Delay(100);
    
    
}

void waves(void){
    BOOL done = FALSE;
    int tick = 0;
    int time = 0;
    int fps = 50;
    int minFps = 1000;
    int defaultSpeed = 2;
    int i;

    const unsigned short palette_grey[32] = {
       0X000,0X111,0X111,0X222,0X222,0X333,0X333,0X333,0X444,0X555,0X444,0X444,0X444,0X444,0X555,0X555,0X555,0X666,0X666,0X777,0X777,0X555,0X777,0X777,0X888,0X888,0Xaaa,0Xaaa,0Xbbb,0Xccc,0Xeee,0Xfff
    };

   


    clearScreen();
    setPalette(waves_palette);
    commitScreen(); 

    waves_intro();

    for (i=0;i<4;i++){
        wave_block_movements[i].speed = 0;
    }
   

    waves_appear(40);
    waves_appear(0XC0);

    commitScreen();
    defaultSpeed = 5;

    Delay(50);
    waves_ordo();
    Delay(100);
    waves_peak();

    waves_appear(40);
    waves_appear2(40);

    fadeToBlack(waves_palette, 40);
    clearScreen();
    commitScreen();
    

    /*while (!done){
        waves_updateBars();
        commitScreen();
        tick++;
        if (tick>100) done = TRUE;

        if (tick==10){
            waves_moveBar_easing(0,-220,0,defaultSpeed);
        }
        if (tick==15){
            waves_moveBar_easing(1,-240,0,defaultSpeed);
        }
        if (tick==20){
            waves_moveBar_easing(2,-240,0,defaultSpeed);
        }
        if (tick==25){
            waves_moveBar_easing(3,-240,0,defaultSpeed);
        }
        
        time = get_timer();
        if (time>0){
            fps = (fps + 1000/time)/2;
            if (fps<minFps){
                minFps = fps;
            }
        }

    }*/


    //waves_appear(0);

    /*init_timer();
    waves_drawBar(0, 0);
    waves_drawBar(1, 0);
    waves_drawBar(2, 0);
    waves_drawBar(3, 0);
    commitScreen();
    time = get_timer();
    printf("time: %d\n", time);
    
    if (time>30){
        // we're one a slow (or real) system, so we need to speed up the animation
        defaultSpeed = 4;
        if (time>45) defaultSpeed = 5;
    }

    
    while (!done){
        waves_updateBars();
        commitScreen();
        tick++;
        if (tick>100) done = TRUE;

        if (tick==10){
            waves_moveBar_easing(0,0,48,defaultSpeed);
        }
        if (tick==15){
            waves_moveBar_easing(1,0,48,defaultSpeed);
        }
        if (tick==20){
            waves_moveBar_easing(2,0,48,defaultSpeed);
        }
        if (tick==25){
            waves_moveBar_easing(3,0,48,defaultSpeed);
        }
        
        time = get_timer();
        if (time>0){
            fps = (fps + 1000/time)/2;
            if (fps<minFps){
                minFps = fps;
            }
        }

    }

    printf("waves: %dfps\n", minFps);*/
    

    
   
    //drawAssetRect(waveAsset, 0, 0, 241, 240, 0, 0);  
    //commitScreen();    

    //waves_moveBar(0,0,48);
    //waves_moveBar(1,0,48);
    //waves_drawBar(0, 0);
    //Delay(20);
    //waves_drawBar(1, 0);
    //Delay(20);
    //waves_drawBar(2, 0);
    //Delay(20);
    //waves_drawBar(3, 0);
    //Delay(20);

    

    /*waves_moveBar(0,0,48);
    Delay(10);
    waves_moveBar(1,0,48);
    Delay(10);
    waves_moveBar(2,0,48);
    Delay(10);
    waves_moveBar(3,0,48);
    Delay(10);

    waves_moveBar(0,48,80);
    Delay(20);

    waves_moveBar(2,48,16);
    Delay(20);
    waves_moveBar(2,16,100);
    Delay(20);
    waves_moveBar(2,100,48);
    waves_moveBar(0,80,48);*/

    Delay(10);
    
    asset_free(waveAsset);

    Delay(10);

}