#include <stdio.h>
#include <stdlib.h>

#ifndef BOARD_WALL_BORDERWALL
#define BOARD_WALL_BORDERWALL

#include "wall.h"
#include "piece.h"

#define BOARD_BORDER_WALLS_N_MAX 4

void boardWallCreateBorderWall();
char boardWallPositionBorderQ(wall *w);
void boardWallBorderWallColorFromPiece(piece *p);

#endif
