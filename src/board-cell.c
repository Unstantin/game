#include "board-cell.h"
#include "board-piece.h"
#include "board.h"
#include "shape.h"
#include "texture.h"

#define BOARD_CELL_TEX_FILENAME "cell.png"
#define BOARD_CELL_TEX_LANDING_SITE_FILENAME "landingsite.png"
#define BOARD_CELL_SUBTEXTURES_N 1

void boardCellTextureInit();

void boardCellInit() {
    board.cells = cellNewArray(board.nm);

    for(int k = 0; k < board.nm; k++) {
        cell *c = &board.cells[k];

        c->i = k / board.m;
        c->j = k % board.m;

        shape *sh = c->shape;

        cell2pivot(c, &sh->pivot);

        sh->dim.w = board.cellW;
        sh->dim.h = board.cellH;
    } 

    boardCellTextureInit();
}

void boardCellTextureInit() {
    texture *tex = texNew();
    tex->filename = BOARD_CELL_TEX_FILENAME;

    int error = texInit(tex);
    if(error) return;

    texture *texLanding = texNew();
    texLanding->filename = BOARD_CELL_TEX_LANDING_SITE_FILENAME;

    error = texInit(texLanding);
    if(error) return;
    
    for(int k = 0; k < board.nm; k++) {
        cell *c = &board.cells[k];
        shape *sh = c->shape;

        sh->texture = tex; 
        
        shapeSubtexNewArray(sh, BOARD_CELL_SUBTEXTURES_N);
        sh->subtextures[CELL_TEX_LANDING_SITE_INDEX] = texLanding;
    }
}

/******************************************************/

void boardCellOffset(cell **pc, int di, int dj) {
    if(!pc) return;

    cell *c = *pc; 
    if(!c) return;
    
    if(!boardPositionInsideQ(c->i + di, c->j + dj)) {
        *pc = NULL;
        return;
    }

    int offset = board.m * di + dj;
    c += offset;

    *pc = c;
}

void boardCellVisitedReset() {
    for(int k = 0; k < board.nm; k++) {
        board.cells[k].visited = 0;
    }
}

void boardCellIndexReset() {
    for(int k = 0; k < board.nm; k++) {
        board.cells[k].index = 0;
    }
}

cell *dir2EmptyCell(cell *c, dir d, char preferedDir) {
    if(!c) return NULL;

    cell *c1 = dir2cell(c, d);
    if(!c1) return NULL;
        
    if(!c1->piece)
        return c1;

    char f = preferedDir == RIGHT_FIRST ? 1 : -1;

    dir d1[] = {
        d, {d.dj * f, -d.di * f}, {-d.dj * f, d.di * f}  
    };

    for(int k = 0; k < 3; k++) {
        cell *c2 = c1;
        if(!c2->walls[dir2dirname(d1[k])]) { 
            boardCellOffset(&c2, d1[k].di, d1[k].dj);
            if(!c2) continue;

            if(c2->piece) continue;

            return c2;
        }
    }

    return NULL;
}

cell *dir2cell(cell *c, dir d) {
    if(!c) return NULL;

    if(c->walls[dir2dirname(d)]) return NULL;

    cell *c1 = c;

    boardCellOffset(&c1, d.di, d.dj);

    return c1;
}


dir boardCellDirToOppositeSide(cell *c) {
    dir d = {0};

    if(!c) return d;

    char sideI = (c->i + board.n / 2) / (board.n - 1) * 2 - 1; 
    if(c->i == board.n / 2) 
        sideI = 0;

    char sideJ = (c->j + board.m / 2) / (board.m - 1) * 2 - 1;
    if(c->j == board.m / 2) 
        sideJ = 0;

    d.di = -sideI;
    d.dj = -sideJ;
    
    return d;
}

void boardCellColorReset() {
    for(int k = 0; k < board.nm; k++) {
        cell *c = &board.cells[k];

        c->shape->color = colors[COLOR_CELL]; 
    }
}

