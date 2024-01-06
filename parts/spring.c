#include "proto.h"
char treeAsset;

int strokeStartX = 0;
int strokeStartY = 0;
int strokeEndX = 0;
int strokeEndY = 0;
int strokeSize = 0;
int strokeSpeed = 0;
int strokeProgress = 0;

static struct blossom {
    int x;
    int y;
    int step;
    int size;
};

static struct blossom blossoms[10];
int blossomCount = 0;
int blossomsDone = 0;
int blossomIndex = 0;

int tree_color_step = 0;
int tree_palette_change = 0;

const unsigned short tree_palette[32] = {
       0X111,0X332,0X136,0X157,0X169,0X566,0X18b,0X29c,0X5ac,0X972,0Xaa8,0Xbcd,0Xc21,0Xeb2,0Xeda,0Xfff,
       0X111,0X332,0X136,0X157,0X169,0X566,0X18b,0X29c,0X5ac,0X972,0Xaa8,0Xbcd,0Xc21,0Xeb2,0Xeda,0Xfff,
        };


const unsigned short tree_palette_purple[32] ={
    0X111,0X312,0X524,0X745,0X968,0Xa6a,0Xa79,0Xa89,0X98a,0X972,0Xaa8,0Xbcd,0Xc21,0Xeb2,0Xeda,0Xfff,
    0X111,0X312,0X524,0X745,0X968,0Xa6a,0Xa79,0Xa89,0X98a,0X972,0Xaa8,0Xbcd,0Xc21,0Xeb2,0Xeda,0Xfff,
};

const unsigned short tree_palette_grey[32] ={
    0X111,0X333,0X333,0X444,0X666,0X777,0X888,0X888,0X999,0X999,0Xaaa,0Xbcd,0Xc21,0Xeb2,0Xeda,0Xfff,
    0X111,0X333,0X333,0X444,0X666,0X777,0X888,0X888,0X999,0X999,0Xaaa,0Xbcd,0Xc21,0Xeb2,0Xeda,0Xfff,
    };

const unsigned short tree_palette_night[32] ={
    0X111,0X332,0X123,0X134,0X135,0X444,0X134,0X235,0X245,0X422,0X367,0X169,0Xc21,0Xbbf,0Xace,0X58a,
    0X111,0X332,0X123,0X134,0X135,0X444,0X134,0X235,0X245,0X422,0X367,0X169,0Xc21,0Xbbf,0Xace,0X58a,
    };


   const unsigned short tree_palette_night2[32] ={
    0X111,0X332,0X123,0X134,0X135,0X444,0X134,0X235,0X245,0X001,0X102,0X122,0X022,0X233,0X113,0X111,
    0X111,0X332,0X123,0X134,0X135,0X444,0X134,0X235,0X245,0X001,0X102,0X122,0X022,0X233,0X113,0X111,
    };

     const unsigned short tree_palette_green[32] ={
        0X111,0X332,0X245,0X154,0X167,0X566,0X188,0X299,0X5a9,0X972,0Xaa8,0Xbed,0Xc21,0Xeb2,0Xeda,0Xeff,
        0X111,0X332,0X245,0X154,0X167,0X566,0X188,0X299,0X5a9,0X972,0Xaa8,0Xbed,0Xc21,0Xeb2,0Xeda,0Xeff,
        };

const unsigned short blossomCoordinates[] = {
    274,154,
    267,154,
    276,161,
    267,153,
    269,166,
    259,154,
    266,167,
    254,157,
    260,171,
    252,163,
    256,173,
    245,163,
    250,173,
    238,165,
    240,178,
    231,177,
    229,172,
    223,175,
    226,182,
    213,168,
    205,170,
    214,178,
    223,188,
    211,184,
    205,183,
    209,177,
   179,78,
   185,61,
   165,62,
   169,80,
   161,66,
   145,88,
   136,97,
   130,80,
   125,93,
   126,70,
   118,89,
   115,124,
   116,109,
   69,90,
   61,86,
   53,82,
   47,75,
   46,67,
   42,59,
   61,58,
   68,65,
   56,53,
   55,47,
   54,38,
   65,38,
   57,29,
   54,109,
   49,99,
   46,105,
   235,205,
   249,206,
   239,217,
   251,218,
   271,214,
   274,223,
   266,207,
   268,227,
   191,208,
   212,223,
   181,218,
   139,190,
   161,200,
   168,189,
   179,175,
   178,167,
   170,165,
   141,180,
   134,173,
   140,166,
   127,159,
   121,157,
   118,149,
   104,178,
   94,196,
   101,212,
   90,216,
   83,206,
   35,185,
   35,177,
   41,169,
   41,161,
   31,162,
   28,171,
   22,168,
   43,134,
   36,128,
   39,117,
   28,123,
   145,135,
   155,135,
   150,128,
   152,141,
   164,136,
   168,149,
   175,132,
   171,124,
   186,132,
   182,122,
   197,132,
   194,123,
   205,133,
   207,125,
   214,133,
   210,142,
   200,148,
   175,104,
   188,97,
   198,89,
   205,81,
   147,37,
   146,29,
   140,25,
   146,19,
   140,11,
   152,23,
   278,147,
   282,141,
   288,133,
   230,143,
   236,136,
   240,130,
   242,138,







    




};

