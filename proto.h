#define SCREENWIDTH 320
#define SCREENHEIGHT 240
#define WRITE_DEBUG 0

extern struct BitMap screenBitmap;
extern struct RastPort screenContext;

#include "src/screen.h"
#include "src/assetManager.h"
#include "src/timer.h"
#include "src/graphics.h"
#include "src/easing.h"
#include "src/util.h"
#include "src/modPlay.h"
#include "parts/bears.h"