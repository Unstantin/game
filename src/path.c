#include "path.h"
#include "board.h"
#include "color.h"

void pathPush(path *pth, cell *c) {
    pth->pos++;
    pth->cells[pth->pos] = c; 
    pth->cell = c;
    
    if(c) c->visited = 1;
}

void pathPop(path *pth) {
    if(pth->pos == 0) return;
    
    if(pth->cell) pth->cell->visited = 2;
    pth->pos--;
    pth->cell = pth->cells[pth->pos];
}

void pathInit(path *pth, cell *c, char (*successCond)(piece *, cell *)) {
    if(c) {
        pth->cells[0] = c;   
        pth->cell = c;

        pth->pos = 0;
        pth->piece = c->piece;

        c->visited = 1;
    } else 
        pth->pos = -1;

    pth->successCondition = successCond;
    pth->successCallback = NULL;
}

void pathShow(path *pth) {
    piece *p = pth->piece;

    color clr = p ? p->shape->color : colors[COLOR_PATH_DEFAULT];

    for(int k = 0; k <= pth->pos; k++)
        pth->cells[k]->shape->color = clr; 
}

void pathShowPos(path *pth) {
    cell *c = pth->cells[pth->pos];
    piece *p = pth->piece;
    
    c->shape->color = p ? p->shape->color : colors[COLOR_PATH_DEFAULT];
}

void pathHidePos(path *pth) {
    cell *c = pth->cells[pth->pos];
    c->shape->color = colors[COLOR_CELL];
}

