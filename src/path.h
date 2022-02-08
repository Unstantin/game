#include <stdio.h>
#include <stdlib.h>

#ifndef PATH
#define PATH

#include "piece.h"
#include "cell.h"
#include "game.h"

#define BOARD_PATH_LENGTH_MAX (BOARD_N * BOARD_M) 

typedef struct path_ path;
struct path_ {
    cell *cells[BOARD_PATH_LENGTH_MAX];
    cell *cell;
    int pos;
    piece *piece;
    char (*successCondition)(piece *p, cell *c);
    void (*successCallback)(void);
};

void pathInit(path *pth, cell *c, char (*successCond)(piece *, cell *));
void pathPush(path *pth, cell *c);
void pathPop(path *pth);
void pathShow(path *pth);
void pathShowPos(path *pth);
void pathHidePos(path *pth);

#endif 
