#include "board-wall-borderwall.h"
#include "board-wall.h"
#include "cell.h"
#include "shape.h"
#include "board.h"
#include "color.h"

void boardWallBorderWallPlaceOpposite(wall *w, piece *p);

void boardWallCreateBorderWall() {
    wall *w = boardWallNew();
    board.borderWalls[board.borderWallsN] = w;
    int lastBorderWallIndex = board.borderWallsN;

    board.borderWallsN++;

    piece *p = &board.pieces[lastBorderWallIndex];

    boardWallBorderWallPlaceOpposite(w, p);
    p->borderWall = w;

    shape *sh = w->shape;

    if(p->selected) 
        sh->color = p->shape->color;
    else 
        sh->color = colors[COLOR_PIECE_DEFAULT];

    boardWallShapeApply(w);
    boardWallCellDetect(w);
}

char boardWallPositionBorderQ(wall *w) {
    if(!w) return 0;

    return w->begin.i == 0 && w->end.i == 0 || 
        w->begin.i == board.n && w->end.i == board.n ||
        w->begin.j == 0 && w->end.j == 0 ||
        w->begin.j == board.m && w->end.j == board.m;
}

void boardWallBorderWallPlaceOpposite(wall *w, piece *p) {
    if(!w || !p) return;
    
    cell *c = p->cell;
    dir *d = &p->targetDir;

    w->begin.i = 0;
    w->begin.j = 0;
    
    w->end.i = board.n;
    w->end.j = board.m;
    
    w->lengthW = board.m;
    w->lengthH = board.n;

    if(d->di) {
        w->begin.i = w->end.i = (d->di + 1) / 2 * board.n;
        w->lengthH = 0;
    }

    if(d->dj) {
        w->begin.j = w->end.j = (d->dj + 1) / 2 * board.m;
        w->lengthW = 0;
    }
}

void boardWallBorderWallColorFromPiece(piece *p) {
    if(p->selected) {
        p->borderWall->shape->color = p->shape->color;
    } else { 
        p->borderWall->shape->color = colors[COLOR_PIECE_DEFAULT];
    } 
}
