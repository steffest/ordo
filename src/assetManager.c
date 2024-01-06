#include "proto.h"
#include <stdio.h>
#include <exec/memory.h>
#include <exec/types.h>
#include <proto/graphics.h>
#include <intuition/intuition.h>

static struct Asset {
    unsigned char type;
    char memoryType;
    char depth;
    short width;
    short height;
    struct RastPort ctx;
    struct BitMap bitmap;
    UBYTE *data;
    short size;
    char planePick;
};

static struct Asset assets[10];
char assetCount = -1;
static struct RastPort ctx;
struct BitMap currentScreenBitmap;

void setup_assets(void){
   
}

void set_asset_screen(void){
    ctx = getScreenContext();
    currentScreenBitmap = *ctx.BitMap;
}

char loadBitPlanes(char *filename, char depth, int width, int height){
    // Load a bitmap from the filesystem into chipmemory;

    int i;
    UBYTE *chipMemory;
    FILE *fileHandle;
    ULONG fileSize;
    struct Image image;

    assetCount++;
    assets[assetCount].type = 2;
    assets[assetCount].depth = depth;
    assets[assetCount].width = width;
    assets[assetCount].height = height;
    assets[assetCount].planePick = 0x1F;
    if (depth==1) assets[assetCount].planePick = 0x01;
    if (depth==2) assets[assetCount].planePick = 0x03;
    if (depth==3) assets[assetCount].planePick = 0x07;
    if (depth==4) assets[assetCount].planePick = 0x0F;

    fileHandle = fopen(filename, "rb");
    if (fileHandle == NULL) {
        printf("Failed to open file\n");
        return -1;
    }

    // Get the file size
    fseek(fileHandle,0,2);
    fileSize=ftell(fileHandle);
    fseek(fileHandle,0,2);
    //printf("file is %d bytes, loading into slot %d \n",fileSize,assetCount);
    rewind(fileHandle);

   
    // Allocate chip memory 
    // note - this doesn't have to be chip as this is just a temporary buffer ?
    // note 2 - apparently it does have to be chip on OS 1.3?
    chipMemory = AllocMem(fileSize, MEMF_CHIP);
    if (chipMemory == NULL) {
        printf("Failed to allocate chip memory\n");
        fclose(fileHandle);
        assets[assetCount].type = 0;
        return -1;
    }

    // Read the file into memory
    if (fread(chipMemory, sizeof(UBYTE), fileSize, fileHandle) != fileSize) {
        printf("Failed to read file\n");
        FreeMem(chipMemory, fileSize);
        fclose(fileHandle);
        assets[assetCount].type = 0;
        return -1;
    }
    fclose(fileHandle);
 
 
    image.Depth = depth;
    image.Width = width;
    image.Height = height;
    image.ImageData = chipMemory;
    image.LeftEdge = 0;
    image.TopEdge = 0;
    image.PlanePick = 0x1F;
    image.PlaneOnOff = 0;
    image.NextImage = NULL;

    
    InitRastPort(&(assets[assetCount].ctx));
    assets[assetCount].ctx.BitMap = &(assets[assetCount].bitmap);
    InitBitMap(&(assets[assetCount].bitmap), depth, width, height);
    for(i = 0; i < depth; i++) {
        assets[assetCount].bitmap.Planes[i] = AllocRaster(width,height);
        if (assets[assetCount].bitmap.Planes[i] == 0) {
            printf("failed to allocate sprite bitmap\n");
            assets[assetCount].type = 0;
            FreeMem(chipMemory, fileSize);
            return -1;
        }
    }
    DrawImage(&(assets[assetCount].ctx),&image,0,0);

    // Free the chip memory
    FreeMem(chipMemory, fileSize);

    return assetCount;

}

char loadFile(char *filename, char memoryType){
    // Load a file from the filesystem into

    int i;
    UBYTE *chipMemory;
    FILE *fileHandle;
    ULONG fileSize;

    int MEMTYPE = MEMF_ANY;
    if (memoryType==1) MEMTYPE = MEMF_CHIP;
    if (memoryType==2) MEMTYPE = MEMF_FAST;
 
    assetCount++;
    assets[assetCount].type = 1;
    assets[assetCount].memoryType = memoryType;
  
    fileHandle = fopen(filename, "rb");
    if (fileHandle == NULL) {
        printf("Failed to open file\n");
        return -1;
    }

    // Get the file size
    fseek(fileHandle,0,2);
    fileSize=ftell(fileHandle);
    if (WRITE_DEBUG) printf("file is %d bytes\n",fileSize);
    rewind(fileHandle);

   
    // Allocate chip memory
    chipMemory = AllocMem(fileSize, MEMTYPE);
    if (chipMemory == NULL) {
        printf("Failed to allocate chip memory\n");
        fclose(fileHandle);
        assets[assetCount].type = 0;
        return -1;
    }

    // Read the file into chip memory
    if (fread(chipMemory, sizeof(UBYTE), fileSize, fileHandle) != fileSize) {
        printf("Failed to read file\n");
        FreeMem(chipMemory, fileSize);
        assets[assetCount].type = 0;
        fclose(fileHandle);
        return -1;
    }
    fclose(fileHandle);
 
 
    assets[assetCount].data = chipMemory;
    assets[assetCount].size = fileSize;

    return assetCount;

}