int blossomMax = sizeof(blossomCoordinates)/2-1;


void tree_updateColors(void){
    if (tree_palette_change==3){
        if (tree_color_step<=200){
            fadePaletteStep(tree_palette,tree_palette_night, tree_color_step/2); 
            tree_color_step++;
        }
    }


    if (tree_palette_change==2){
        if (tree_color_step<=200){
            fadePaletteStep(tree_palette_purple,tree_palette, tree_color_step/2); 
        }
        tree_color_step++;
        if (tree_color_step>250){
            tree_palette_change = 3;
            tree_color_step = 0;
        }
    }

    if (tree_palette_change==1){
        if (tree_color_step<=200){
            fadePaletteStep(tree_palette_grey,tree_palette_purple, tree_color_step/2); 
        }
        tree_color_step++;
        if (tree_color_step>250){
            tree_palette_change = 2;
            tree_color_step = 0;
        }
    }
}

void createRandomBlossom(void){
    int x = random(240) + 20;
    int y = random(150) + 50;

    if(x>200 && y<100){
        x = 20;
        y = 220 ;
    }

    //printf("blossom %d at %d,%d\n",blossomCount,x,y);


    blossoms[blossomCount%10].x = x;
    blossoms[blossomCount%10].y = y;
    blossoms[blossomCount%10].step = 0; 
    blossoms[blossomCount%10].size = 40;   
    blossomCount++; 

    if (blossomCount>300){
            blossomsDone = 1;
        }else{
            //blossomsDone = 1;
            //createRandomBlossom();
        }
}


void createBlossom(void){
    if (blossomIndex<blossomMax-2){
        blossoms[blossomCount%10].x = blossomCoordinates[blossomIndex++];
        blossoms[blossomCount%10].y = blossomCoordinates[blossomIndex++];;
        blossoms[blossomCount%10].step = 0; 
        blossoms[blossomCount%10].size = 30;   
        blossomCount++;
    }else{
        //printf("blossoms done at index %d\n",blossomIndex);
        if (blossomCount>350){
            blossomsDone = 1;
        }else{
            blossomsDone = 1;
            //createRandomBlossom();
        }
    }
}



void updateBlossoms(void){
    int i;
    int x;
    int y;
    int w;

    int max = blossomCount;
    if (max>10) max = 10;

    for (i=0; i<max; i++){
        blossoms[i].step++;
        if (blossoms[i].step<blossoms[i].size+1){
            x = blossoms[i].x;
            y = blossoms[i].y;
            w = blossoms[i].step;
            drawAssetRect(treeAsset, x-w/2, y-w/2, w, w, x-w/2, y - w/2 + 720); 
        }else{
            createBlossom();
        } 
    }

    if (max<10) createBlossom();


    commitScreen();
}


void drawFlake(char index, short x, short y){
    //drawAssetRectMask(iceAsset, x, y, 15, 15, index*16, 0, iceMask);  
}

void drawTreeCross(int x, int y, int width, int height){
     drawAssetRect(treeAsset, x - width/2, y, width, height, x - width/2, y+240); 
     drawAssetRect(treeAsset, x, y - width/2, height, width, x, y-width/2+240); 
}

void drawTreeRect(int x, int y, int size){
     drawAssetRect(treeAsset, x - size/2, y - size/2, size, size, x - size/2, y - size/2 + 240); 
}

void drawTreeLine(int x, int y, int x2, int y2, int size, int speed){
     strokeProgress = 0;
     while (strokeProgress<100){
            x = linear(strokeProgress, x, x2 - x);
            y = linear(strokeProgress, y, y2 - y);

            //drawTreeCross(x, y, 8, 4);
            drawTreeRect(x, y, size);
            strokeProgress += speed;
            
            commitScreen();
            tree_updateColors();
        }
}

