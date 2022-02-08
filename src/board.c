#include "board.h"
#include "board-piece.h"
#include "board-cell.h"
#include "board-node.h"
#include "polygon.h"

#define BOARD_X 0.
#define BOARD_Y 0.
#define BOARD_H 1.75
#define BOARD_W 1.75
#define BOARD_SPACEFACTOR_X 0.15
#define BOARD_SPACEFACTOR_Y 0.15
#define BOARD_CELL_W_0 1.
#define BOARD_CELL_H_0 1.

#define BOARD_N_MAX 100
#define BOARD_M_MAX 100

#define IJ_NODE_TOLERANCE_FACTOR 1.3

#define BOARD_WALL_FLOAT_LENGTH_H_DEFAULT 3
#define BOARD_WALL_FLOAT_LENGTH_W_DEFAULT 0

void boardLayoutInit();

struct board_ board = {
    .n = BOARD_N, 
    .m = BOARD_M,
    .nm = BOARD_N * BOARD_M,
    .x = BOARD_X,
    .y = BOARD_Y,
    .w = BOARD_W,
    .h = BOARD_H,
    .spaceFactorX = BOARD_SPACEFACTOR_X,
    .spaceFactorY = BOARD_SPACEFACTOR_Y,
    .stepX = 0,
    .stepY = 0,
    .cells = NULL,
    .pieces = NULL,
    .piecesN = BOARD_PIECES_N,
    .piecesSelectedN = 0,
    .pieceActive = NULL,
    .pieceMove = NULL,
    .wallsN = 0,
    .wallFloat = NULL,
    .theWall = NULL,
    .borderWallsN = 0,
    .wallFloatLengthW = BOARD_WALL_FLOAT_LENGTH_W_DEFAULT,
    .wallFloatLengthH = BOARD_WALL_FLOAT_LENGTH_H_DEFAULT,
    .lastWallPath = {0},
    .wallTexture = NULL,
    .nodeTexture = NULL,
    .nodes = NULL,
    .nodesN = (BOARD_N + 1) * (BOARD_M + 1)
};

void boardInit() {
    board.path = &board_path; 

    boardLayoutInit();
    boardCellInit();
    boardPieceInit();
    boardWallInit();
    boardNodeInit();
}

void boardLayoutInit() {
    float cellW0 = BOARD_CELL_W_0;
    float cellH0 = BOARD_CELL_H_0;

    board.cellW = board.w / (board.m + board.spaceFactorX * (board.m + 1));
    board.cellH = board.h / (board.n + board.spaceFactorY * (board.n + 1));

    float aspect = board.cellH / board.cellW;
    float aspect0 = cellH0 / cellW0;

    if(aspect > aspect0) {
        board.cellH = board.cellW * aspect0;
        board.h = board.cellH * (board.n + board.spaceFactorY * (board.n + 1));
    } else {
        board.cellW = board.cellH / aspect0;
        board.w = board.cellW * (board.m + board.spaceFactorX * (board.m + 1));
    }

    board.spaceX = board.cellW * board.spaceFactorX;
    board.spaceY = board.cellH * board.spaceFactorY;

    board.stepX = board.cellW + board.spaceX;
    board.stepY = -(board.cellH + board.spaceY);

    board.boardLUX = board.x - board.w / 2;
    board.boardLUY = board.y + board.h / 2;
    board.cellLUX = board.boardLUX + board.cellW / 2 + board.spaceX;
    board.cellLUY = board.boardLUY - board.cellH / 2 - board.spaceY;
}

/***********************************************/

void ij2xy(int i, int j, double *px, double *py, int mode) {
    if(mode == IJ_CELL_MODE) {
        *px = board.cellLUX;
        *py = board.cellLUY;
    } 
    else if(mode == IJ_NODE_MODE) {
        *px = board.boardLUX + board.spaceX / 2;
        *py = board.boardLUY - board.spaceY / 2;
    }

    *px += board.stepX * j;
    *py += board.stepY * i;
}

