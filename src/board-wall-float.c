#include "board-wall-float.h"
#include "board-wall.h"
#include "wall.h"
#include "board.h"
#include "mouse.h"
#include "shape.h"
#include "node.h"
#include "color.h"
#include "board-path.h"

#define BOARD_WALL_FLOAT_LENGTH_H_DEFAULT 3
#define BOARD_WALL_FLOAT_LENGTH_W_DEFAULT 0

void boardWallFloatCreate();
void boardWallFloatDestroy();
char boardWallFloatSnapToBoard();
char boardWallFloatIntersectionQ();
char boardWallFloatPlaceableQ();

void boardWallFloatCreate() {
    wall *w = boardWallNew();
    board.wallFloat = w;

    w->lengthW = board.wallFloatLengthW;
    w->lengthH = board.wallFloatLengthH;

    w->shape->color = colors[COLOR_WALL_FLOAT_NON_PLACEABLE];

    boardWallShapeApply(w);
}

void boardWallFloatDestroy() {
    if(!board.wallFloat) return;

    wallDestroy(board.wallFloat);
    board.wallFloat = NULL;
}

void boardWallFloatMouseMove() {
//    printf("M");
//    fflush(stdout);
    wall *w = board.wallFloat;
    if(!w) return;

    shape *sh = w->shape;

    sh->pivot = mouseCoords;

    char placeable = boardWallFloatPlaceableQ();

    if(placeable) 
        sh->color = colors[COLOR_WALL_FLOAT_PLACEABLE];
    else
        sh->color = colors[COLOR_WALL_FLOAT_NON_PLACEABLE];
}

void boardWallFloatSceneOpen() {
    boardWallFloatCreate();

    boardWallFloatMouseMove();
}

void boardWallFloatSceneClose() {
    boardWallFloatDestroy();
}

char boardWallFloatSnapToBoard() {
    wall *w = board.wallFloat;
    shape *sh = w->shape;

    point pntBegin = sh->pivot;
    point pntEnd = sh->pivot;

    if(w->lengthW) {
        pntBegin.x = sh->pivot.x - sh->dim.w / 2;
        pntEnd.x = sh->pivot.x + sh->dim.w / 2;
    } 
    else if(w->lengthH) {
        pntBegin.y = sh->pivot.y - sh->dim.h / 2;
        pntEnd.y = sh->pivot.y + sh->dim.h / 2;
    }
    
    node ndBegin, ndEnd;

    char hitBegin = point2node(pntBegin, &ndBegin);
    char hitEnd = point2node(pntEnd, &ndEnd);

    if(!hitBegin || !hitEnd) return 0;
    
    w->begin = ndBegin;
    w->end = ndEnd;

    return 1;
}

char boardWallFloatIntersectionQ() {
    wall *w = board.wallFloat;

    for(int k = 0; k < board.wallsN; k++) {
        wall *w1 = board.walls[k];

        if(boardWallIntersectionQ(w, w1)) return 1;
    }

    return 0;
}

char boardWallFloatPlaceableQ() {
    wall *w = board.wallFloat;

    if(board.wallsN >= BOARD_WALLS_N_MAX) return 0;

    char success = boardWallFloatSnapToBoard(); 
    if(!success) return 0;

    char intersection = boardWallFloatIntersectionQ(); 
    if(intersection) return 0;

    char border = boardWallPositionBorderQ(w);
    if(border) return 0;

    boardWallCellDetect(w); 
    char passable = boardPassableQ();
    boardWallCellDetectUndo();

    if(!passable) return 0;

    return 1;
}

char boardWallFloatPlace() {
    char placeable = boardWallFloatPlaceableQ();
    if(!placeable) return 0;

    wall *w = board.wallFloat;

    int lastWallIndex = board.wallsN;
    boardWallJointDetect(w, lastWallIndex);
    boardWallNodePlace(w);

    boardWallCellDetect(w);
    boardWallShapeApply(w);

    board.walls[board.wallsN++] = w; 
    board.wallFloat = NULL;

    w->shape->color = colors[COLOR_WALL]; 
    
    return 1;
}

void boardWallFloatTranspose() {
    wall *w = board.wallFloat;
    if(!w) return;

    shape *sh = w->shape;
    
    double x;
    x = w->lengthW;
    w->lengthW = w->lengthH;
    w->lengthH = x;
    
    x = sh->dim.w;
    sh->dim.w = sh->dim.h;
    sh->dim.h = x;

    board.wallFloatLengthW = w->lengthW;
    board.wallFloatLengthH = w->lengthH;
}

void boardWallFloatResize(int delta) {
    wall *w = board.wallFloat;
    if(!w) return;

    int length = (w->lengthW ? w->lengthW : w->lengthH) + delta; 
    int lengthMax = w->lengthW ? board.m : board.n;
    
    if(length <= 0 || length > lengthMax) 
        return;

    shape *sh = w->shape;

    if(w->lengthW) {
        w->lengthW = length;
        sh->dim.w += board.stepX * delta;
    } else {
        w->lengthH = length;
        sh->dim.h -= board.stepY * delta;
    }

    board.wallFloatLengthW = w->lengthW;
    board.wallFloatLengthH = w->lengthH;
}