// x , y , size , speed
// speed=0 => jump
const unsigned short treeCoordinates[] = {
    8, 228, 10, 0,
    75, 174, 12, 6,
    104,137, 10, 8,
    130,107, 10, 8,
    163,76,10, 4,
    207,54,10, 4,
    99,146,0,0,
    131,136,10, 8,
    216,133,10, 4,
    37,204,0,0,
    109,198, 10, 8,
    141,193,10, 8,
    235,160,10, 8,
    251,159,10, 8,
    71,173,0,0,
    137,174,10, 8,
    200,182,10, 8,
    249,211,10, 8,
    274,217,10, 8,
    249,211,0,0,
    255,228,10, 8,
    67,173,0,0,
    69,127,10, 8,
    70,91,10, 8,
    60,34,10, 8,
    14,223,0,0,
    90,216,10, 8,
    109,221,10, 8,
    29,208,0,0,
    29,182,10, 8,
    40,165,10, 8,
    130,199,0,0,
    158,201,10, 8,
    206,232,14, 8,
    200,182,0,0,
    267,164,14, 8,
    289,131,14, 8,
    66,150,0,0,
    36,125,14, 8,
    72,126,0,0,
    96,113,14, 8,
    97,96,14, 8,
    70,91,14, 8,
    44,59,14, 8,
    70,91,0,0,
    78,49,14, 8,
    9,228,0,0,
    72,232,14, 8,
    135,232,14, 8,
    182,217,14, 8,
    190,226,14, 8,
    226,225,14, 8
}; 

void tree_draw_braches(void){
    short max = 0;
    short lineIndex = 0;

    max = sizeof(treeCoordinates)/2-3;
   

    while (lineIndex<max){
        strokeStartX = strokeEndX;
        strokeStartY = strokeEndY;
        strokeEndX = treeCoordinates[lineIndex++];
        strokeEndY = treeCoordinates[lineIndex++];
        strokeSize = treeCoordinates[lineIndex++];
        strokeSpeed = treeCoordinates[lineIndex++];

        if (strokeSpeed>0){
            drawTreeLine(strokeStartX, strokeStartY, strokeEndX, strokeEndY, strokeSize, strokeSpeed);
        }
    }   
}

void tree_draw_braches_block_line(int x, int y){
     while (x<320 && y>0){
        drawAssetRect(treeAsset, x, y, 40, 40, x, y + 240); 
        commitScreen();   
        x+=40;
        y-=40;

        commitScreen();
   }
}

void tree_draw_braches_blocks(void){
   int x=40;
   int y=200;

   while (x<320 && y>0){
        tree_draw_braches_block_line(0,y);
        tree_draw_braches_block_line(x,200);
        x+=40;
        y-=40;
   }
}

void tree_draw_ice_block_line(int x, int y,int length, int size, BOOL vertical){
    int index = 0;

     while (x>0 && y<(241-size) & index<length){
        drawAssetRect(treeAsset, x, y, size, size, x, y + 480); 
        commitScreen(); 
        tree_updateColors();
        if (!vertical){
            x-=size;
            if (x>0){
                drawAssetRect(treeAsset, x, y, size, size, x, y + 480); 
                commitScreen();   
                tree_updateColors();
            }
        }  
        y+=size;
        index++;

   }
}

void tree_draw_ice(){
    int tick=0;
    int x;
    int y;
    tree_draw_ice_block_line(290,120,3,10,FALSE);
    Delay(10);
    tree_draw_ice_block_line(240,135,2,10,FALSE);
    tree_draw_ice_block_line(205,75,3,10,FALSE);
    tree_draw_ice_block_line(170,100,3,10,FALSE);
    Delay(10);
    tree_draw_ice_block_line(208,39,3,10,FALSE);
    tree_draw_ice_block_line(184,56,3,10,FALSE);
    tree_draw_ice_block_line(158,79,3,10,FALSE);
    tree_draw_ice_block_line(127,106,7,10,FALSE);
    tree_draw_ice_block_line(68,174,5,10,FALSE);
    tree_draw_ice_block_line(22,209,4,10,FALSE);


    tree_draw_ice_block_line(58,34,6,10,TRUE);
    tree_draw_ice_block_line(64,94,8,10,TRUE);


    tree_draw_ice_block_line(190,30,30,10,FALSE);
    tree_draw_ice_block_line(180,20,30,10,FALSE);
    tree_draw_ice_block_line(170,10,30,10,FALSE);
    tree_draw_ice_block_line(160,0,30,10,FALSE);

    tree_draw_ice_block_line(60,20,2,40,TRUE);
    tree_draw_ice_block_line(20,20,2,40,TRUE);

    tree_draw_ice_block_line(160,40,20,40,FALSE);
    drawAssetRect(treeAsset, 0, 200, 40, 40, 0, 200 + 480); 
    commitScreen(); 


    while (tick<100){
        x = random(280) + 10;
        y = random(150) + 80;
        drawAssetRect(treeAsset, x, y, 10, 10, x, y + 480); 
        x = random(280) + 10;
        y = random(150) + 80;
        drawAssetRect(treeAsset, x, y, 10, 10, x, y + 480); 
        commitScreen(); 
        tree_updateColors();
        tick++;
    }


    //tree_draw_ice_block_line(200,80,20,10,FALSE);
    //tree_draw_ice_block_line(240,120,20,40,FALSE);
    //tree_draw_ice_block_line(280,160,20,40,FALSE);

    Delay(100);


    //Delay(1000);
}

