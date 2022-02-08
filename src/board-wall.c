#include "board-wall.h"
#include "board-wall-borderwall.h"
#include "board-wall-rand.h"
#include "board.h"
#include "piece.h"
#include "cell.h"
#include "shape.h"
#include "color.h"
#include "mouse.h"
#include "dir.h"
#include "path.h"
#include "texture.h"
#include "node.h"
#include "board-node.h"

#define BOARD_WALL_TEX_FILENAME "wall.png"

void boardWallCreateTheWall();
void boardWallTextureInit();

wall *boardWallNew() {
    wall *w = wallNew();
    shape *sh = w->shape;
    
    sh->texture = board.wallTexture;

    return w;
}

void boardWallInit() {
    boardWallTextureInit();

//    boardWallCreateTheWall();

    for(int t = 0; t < board.piecesN; t++) {
       boardWallCreateBorderWall(); 
    }
} 

void boardWallTextureInit() {
    texture *tex = texNew();
    tex->filename = BOARD_WALL_TEX_FILENAME;

    int error = texInit(tex);
    if(error) return;
    
    board.wallTexture = tex; 
}

void boardWallCreateTheWall() {
    wall *w = boardWallNew(); 
    board.theWall = w;

    w->begin.i = 2;
    w->begin.j = 2;
    w->end.i = 5;
    w->end.j = 2;

    w->lengthW = 0;
    w->lengthH = 3;

    boardWallShapeApply(w);
    
    boardWallCellDetect(w);
}

/*******************************************************/

char boardWallIntersectionQ(wall *w1, wall *w2) {
    int u1 = w1->begin.i < w1->end.i ? w1->begin.i : w1->end.i;
    int d1 = w1->begin.i < w1->end.i ? w1->end.i : w1->begin.i;
    int l1 = w1->begin.j < w1->end.j ? w1->begin.j : w1->end.j;
    int r1 = w1->begin.j < w1->end.j ? w1->end.j : w1->begin.j;

    int u2 = w2->begin.i < w2->end.i ? w2->begin.i : w2->end.i;
    int d2 = w2->begin.i < w2->end.i ? w2->end.i : w2->begin.i;
    int l2 = w2->begin.j < w2->end.j ? w2->begin.j : w2->end.j;
    int r2 = w2->begin.j < w2->end.j ? w2->end.j : w2->begin.j;

    char intersectionI = u2 < u1 && u1 < d2 ||
        u2 < d1 && d1 < d2 ||
        u1 < u2 && u2 < d1 ||
        u1 < d2 && d2 < d1 ||
        u1 == d1 && u2 == d2 && u1 == u2 ||
        (u1 == u2 || d1 == d2) && d1 != u1 && d2 != u2;

    char intersectionJ = l2 < l1 && l1 < r2 ||
        l2 < r1 && r1 < r2 ||
        l1 < l2 && l2 < r1 ||
        l1 < r2 && r2 < r1 ||
        l1 == r1 && l2 == r2 && l1 == l2 ||
        (l1 == l2 || r1 == r2) && r1 != l1 && r2 != l2;

    return intersectionI && intersectionJ;
}

void boardWallJointDetect(wall *w, int lastWallIndex) {
    node *ends[] = {&w->begin, &w->end};

    for(int endindex = 0; endindex < 2; endindex++) {
        char jointNeeded = 0;

        for(int k = 0; k < lastWallIndex; k++) {
            wall *w1 = board.walls[k];

            node *ends1[] = {&w1->begin, &w1->end};

            for(int endindex1 = 0; endindex1 < 2; endindex1++) {
                node *nd = ends[endindex];
                node *nd1 = ends1[endindex1];

                if(nd->i == nd1->i && nd->j == nd1->j) {
                    if(nd1->joint) {
                        jointNeeded = 0;
                        goto finish;
                    } else {
                        jointNeeded = 1;
                    }
                }
            }
        }
finish:

        ends[endindex]->joint = jointNeeded;
    }
}

