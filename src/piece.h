#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>

#ifndef PIECE
#define PIECE

#include "shape.h"
#include "cell.h"
#include "wall.h"

#define PIECE_ACTIVE_SCALE 1.2
#define PIECE_LANDING_CELLS_N_MAX 2
#define PIECE_TEX_OUTLINE_INDEX 0

typedef
struct piece_ {
    shape *shape;
    cell *cell;
    cell *landingCells[PIECE_LANDING_CELLS_N_MAX];
    int landingCellsN;
    dir landingDir;
    wall *borderWall;
    char active;
    char enabled;
    char selected;
    char landing;
    int stepCount;
    dir targetDir;
} piece;

piece *pieceNewArray(size_t n);
void pieceDestroyArray(piece *pieces, size_t n);
void pieceColorRandDist(piece *p);
void piecePrint(piece *p);
void pieceLandingOn(piece *p);
void pieceLandingOff(piece *p);

#endif 
