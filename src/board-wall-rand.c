#include "board-wall-rand.h"
#include "board.h"
#include "board-wall.h"
#include "board-wall-borderwall.h"
#include "color.h"
#include "shape.h"
#include "board-path.h"

#define BOARD_WALL_DISTRIBUTE_ATTEMPTS_FACTOR 200
#define BOARD_WALLS_RAND_N 5

char boardWallRandDistribute(wall *w, int lastWallIndex);
void boardWallRandDistributeAttempt(wall *w);

void boardWallRandInit() {
    for(int k = 0; k < BOARD_WALLS_RAND_N; k++) {
        boardWallRandCreate();
    }
}

void boardWallRandCreate() {
    if(board.wallsN >= BOARD_WALLS_N_MAX) return;

    wall *w = boardWallNew();
    board.walls[board.wallsN] = w;
    int lastWallIndex = board.wallsN;
    
    char success = boardWallRandDistribute(w, lastWallIndex);
    if(!success) { 
        wallDestroy(w);
        return;
    }

    board.wallsN++;

    boardWallJointDetect(w, lastWallIndex);
    boardWallNodePlace(w);
    boardWallShapeApply(w);
    boardWallCellDetect(w);

//    shape *sh = w->shape;
//    sh->color.r = randRange(0.2, 0.8);
//    sh->color.g = randRange(0, 0);
//    sh->color.b = randRange(0.5, 0.8);
}

char boardWallRandDistribute(wall *w, int lastWallIndex) {
    char repeat;
    int attempts = 0;
    int attemptsMax = BOARD_WALL_DISTRIBUTE_ATTEMPTS_FACTOR * board.nm;

    do {
        attempts++;
        if(attempts > attemptsMax) 
            return 0;

        repeat = 0;
        boardWallRandDistributeAttempt(w);

        for(int k = 0; k < lastWallIndex; k++) { 
            if(boardWallIntersectionQ(board.walls[k], w)) {
                repeat = 1;
                break;
            }
        }

        if(repeat) continue;

        boardWallCellDetect(w); 
        char passable = boardPassableQ();
        boardWallCellDetectUndo();

        if(!passable) 
            repeat = 1;
    } while(repeat); 

    return 1;
}

void boardWallRandDistributeAttempt(wall *w) {
    char vert = rand() % 2;

    char border = 0;
    do {
        w->begin.i = rand() % (board.n + 1);
        w->begin.j = rand() % (board.m + 1);
        w->end.i = vert ? rand() % (board.n + 1) : w->begin.i;
        w->end.j = vert ? w->begin.j : rand() % (board.m + 1);
        
        border = boardWallPositionBorderQ(w);

        w->lengthH = w->begin.i > w->end.i ?
            w->begin.i - w->end.i : w->end.i - w->begin.i;
        w->lengthW = w->begin.j > w->end.j ?
            w->begin.j - w->end.j : w->end.j - w->begin.j;
    } while(w->lengthH + w->lengthW == 0 || border);

} 
