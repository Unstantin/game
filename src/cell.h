#include <stdlib.h>
#include <GL/glut.h>

#ifndef CELL
#define CELL

#include "shape.h"
#include "dir.h"
#include "wall.h"

#define CELL_PIECE_SCALE_FACTOR 0.75
#define CELL_TEX_LANDING_SITE_INDEX 0

typedef struct piece_ piece;

typedef
struct cell_ {
    int i, j;
    shape *shape;
    piece *piece;
    wall *walls[DIR_N];
    char visited;
    char index;
    char lastWallDirname;
    char landingSiteVisible;
} cell;

cell *cellNewArray(size_t n);
void cellDestroyArray(cell *cells, size_t n);
void cellDraw(shape *sh);
void cellPrint(cell *c);

#endif