void tree_scanline(int y, int step){
    while (y<240){
        drawAssetRect(treeAsset, 0, y, 320, 1, 0, y + 240); 
        commitScreen(); 
        y += step;
    }
}

void tree_appear_rect(int x, int y, int width, int height, char speed){
    int h=1;
    int w=1;
    int steps = height;
    if (height<0) steps = height * -1;

    while (h<=steps){
        drawAssetRect(treeAsset, x-w/2, y-h/2, w, h, x-w/2, y - h/2 + 720); 
        commitScreen(); 
        h += speed;
        if (w<width) w += speed;
    }

}

void tree_ping(int x, int y){
    int size = 5;
    int color = 13;

    while (size<26){

        drawAssetRect(treeAsset, 244, 119, 70, 70, 244, 720);     
        drawCirc(x, y, size, color);
        if (color>0 && size%2==0) color--;

        commitScreen(); 
        size += 1;
    }
    drawAssetRect(treeAsset, 244, 119, 70, 70, 244, 720);     

}

void tree_wait_and_updateColors(int ticks){
    int i;
    for (i=0; i<ticks; i++){
        tree_updateColors();
    }
}

char winter_preLoad(void){
   treeAsset = loadBitPlanes("data/tree.planes", 4, 320, 960);
   //treeAsset = loadFile("data/tree.planes", 1);
   return treeAsset;
}

void winter(void){
    int i;
    int ticks = 0;
    int flakeIndex;
    BOOL done = 0;
    short x;
    short y;

    clearScreen();
    setPaletteToBlack(tree_palette_grey);
    commitScreen(); 

    //printf("blossomMax: %d\n",blossomMax);
   

    if (treeAsset<0){
        printf("tree asset not loaded\n");
        print_free_memory();
        return;
    }

    //createBitMap(treeAsset, 4, 320, 960);
    WaitTOF();

    // background
    drawAssetRect(treeAsset, 0, 0, 320, 240, 0, 0);
    commitScreen(); 
    fadeFromBlack(tree_palette_grey,50);
   

    // branches
    tree_palette_change = 1;
    tree_draw_braches();
    tree_draw_braches_blocks();

    tree_wait_and_updateColors(100);



    //ice
    tree_draw_ice();

    fadePalette(tree_palette_night, tree_palette_night2, 100);

    tree_scanline(0,2);
    tree_scanline(1,2);

    

    //draw full tree
    drawAssetRect(treeAsset, 0, 0, 320, 240, 0, 240);
    commitScreen();
    fadePalette(tree_palette_night2, tree_palette_green, 200);

    tree_appear_rect(286,150,9,14,1);
    tree_appear_rect(286,164,9,14,1);


    tree_ping(286,150);
    Delay(20);

    createBlossom();

    while (!blossomsDone){
        updateBlossoms();
    }
     //10 more iterations to complete the blossoms
    for (i=0; i<10; i++){
        //updateBlossoms();
    }

   
    commitScreen();
    Delay(10);


    /*while (!done){
        if (strokeProgress<100){
            x = linear(strokeProgress, strokeStartX, strokeEndX - strokeStartX);
            y = linear(strokeProgress, strokeStartY, strokeEndY - strokeStartY);

            //drawTreeCross(x, y, 8, 4);
            drawTreeRect(x, y, strokeSize);
            strokeProgress += strokeSpeed;
            
            commitScreen();
        }

        ticks++;
        if (ticks > 100) done = 1;

    }*/



    //iceAsset = loadBitPlanes("data/ice.planes", 4, 80, 80);
    //iceMask = loadFile("data/ice.mask");

    /*while(ticks < 100){
        flakeIndex = random(3);
        drawFlake(flakeIndex,random(300),random(200));

        ticks++;
        commitScreen();    
       
    }*/

    Delay(50);

    // TODO: crash on 500?
    //asset_free(treeAsset);
}