char xy2ij(double x, double y, int *pi, int *pj, int mode) {
    double x1 = 0;
    double y1 = 0;

    if(mode == IJ_CELL_MODE) {
        x1 = x - board.boardLUX; 
        y1 = y - board.boardLUY;
    }
    else if(mode == IJ_NODE_MODE) {
        x1 = x - board.cellLUX + board.stepX; 
        y1 = y - board.cellLUY + board.stepY;
    }

    if(x1 < 0) return 0;
    if(y1 > 0) return 0;

    int j = x1 / board.stepX;
    int i = y1 / board.stepY;

    if(mode == IJ_CELL_MODE) {
        if(i >= board.n) return 0;
        if(j >= board.m) return 0;
    }
    else if(mode == IJ_NODE_MODE) {
        if(i > board.n) return 0;
        if(j > board.m) return 0;
    }

    double x1_ = j * board.stepX;
    double y1_ = i * board.stepY;
    
    double dx = x1 - x1_;
    double dy = y1 - y1_;

    if(mode == IJ_CELL_MODE) {
        if(dx < board.spaceX) return 0;
        if(-dy < board.spaceY) return 0;
    }
    else if(mode == IJ_NODE_MODE) {
        double dxc = dx - board.stepX / 2;
        double dyc = dy - board.stepY / 2;

        double toleranceX = IJ_NODE_TOLERANCE_FACTOR * board.spaceX;
        double toleranceY = IJ_NODE_TOLERANCE_FACTOR * board.spaceY;

        if(dxc < -toleranceX || dxc > toleranceY) return 0;
        if(-dyc < -toleranceY || -dyc > toleranceY) return 0;
    }

    *pi = i;
    *pj = j;

    return 1;
}

void cell2pivot(cell *c, pivot *pvt) {
    ij2xy(c->i, c->j, &pvt->x, &pvt->y, IJ_CELL_MODE);
}

void node2pivot(node nd, pivot *pvt) {
    ij2xy(nd.i, nd.j, &pvt->x, &pvt->y, IJ_NODE_MODE); 
}

char point2cell(point pnt, cell **pc) {
    int i, j;

    char hit = xy2ij(pnt.x, pnt.y, &i, &j, IJ_CELL_MODE);

    *pc = hit ? &board.cells[i * board.m + j] : NULL;

    return hit;
}

char point2node(point pnt, node *nd) {
    int i, j;

    char hit = xy2ij(pnt.x, pnt.y, &nd->i, &nd->j, IJ_NODE_MODE);

    return hit;
}

char point2piece(point pnt, piece **pp) {
    for(int k = 0; k < board.piecesN; k++) {
        piece *p = &board.pieces[k];
        if(!p->enabled) continue;

        char hit = boardPieceHitQ(p, pnt);

        if(hit) { 
            *pp = p;
            return 1;
        } 
    }

    return 0;
}

char boardPositionInsideQ(int i, int j) {
    return i >= 0 && i < board.n && j >= 0 && j < board.m;
}

void drawBoard() {
    for(int k = 0; k < board.nm; k++) {
        shape *sh = board.cells[k].shape;
        sh->draw(sh);
    }

    for(int k = 0; k < board.piecesN; k++) {
        piece *p = &board.pieces[k];

        if(!p->enabled) continue;

        shape *sh = p->shape;
        sh->draw(sh);
    }

    if(board.theWall) {
        shape *sh = board.theWall->shape; 
        sh->draw(sh); 
    }

    if(board.wallFloat) {
        shape *sh = board.wallFloat->shape;
        sh->draw(sh);
    }

    for(int k = 0; k < board.wallsN; k++) {
        shape *sh = board.walls[k]->shape;
        sh->draw(sh);
    } 
    
    for(int k = 0; k < board.borderWallsN; k++) {
        shape *sh = board.borderWalls[k]->shape;
        sh->draw(sh);
    }

    for(int k = 0; k < board.nodesN; k++) {
        node *nd = board.nodes[k];
        if(!nd) continue;
        if(!nd->enabled) continue;

        shape *sh = nd->shape;
        sh->draw(sh);
    }
}