char createBitMap(char index, char depth, short width, short height){
    int i;
    UBYTE *chipMemory;
    char memoryType;
    struct Image image;
    char planePick;

    assets[index].depth = depth;
    assets[index].width = width;
    assets[index].height = height;

    memoryType = assets[index].memoryType;

    planePick = 0x1F;
    if (depth==1) planePick = 0x01;
    if (depth==2) planePick = 0x03;
    if (depth==3) planePick = 0x07;
    if (depth==4) planePick = 0x0F;


    image.Depth = depth;
    image.Width = width;
    image.Height = height;
    
    image.LeftEdge = 0;
    image.TopEdge = 0;
    image.PlanePick = planePick;
    image.PlaneOnOff = 0;
    image.NextImage = NULL;



    if (assets[index].memoryType==2){
        printf("fast memory\n");
        // copy to chip?
        chipMemory = AllocMem(assets[index].size, MEMF_CHIP);
        if (chipMemory == NULL) {
            printf("Failed to allocate chip memory\n");
            return 1;
        }

        CopyMem(assets[index].data, chipMemory, assets[index].size);
        image.ImageData = chipMemory;
    

    }else{
        image.ImageData = assets[index].data;
    }

    

    InitRastPort(&(assets[index].ctx));
    assets[index].ctx.BitMap = &(assets[index].bitmap);
    InitBitMap(&(assets[index].bitmap), depth, width, height);
    for(i = 0; i < depth; i++) {
        assets[index].bitmap.Planes[i] = AllocRaster(width,height);
        if (assets[index].bitmap.Planes[i] == 0) {
            printf("failed to allocate sprite bitmap\n");
            return -1;
        }
    }

    DrawImage(&(assets[index].ctx),&image,0,0);
    FreeMem(assets[index].data, assets[index].size);
    if (assets[index].memoryType==2){
        FreeMem(chipMemory, assets[index].size);
    }
    assets[index].type = 2;

    return index;
}

struct RastPort getAssetContext(char index){
    return assets[index].ctx;
}

UBYTE *getAssetData(char index){
    return assets[index].data;
}

void drawAsset(char index, short x, short y){
    /*BltBitMap(assets[index].bitmap], 
        x,y,
        &screenBitmap,
        0,0,
        320,180,
        0xC0,0x1F,NULL );   */ 
}


void drawAssetRect(char index, short x, short y, short width, short height, short sourceX, short sourceY){
    BltBitMap(&assets[index].bitmap, 
        sourceX,sourceY,
        &currentScreenBitmap,
        x,y,
        width,height,
        0xC0,assets[index].planePick,NULL );  
}

void drawAssetRectPlane(char index, short x, short y, short width, short height, short sourceX, short sourceY, char plane){
    BltBitMap(&assets[index].bitmap, 
        sourceX,sourceY,
        &currentScreenBitmap,
        x,y,
        width,height,
        0xC0,plane,NULL );  
}

void drawAssetRectBlend(char index, short x, short y, short width, short height, short sourceX, short sourceY, int blendMode){
    BltBitMap(&assets[index].bitmap, 
        sourceX,sourceY,
        &currentScreenBitmap,
        x,y,
        width,height,
        blendMode,assets[index].planePick,NULL );  
}

void drawAssetRectMask(char index, short x, short y, short width, short height, short sourceX, short sourceY, char maskIndex){
    UBYTE *mask = assets[maskIndex].data;
    
    printf("drawing mask %d\n",maskIndex);

    
    BltMaskBitMapRastPort(&assets[index].bitmap, 
        sourceX,sourceY,
        &ctx,
        x,y,
        width,height,
        ABC|ABNC|ANBC,mask);  
}

void asset_free(char index){
    if (assets[index].type>0){
        int i;
        if (WRITE_DEBUG) printf("freeing asset %d of type %d\n",index,assets[index].type);

        if (assets[index].type==1){
            FreeMem(assets[index].data, assets[index].size);
        }

        if (assets[index].type==2){
            //printf("height %d\n", assets[index].height);
            for(i = 0; i < assets[index].depth; i++) {
                FreeRaster(assets[index].bitmap.Planes[i], assets[index].width, assets[index].height);
            };
        }
        
        assets[index].type = 0;
    }
}

void free_assets(void){
    int i;
    for (i=0; i<=assetCount; i++){
        asset_free(i);
    }
    
}