#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>

#ifndef BOARD
#define BOARD

#include "game.h"
#include "cell.h"
#include "piece.h"
#include "wall.h"
#include "node.h"
#include "path.h"
#include "shape.h"
#include "point.h"
#include "board-wall.h"
#include "board-wall-float.h"
#include "board-wall-borderwall.h"
#include "board-path.h"
#include "dir.h"
#include "texture.h"

struct board_ {
    struct board_path_ *path;

    int n, m, nm; 
    double x, y;
    double w, h;
    double spaceFactorX, spaceFactorY;
    double cellW, cellH;
    double spaceX, spaceY;
    double stepX, stepY;
    double boardLUX, boardLUY;
    double cellLUX, cellLUY;
    cell *cells;
    piece *pieces;
    int piecesN;
    int piecesSelectedN;
    piece *pieceActive;
    char (*pieceMove)(dir d); 
    wall *theWall;
    wall *walls[BOARD_WALLS_N_MAX];
    wall *borderWalls[BOARD_BORDER_WALLS_N_MAX];
    int wallsN;
    wall *wallFloat;
    int borderWallsN;
    int wallFloatLengthW;
    int wallFloatLengthH;
    path lastWallPath;
    texture *wallTexture;
    texture *nodeTexture;
    node *nodes[(BOARD_N + 1) * (BOARD_M + 1)];
    int nodesN;
};

void boardInit();
void drawBoard();
void ij2xy(int i, int j, double *px, double *py, int mode);
char xy2ij(double x, double y, int *pi, int *pj, int mode);
void node2pivot(node ps, pivot *pvt);
void cell2pivot(cell *c, pivot *pvt);
char point2cell(point pnt, cell **pc);
char point2node(point pnt, node *nd);
char point2piece(point pnt, piece **pp);
char boardPositionInsideQ(int i, int j);

enum ij_mode {IJ_CELL_MODE, IJ_NODE_MODE};

extern struct board_ board;

#endif
