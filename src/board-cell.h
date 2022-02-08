#include <stdio.h>
#include <stdlib.h>

#ifndef BOARD_CELL
#define BOARD_CELL

#include "cell.h"
#include "dir.h"

void boardCellInit();
void boardCellOffset(cell **pc, int di, int dj);
void boardCellVisitedReset();
void boardCellIndexReset();
cell *dir2EmptyCell(cell *c, dir d, char preferDir);
cell *dir2cell(cell *c, dir d);
dir boardCellDirToOppositeSide(cell *c);
void boardCellColorReset();

enum prefered_dir {LEFT_FIRST, RIGHT_FIRST};

#endif
