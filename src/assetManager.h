void setup_assets(void);
void set_asset_screen(void);
void free_assets(void);
char loadBitPlanes(char *filename, char depth, int width, int height);
char loadFile(char *filename, char memoryType);
struct RastPort getAssetContext(char index);
UBYTE *getAssetData(char index);
void drawAssetRect(char index, short x, short y, short width, short height, short sourceX, short sourceY);
void drawAssetRectMask(char index, short x, short y, short width, short height, short sourceX, short sourceY, char maskIndex);