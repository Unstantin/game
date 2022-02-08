#include "board-path.h"
#include "board-piece.h"
#include "board-cell.h"
#include "path.h"
#include "board.h"
#include "dir.h"
#include "cell.h"
#include "timer.h"

char step();
void pathShow();
void stepDelayed();
void boardPathSearchAnimateStep();

struct board_path_ board_path = {
    .stepCount = 0,
    .visible = 0,
    .pieceAnimatedFirst = NULL
};

path thread = {
    .cells = {0},
    .cell = NULL,
    .pos = 0, 
    .piece = NULL,
    .successCondition = NULL,
    .successCallback = NULL
};

char boardPathToOppositeSideExistsQ(piece *p) {
    if(!p) return 0;
    if(!p->cell) return 0;
    
    boardCellVisitedReset();
    pathInit(&thread, p->cell, boardPieceOppositeSideReachedQ);  

    board.path->stepCount = 0;
    
    char success = step();
    
    if(board.path->visible) {
        pathShow(&thread);
        glutPostRedisplay();
    }


//    printf("stepCount: %ld\n", board.path->stepCount); 
//    fflush(stdout);

    return success;
}

char step() {
    board.path->stepCount++;
    path *th = &thread;
    piece *p = th->piece;
    cell *c;
    
    if(th->successCondition(p, th->cell)) {
        return 1;
    } else {
        dir d = p->targetDir;

        dir d1[] = {
            d, {d.dj, -d.di}, {-d.dj, d.di}, {-d.di, -d.dj}  
        };

        for(int k = 0; k < DIR_N; k++) { 
            c = dir2cell(th->cell, d1[k]);
            if(!c) continue;

            if(c->visited) continue;

            pathPush(th, c);
            char success = step();

            if(success) {
                return 1;
            }

            pathPop(th);
        }
        return 0;
    } 
}

char boardPassableQ() {
    boardCellColorReset();

    for(int k = 0; k < board.piecesN; k++) { 
        piece *p = &board.pieces[k];

        if(!p->enabled) continue;

        if(!boardPathToOppositeSideExistsQ(p))
            return 0;
    }

    return 1;
}

void stepDelayed() {
    board.path->stepCount++;
    path *th = &thread;
    piece *p = th->piece;
    cell *c = th->cell;
    cell *c1;
    
    if(th->successCondition(p, c)) {
        if(th->successCallback)
            timerDelay(th->successCallback);
    } else {
        dir d = p->targetDir;

        dir d1[] = {
            d, {d.dj, -d.di}, {-d.dj, d.di}, {-d.di, -d.dj}  
        };

        c1 = dir2cell(c, d1[c->index]);

        if(!c1 || c1->visited) {
            c->index++;

            if(c->index >= DIR_N) {
                pathHidePos(th);
                pathPop(th);
            } else {
                stepDelayed();
                return;
            }
        } else {
            pathPush(th, c1); 
            pathShowPos(th);
        }

        glutPostRedisplay();

        timerDelay(stepDelayed);

//        printf("timer\n");
//        fflush(stdout);
    } 
}

void boardPathSearchAnimate() {
    board.path->pieceAnimatedFirst = NULL;
    boardPathSearchAnimateStep();
}

void boardPathSearchAnimateStep() {
    boardCellColorReset();

    boardPieceActivateNext();
    
    piece *p = board.pieceActive;

    if(!board.path->pieceAnimatedFirst) {
        board.path->pieceAnimatedFirst = p;
    } 
    else if(p == board.path->pieceAnimatedFirst) {
        glutPostRedisplay();
        return;
    }

    if(!p) return;
    if(!p->cell) return;

    path *th = &thread;

    boardCellVisitedReset();
    boardCellIndexReset();

    pathInit(th, p->cell, boardPieceOppositeSideReachedQ);  
    th->successCallback = boardPathSearchAnimateStep;
    board.path->stepCount = 0;

    pathShowPos(th);
    timerDelay(stepDelayed);
}

