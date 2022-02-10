#include "board-piece.h"
#include "board-piece-move.h"
#include "board-cell.h"
#include "board.h"
#include "shape.h"
#include "color.h"
#include "dir.h"
#include "game.h"
#include "texture.h"

#define BOARD_PIECE_RAND_LEVEL_PASS_FACTOR 4
#define BOARD_PIECE_SUBTEXTURES_N 1
#define BOARD_PIECE_TEX_FILENAME "piece.png"
#define BOARD_PIECE_TEX_OUTLINE_FILENAME "pieceoutline.png"

void boardPieceColorRandDist(int lastPieceIndex);
void boardPieceDistributeRand();
void boardPieceDistributeBorder();
piece *boardPieceNextEnabled(piece *p0);
void boardPieceTextureInit();

enum piece_name {PIECE_FIRST, PIECE_SECOND, PIECE_THIRD, PIECE_FOURTH};

unsigned int piecename2dirname[] = {
    [PIECE_FIRST] = DIR_LEFT,
    [PIECE_SECOND] = DIR_RIGHT,
    [PIECE_THIRD] = DIR_UP,
    [PIECE_FOURTH] = DIR_DOWN 
};

void boardPieceInit() {
    board.pieces = pieceNewArray(board.piecesN);

//    boardPieceDistributeRand();
    boardPieceDistributeBorder();
    
    char selectionFlag[BOARD_PIECES_N] = {
        [DIR_LEFT] = 1,
        [DIR_RIGHT] = 1
    };

    for(int k = 0; k < board.piecesN; k++) {
        piece *p = &board.pieces[k];

        p->active = 0;
        p->enabled = 1;

        if(selectionFlag[k]) 
            boardPieceSelectionSwitch(p);

        int lastPieceIndex = k;
        boardPieceColorRandDist(lastPieceIndex);

        shape *sh = p->shape;
        
        sh->pivot.x = p->cell->shape->pivot.x;
        sh->pivot.y = p->cell->shape->pivot.y;

        sh->dim.w = board.cellW * CELL_PIECE_SCALE_FACTOR;
        sh->dim.h = board.cellH * CELL_PIECE_SCALE_FACTOR;
    }

    boardPieceActivate(&board.pieces[PIECE_FIRST]);
    
//    board.pieceMove = boardPieceActiveMoveRecursive;
//    board.pieceMove = boardPieceActiveMove;
//    board.pieceMove = boardPieceActiveMoveSingle;
//    board.pieceMove = boardPieceActiveJump;
    board.pieceMove = boardPieceActiveJumpAny;

    boardPieceTextureInit();
}

void boardPieceTextureInit() {
    texture *tex = texNew();
    tex->filename = BOARD_PIECE_TEX_FILENAME;
    tex->mipmap = 1;

    int error = texInit(tex);
    if(error) return;

    texture *texOutline = texNew();
    texOutline->filename = BOARD_PIECE_TEX_OUTLINE_FILENAME;
    texOutline->mipmap = 1;

    error = texInit(texOutline);
    if(error) return;
    
    for(int k = 0; k < board.piecesN; k++) {
        piece *p = &board.pieces[k];
        shape *sh = p->shape;

        sh->texture = tex; 

        shapeSubtexNewArray(sh, BOARD_PIECE_SUBTEXTURES_N);
        sh->subtextures[PIECE_TEX_OUTLINE_INDEX] = texOutline;
    }
}


void boardPieceColorRandDist(int lastPieceIndex) {
    piece *p = &board.pieces[lastPieceIndex];
    shape *sh = p->shape;

    char colorCheckFailed;
    do {
        pieceColorRandDist(p);

        colorCheckFailed = 0;
        for(int i = 0; i < lastPieceIndex; i++) {
            shape *sh1 = board.pieces[i].shape;
            if(!colorDistCheck(sh->color, sh1->color, PIECE_COLOR_DIST_MIN)) 
                colorCheckFailed = 1;
        }
    } while(colorCheckFailed);
}

void boardPieceDistributeRand() {
    if(!board.pieces) return;

    double randLevel = (double)board.piecesN / board.nm / 
                            BOARD_PIECE_RAND_LEVEL_PASS_FACTOR;

    int piecename = 0;
    piece *p = board.pieces;

    while(piecename < board.piecesN) {
        for(int cellname = 0; cellname < board.nm; cellname++) {
            cell *c = &board.cells[cellname];

            if(!c->piece && (double)rand() / RAND_MAX <= randLevel) {
                c->piece = p;
                p->cell = c;
                
                p++;
                piecename++;

                if(piecename == board.piecesN) break;
            }
        }
    }
}