void boardWallCellDetect(wall *w) {
    pathInit(&board.lastWallPath, NULL, NULL);

    int di = w->end.i > w->begin.i ? 1 : (w->end.i == w->begin.i ? 0 : -1);
    int dj = w->end.j > w->begin.j ? 1 : (w->end.j == w->begin.j ? 0 : -1);

    int dbi = di >= 0 ? 0 : -1;
    int dei = di < 0 ? -1 : 0;

    int dbj = dj >= 0 ? 0 : -1;
    int dej = dj < 0 ? -1 : 0;

    for(int i = w->begin.i + dbi, j = w->begin.j + dbj;
            i != w->end.i + dei || j != w->end.j + dej;
            i += di, j += dj) {
        int k; 
        cell *c[] = {NULL, NULL};

        if(boardPositionInsideQ(i, j)) {
            k = i * board.m + j;
            c[0] = &board.cells[k];
        }

        int i1 = i - !!dj;
        int j1 = j - !!di;

        if(boardPositionInsideQ(i1, j1)) {
            k = i1 * board.m + j1;
            c[1] = &board.cells[k];
        }

        for(int k = 0; k < 2; k++) {
            if(!c[k]) continue;

            char dirname = DIR_N;
            if(dj)  
                dirname = k == 0 ? DIR_UP : DIR_DOWN;
            else if(di) 
                dirname = k == 0 ? DIR_LEFT : DIR_RIGHT;

            if(dirname != DIR_N) {
                c[k]->walls[dirname] = w;
                c[k]->lastWallDirname = dirname;
                pathPush(&board.lastWallPath, c[k]);
            }
        }
    }
}

void boardWallCellDetectUndo() {
    path *pth = &board.lastWallPath;

    for(int k = 0; k <= pth->pos; k++) {
        cell *c = pth->cells[k];

        char dirname = c->lastWallDirname;

        c->walls[dirname] = NULL;
//        cellPrint(c);
    }
}

void boardWallShapeApply(wall *w) {
    shape *sh = w->shape;

    pivot pvt1, pvt2; 

    node2pivot(w->begin, &pvt1);
    node2pivot(w->end, &pvt2);

    sh->pivot.x = (pvt1.x + pvt2.x) / 2; 
    sh->pivot.y = (pvt1.y + pvt2.y) / 2; 

    if(w->lengthW) {
        int jointFactorX = w->begin.j < w->end.j ? 1 : -1;

        if(w->begin.joint) sh->pivot.x -= jointFactorX * board.spaceX / 2;
        if(w->end.joint) sh->pivot.x += jointFactorX * board.spaceX / 2;
    }

    if(w->lengthH) {
        int jointFactorY = w->begin.i < w->end.i ? 1 : -1;

        if(w->begin.joint) sh->pivot.y += jointFactorY * board.spaceY / 2;
        if(w->end.joint) sh->pivot.y -= jointFactorY * board.spaceY / 2;
    }

    if(w->lengthW) {
        sh->dim.w = board.stepX * w->lengthW - board.spaceX; 

        if(w->begin.joint) sh->dim.w += board.spaceX; 
        if(w->end.joint) sh->dim.w += board.spaceX;
    }
    else 
        sh->dim.w = board.spaceX;

    if(w->lengthH) {
        sh->dim.h = -board.stepY * w->lengthH - board.spaceY; 

        if(w->begin.joint) sh->dim.h += board.spaceY; 
        if(w->end.joint) sh->dim.h += board.spaceY;
    }
    else 
        sh->dim.h = board.spaceY;
}

void boardWallNodePlace(wall *w) {
    int di = w->end.i > w->begin.i ? 1 : (w->end.i == w->begin.i ? 0 : -1);
    int dj = w->end.j > w->begin.j ? 1 : (w->end.j == w->begin.j ? 0 : -1);
    
    for(int i = w->begin.i, j = w->begin.j; ; i += di, j += dj) {
        node *nd = board.nodes[(board.m + 1) * i + j];

        if(!nd) {
            node nd1 = {.i = i, .j = j};
            nd = boardNodeCreate(&nd1);
        }

        if(di) {
            nd->jointY = 1;
        } 
        else if(dj) {
            nd->jointX = 1;
        }

        boardNodeJointApply(nd);

        if(i == w->end.i && j == w->end.j) break;
    }
}



