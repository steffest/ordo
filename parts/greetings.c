
#include "proto.h"

char greetAsset;

const unsigned short greetings_palette[32] = {
       0X000,0X100,0X011,0X110,0X011,0X111,0X111,0X111,0X011,0X111,0X122,0X022,0X222,0X122,0X133,0X233,0X144,0X244,0X453,0X255,0X166,0X465,0X366,0X763,0X577,0X387,0X875,0X698,0Xa96,0X9a9,0Xca6,0Xec8
    };

void greetings_preLoad(void){
    greetAsset = loadBitPlanes("data/greetings.planes", 5, 228, 200);
    return greetAsset;
}

void greetings_appear_lines(char offset){
   int step = 0;
   int max = 34;
   int left = 1;
   int blockWidth = 69;
   int x;
   int sourceX;
   int gap = 3;

   while (step<max){
        x = 160-20 -step*2;
        sourceX = blockWidth-step*2-offset-1;
        if (sourceX<0) sourceX = 0;

        drawAssetRect(greetAsset, x-offset , 20, 2, 200, sourceX, 0); 
        commitScreen(); 
    
        x = 160+20+step*2;
        sourceX = blockWidth+step*2+offset+2;
        drawAssetRect(greetAsset, x+offset , 20, 2, 200, sourceX, 0); 
        commitScreen(); 

        step += 2; 
        left = 1-left;
   }
   drawAssetRect(greetAsset, 72 , 20, 2, 200, 0, 0); 
   commitScreen(); 

}

void greetings_appear(){
   greetings_appear_lines(0);
   greetings_appear_lines(2);
}

void greetings_disappear(){
    int y;
    int offset;
    for (offset = 0; offset<2; offset++){
        y=20+offset;
        while (y<=220){
            drawAssetRect(greetAsset,72, y, 70, 1, 140, 142); 
            commitScreen(); 
            y+=2;
        }
    }
}

void greetings_title(){
    int x = 0;
    while (x<66){
        drawAssetRect(greetAsset,74 + x, 20, 2, 34, 142+x, 0); 
        commitScreen(); 
        x+=2;
    }
}

void greetings_dropLines(int fromY, int toY){
    int y = fromY;
    int offset;
    for (offset = 0; offset<2; offset++){
        y=fromY+offset;
        while (y<=toY){
            drawAssetRect(greetAsset,184, 20 + y, 68, 1, 140, 142); 
            commitScreen(); 
            y+=2;
        }
    }
}

void greetings_dropName(short fromY, short height, short sourceY){
    int step;
    int offset;
    for (offset = 0; offset<2; offset++){
        step = offset;
        while (step<=height){
            drawAssetRect(greetAsset,184, fromY+step, 68, 1, 140, sourceY + step); 
            commitScreen(); 
            step+=2;
        }
    }
}

void greetings_names(){

    greetings_dropLines(0,66);


    greetings_dropName(20 + 66,20,103);
    Delay(10);

    // RBBS
    greetings_dropName(20 + 66,20,103);
    Delay(15);

    // Desire
    greetings_dropLines(86,96);
    greetings_dropName(20 + 96,20,122);
    Delay(25);

    // BAC
    greetings_dropLines(115,126);
    greetings_dropName(20 + 126,20,142);
    Delay(25);

    // Dusk Wave
    greetings_dropLines(146,154);
    greetings_dropName(20 + 154,36,163);
    Delay(25); 

    greetings_dropLines(190,199);
    Delay(15);  
}

void credits_line(){
    int w = 2;
    int y=0;

    while (y<=160-w){
        drawAssetRect(greetAsset, 160+y, 60, w, 1, 140,98);  
        drawAssetRect(greetAsset, 160-y-w, 60, w, 1, 140,98); 
        
        drawAssetRect(greetAsset, 160+y, 60+16, w, 1, 140,99);  
        drawAssetRect(greetAsset, 160-y-w, 60+16, w, 1, 140,99);  
        commitScreen(); 
        y+=w;
    }
}

void credits_text(int x, int y, int w, int sourceX, int sourceY){
    int step = 0;
    while (step<=w){
        drawAssetRect(greetAsset, x + step, y, 1, 5, sourceX+step, sourceY);  
        commitScreen(); 
        step++;
    
    }

}

void credits_steffest(){
    int x = 0;
    while (x<126){
        drawAssetRect(greetAsset, x, 137, 2, 2, 140, 98); 
        commitScreen(); 
        x+=2;
    }
    x=0;
    while (x<68){
        drawAssetRect(greetAsset, 126 + x, 120, 1, 20, 140+x,81);  
        commitScreen(); 
        x++;
    
    }
    x=194;
    while (x<319){
        drawAssetRect(greetAsset, x, 120, 2, 2, 165, 81); 
        commitScreen(); 
        x+=2;
    }
}

void credits(){

    setPaletteToBlack();

    drawAssetRect(greetAsset, 160-41, 40, 82, 5, 140,39);  
    commitScreen();    


    fadeFromBlack(greetings_palette,100);

    credits_line();
    //drawRect(0, 60, 320, 1, 20);
    //drawRect(0, 60+17, 320, 1, 20);

    commitScreen();    
    Delay(40);
    // ordo
    drawAssetRect(greetAsset, 160-(34/2), 62, 8, 14, 145,66); 
    commitScreen();    
    Delay(20);
    drawAssetRect(greetAsset, 160-(34/2)+8, 62, 9, 14, 145+8,66);
    commitScreen();    
    Delay(20); 
    drawAssetRect(greetAsset, 160-(34/2)+17, 62, 8, 14, 145+17,66);
    commitScreen();    
    Delay(20); 
    drawAssetRect(greetAsset, 160-(34/2)+25, 62, 9, 14, 145+25,66);
    commitScreen();    
    Delay(20); 
   

    // credits
    credits_text(160-56, 100, 70, 141,52);
    credits_text(160-56+74, 100, 50, 141,58);
    

    credits_steffest();
    //steffest
    //drawAssetRect(greetAsset, 160-34, 120, 68, 20, 140,81);  

    Delay(40); 

    // thanks
    credits_text(126, 160, 40, 141,46);
    credits_text(126 + 42, 160, 33, 189,39);

    Delay(30); 

    // peace
    drawAssetRect(greetAsset, 160-6, 190, 13, 18, 213,61);  
    commitScreen(); 

    Delay(300);

   
}

void greetings(void){

    clearScreen();
    setPalette(greetings_palette);

    greetings_appear();
    greetings_title();
    greetings_names();
    greetings_disappear();

    fadeToBlack(greetings_palette,100);
    clearScreen();
    commitScreen(); 

    Delay(50);


    credits();
    fadeToBlack(greetings_palette,100);
    clearScreen();
    commitScreen(); 
    
    Delay(50);
 
    asset_free(greetAsset);


}