void boardPieceDistributeBorder() {
    int cellnames[] = {
        [DIR_LEFT] = (board.n / 2) * board.m, 
        [DIR_RIGHT] = (board.n / 2) * board.m + board.m - 1, 
        [DIR_UP] = board.m / 2, 
        [DIR_DOWN] = (board.n - 1) * board.m + board.m / 2
    };

    for(int k = 0; k < board.piecesN; k++) {
        cell *c = &board.cells[cellnames[piecename2dirname[k]]];
        piece *p = &board.pieces[k];

        c->piece = p;
        p->cell = c;

        p->targetDir = boardCellDirToOppositeSide(c);
    }
} 

char boardPieceOppositeSideReachedQ(piece *p, cell *c) {
    if(!c) return 0;

    wall *w = p->borderWall;
    if(!w) return 0;
    
    for(int k = 0; k < DIR_N; k++) {
        if(c->walls[k] == w) return 1;
    }

    return 0;
}

/***********************************************************/

char boardPieceActivate(piece *p) {
    if(!p) return 0;
    if(!p->enabled) return 0;

    if(board.pieceActive)
        board.pieceActive->active = 0;

    board.pieceActive = p;
    p->active = 1;

    p->stepCount = 0;
    
    return 1;
}


piece *boardPieceNextEnabled(piece *p0) {
    if(!p0) return NULL;

    piece *p = p0;

    do {
        p++;
        int pieceActiveIndex = p - board.pieces;
        p = board.pieces + pieceActiveIndex % board.piecesN;
    } while(!p->enabled);

    return p;
}

void boardPieceActivateNext() {
    piece *p = boardPieceNextEnabled(board.pieceActive);
    boardPieceActivate(p);
}

void boardPieceActiveFirstEnabled() {
    piece *p = &board.pieces[PIECE_FIRST];

    char success = boardPieceActivate(p); 
    if(!success) {
        piece *p1 = boardPieceNextEnabled(p);
        boardPieceActivate(p1);
    }
}

char boardPieceJumpPossibleQ(piece *p, cell *c) {
    if(!p) return 0; 
    if(!c) return 0;

    if(c->piece) return 0;

    cell *c1 = p->cell;
    if(!c1) return 0;

    if(c1 == c) return 0;
    
    int preferedDir[] = {RIGHT_FIRST, LEFT_FIRST};

    p->stepCount = 0;
    for(int k = 0; k < DIR_N; k++) {
        for(int t = 0; t < 2; t++) {
            cell *c2 = dir2EmptyCell(c1, dirs[k], preferedDir[t]);
            if(!c2) continue;

            if(c == c2) return 1;
        }
    }

    return 0;
}

void boardPieceShapeXYApply(piece *p) {
    if(!p) return;

    shape *sh = p->shape;
    cell2pivot(p->cell, &sh->pivot);
}

char boardPieceHitQ(piece *p, point pnt) {
    if(!p) return 0;

    shape *sh = p->shape;

    return shapeHitCircleQ(sh, pnt);
}

char boardPieceSelectionSwitch(piece *p) {
    if(!p) return 0;

    p->selected = !p->selected;

    board.piecesSelectedN += p->selected ? 1 : -1;

    return p->selected;
}

void boardPieceSelectedToEnabled() {
    for(int k = 0; k < board.piecesN; k++) {
        piece *p = &board.pieces[k];

        if(!p->selected) 
            boardPieceRemoveDisable(p);
    }
}

void boardPieceRemoveDisableOthers(piece *p) {
    for(int k = 0; k < board.piecesN; k++) {
        piece *p1 = &board.pieces[k];
        if(p1 == p) continue;
        
        boardPieceRemoveDisable(p1);
    }
}

void boardPieceRemoveDisable(piece *p) {
    if(!p) return;

    boardPieceJump(p, NULL);
    p->enabled = 0;
}

char boardPieceLandingPossibleQ(piece *p, cell *c) {
    if(!p) return 0;
    if(!c) return 0;
    if(!p->landing) return 0;

    for(int k = 0; k < p->landingCellsN; k++) {
        if(p->landingCells[k] == c) 
            return 1;
    }

    return 0;
}


