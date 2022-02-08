#include <stdio.h>
#include <stdlib.h>

#ifndef BOARD_WALL
#define BOARD_WALL

#include "game.h"
#include "wall.h"

#define BOARD_WALLS_N_MAX ((BOARD_M + 1) * (BOARD_N + 1))

wall *boardWallNew();
void boardWallInit();
void boardWallShapeApply(wall *w);
void boardWallJointDetect(wall *w, int lastWallIndex);
void boardWallCellDetect(wall *w);
char boardWallIntersectionQ(wall *w1, wall *w2);
void boardWallCellDetectUndo();
void boardWallNodePlace(wall *w);

#endif
