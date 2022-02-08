#include <stdio.h>
#include <stdlib.h>

#ifndef BOARD_WALL_FLOAT
#define BOARD_WALL_FLOAT

#define BOARD_WALL_FLOAT_LENGTH_DELTA 1

void boardWallFloatSceneOpen();
void boardWallFloatSceneClose();
char boardWallFloatPlace();
void boardWallFloatMouseMove();
void boardWallFloatTranspose();
void boardWallFloatResize(int delta);

#endif
