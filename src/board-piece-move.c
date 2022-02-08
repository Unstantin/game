#include "board-piece-move.h"
#include "game.h"
#include "board.h"
#include "board-cell.h"
#include "board-piece.h"

void boardPieceStep(unsigned int dirname) {
    dir d = dirname2dir(dirname);
    piece *p = board.pieceActive;

    char success;

    if(p->landing) 
        success = boardPieceActiveLandingJump(d);
    else 
        success = board.pieceMove(d);
    
    if(!success) return;

    char reached = boardPieceOppositeSideReachedQ(p, p->cell);

    if(reached) {
        printf("Border reached\n");
        fflush(stdout);

        boardPieceRemoveDisableOthers(board.pieceActive);
    }

#ifdef GAME_DEV_MODE
    p->stepCount++;
#else 
    boardPieceActivateNext();
#endif
}

char boardPieceActiveJumpToCell(cell *c) {
    piece *p = board.pieceActive;

    if(!c) return 0;
    if(c->piece) return 0;

    if(p->landing) {
        if(!boardPieceLandingPossibleQ(p, c)) return 0;
    } else { 
        if(!boardPieceJumpPossibleQ(p, c)) return 0;
    }

    if(p->landing)
        pieceLandingOff(p);

    boardPieceJump(p, c);
    boardPieceShapeXYApply(p);

    return 1;
}

char boardPieceActiveJump(dir d) {
    piece *p = board.pieceActive;
    cell *c = p->cell;

    for(int k = 0; k < 2; k++) {
        if(c->walls[dir2dirname(d)]) break;
        boardCellOffset(&c, d.di, d.dj);
        if(!c) break;

        if(!c->piece) {
            boardPieceJump(p, c);
            boardPieceShapeXYApply(p);

            return 1;
        }
    }

    return 0;
}

char boardPieceActiveJumpAny(dir d) {
    piece *p = board.pieceActive;

    int preferedDir[] = {RIGHT_FIRST, LEFT_FIRST};

    p->landingCellsN = sizeof(preferedDir) / sizeof(int);
    if(p->landingCellsN > PIECE_LANDING_CELLS_N_MAX) 
        p->landingCellsN = PIECE_LANDING_CELLS_N_MAX;

    char choice = 0;
    for(int t = 0; t < p->landingCellsN; t++) { 
        p->landingCells[t] = dir2EmptyCell(p->cell, d, preferedDir[t]);

        if(t > 0 && p->landingCells[t] != p->landingCells[t - 1])
            choice = 1;
    }

    if(!choice) {
        cell *c = p->landingCells[0];
        if(!c) return 0; 
        
        boardPieceJump(p, c);
        boardPieceShapeXYApply(p);

        return 1;
    } else {
        pieceLandingOn(p);
        p->landingDir = d;
        
        return 0;
    }
}

char boardPieceActiveLandingJump(dir d) {
    piece *p = board.pieceActive;
    cell *c = p->cell;
    dir d1 = p->landingDir;

    cell *c1 = c;
    boardCellOffset(&c1, d1.di, d1.dj); 
    boardCellOffset(&c1, d.di, d.dj);
    if(!c1) return 0;

    if(!boardPieceLandingPossibleQ(p, c1)) return 0;

    pieceLandingOff(p);

    boardPieceJump(p, c1);
    boardPieceShapeXYApply(p);

    return 1;
}

char boardPieceActiveMoveSingle(dir d) {
    piece *p = board.pieceActive;
    cell *c = p->cell;

    cell *c1 = c;
    if(c->walls[dir2dirname(d)]) return 0;
    boardCellOffset(&c1, d.di, d.dj);
    if(!c1) return 0;

    if(!c1->piece) {
        boardPieceJump(p, c1);
        boardPieceShapeXYApply(p);
    } else {
        cell *c2 = c1;
        if(c1->walls[dir2dirname(d)]) return 0;
        boardCellOffset(&c2, d.di, d.dj);
        if(!c2) return 0;

        if(!c2->piece) {
            piece *p1 = c1->piece;

            boardPieceJump(p1, c2);
            boardPieceJump(p, c1);
            boardPieceShapeXYApply(p);
            boardPieceShapeXYApply(p1);
        }
    }

    return 1;
}

char boardPieceActiveMove(dir d) {
    piece *p = board.pieceActive;
    cell *c = p->cell;

    cell *c1 = c;

    do {
        if(c1->walls[dir2dirname(d)]) return 0;
        boardCellOffset(&c1, d.di, d.dj);
        if(!c1) return 0;
    } while(c1->piece);


    cell *c2 = c1;

    while(c2 != c) {
        boardCellOffset(&c2, -d.di, -d.dj);
        boardPieceJump(c2->piece, c1);
        boardPieceShapeXYApply(c1->piece);

        c1 = c2;
    }

    return 1;
} 

char boardPieceActiveMoveRecursive(dir d) {
    piece *p = board.pieceActive;
    cell *c = p->cell;

    return boardPieceActiveMoveRecursiveStep(c, d);
}

char boardPieceActiveMoveRecursiveStep(cell *c, dir d) {
    piece *p = c->piece;
    if(!p) return 1;

    if(c->walls[dir2dirname(d)]) return 0;

    cell *c1 = c;

    boardCellOffset(&c1, d.di, d.dj);
    if(!c1) return 0;

    char success = boardPieceActiveMoveRecursiveStep(c1, d);
    if(!success) return 0;


    boardPieceJump(p, c1);
    boardPieceShapeXYApply(p);

    return 1; 
}

void boardPieceJump(piece *p, cell *c) {
    if(!p) return;
    
    if(p->cell) p->cell->piece = NULL;
    p->cell = c;

    if(!c) return;

    if(c->piece) c->piece->cell = NULL;
    c->piece = p;
}

