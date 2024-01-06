#include <exec/types.h>

BOOL openScreen(void);
void closeScreen(void);
void clearScreen(void);
void drawRect(int x, int y, int w, int h, int color);
void drawCirc(int x, int y, int r, int color);
void drawPoly(int color);
void setColor(int color, UBYTE r, UBYTE g, UBYTE b);
void setPalette(const unsigned short *palette);
void scrollScreen(int x, int y);
void commitScreen(void);
void waitVbl(void);
void hideMouse(void);
struct BitMap getScreenBitMap(void);
struct RastPort getScreenContext(void);