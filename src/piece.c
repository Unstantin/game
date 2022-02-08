#include "piece.h"
#include "color.h"
#include "polygon.h"
#include "dir.h"
#include "texture.h"

void pieceDraw(shape *sh);
void pieceColorRandDist(piece *p);
void pieceLandingMode(piece *p, char mode);

enum piece_landing_mode {LANDING_ON, LANDING_OFF};

piece *pieceNewArray(size_t n) {
    if(n == 0) return NULL;

    piece *pieces = malloc(sizeof(piece) * n);

    for(int k = 0; k < n; k++) {
        piece *p = &pieces[k];

        p->active = 0;
        p->stepCount = 0;
        p->enabled = 0;
        p->selected = 0;
        p->landing = 0;
        p->cell = NULL;
        p->borderWall = NULL;
        p->landingCellsN = PIECE_LANDING_CELLS_N_MAX;

        dirInit(&p->landingDir);
        dirInit(&p->targetDir);

        for(int k = 0; k < PIECE_LANDING_CELLS_N_MAX; k++) 
            p->landingCells[k] = NULL;

        p->shape = shapeNew();
        shape *sh = p->shape;

        sh->color = colors[COLOR_PIECE_DEFAULT];

        sh->draw = pieceDraw;
        sh->object = p;
    }

    return pieces;
}

void pieceDestroyArray(piece *pieces, size_t n) {
    if(!pieces) return;

    for(int k = 0; k < n; k++) {
        piece *p = &pieces[k];
        if(!p) continue;
            
        shapeDestroy(p->shape);
    }

    free(pieces);
}

void pieceDraw(shape *sh) {
    piece *p = sh->object;
    texture *tex = sh->texture;
    texture *texOutline = PIECE_TEX_OUTLINE_INDEX < sh->subtexturesN ?
        sh->subtextures[PIECE_TEX_OUTLINE_INDEX] : NULL;

    glPushMatrix();
    glTranslatef(sh->pivot.x, sh->pivot.y, 0);
    glRotatef(sh->angle.phi, 0, 0, 1);

    if(p->active) {
        setColor(COLOR_PIECE_ACTIVE);

        double outlineR = sh->dim.w * PIECE_ACTIVE_SCALE / 2;

        if(!texOutline) { 
            circle(0, 0, outlineR, CIRCLE_N);
        } else { 
            texOutline->mode = GL_MODULATE;
            texDrawRect(texOutline, 0, 0, 2 * outlineR, 2 * outlineR); 
        }
    }

    if(p->selected) {
        glColor3f(sh->color.r, sh->color.g, sh->color.b);
    } else {
        setColor(COLOR_PIECE_DEFAULT);
    }

    if(!tex) {
        circle(0, 0, sh->dim.w / 2, CIRCLE_N);
    } else {
        tex->mode = GL_MODULATE;
        texDrawRect(tex, 0, 0, sh->dim.w, sh->dim.h);
    }
    
    glPopMatrix();
}
double randRange(double x, double y);

void pieceColorRandDist(piece *p) {
    shape *sh = p->shape;
    cell *c = p->cell;
    shape *shc = c ? c->shape : NULL;
    
    double min = PIECE_COLOR_RANGE_MIN;
    double max = PIECE_COLOR_RANGE_MAX;

    while(1) {
        sh->color.r = randRange(min, max);
        sh->color.g = randRange(min, max);
        sh->color.b = randRange(min, max);

        char colorCheckFailed;

        colorCheckFailed = shc && !colorDistCheck(sh->color, shc->color, PIECE_COLOR_DIST_MIN);
        if(colorCheckFailed) continue;

        int clrnames[] = {
            COLOR_PIECE_ACTIVE, COLOR_BOARD_BACKGROUND, 
            COLOR_PIECE_DEFAULT, COLOR_CELL_LANDING_SITE, 
            COLOR_WALL, COLOR_NODE
        };

        int clrN = sizeof(clrnames) / sizeof(int);

        for(int k = 0; k < clrN; k++) {
            colorCheckFailed = !colorDistCheck(
                sh->color, colors[clrnames[k]],
                PIECE_COLOR_DIST_MIN
            );
            
            if(colorCheckFailed) break;
        }

        if(!colorCheckFailed) break;
    }
} 

void pieceLandingOn(piece *p) {
    pieceLandingMode(p, LANDING_ON); 
}

void pieceLandingOff(piece *p) {
    pieceLandingMode(p, LANDING_OFF); 
}

void pieceLandingMode(piece *p, char mode) {
    if(!p) return;

    char modeOn = mode == LANDING_ON; 

    p->selected = modeOn ? 0 : 1;
    p->landing = modeOn ? 1 : 0;

    for(int k = 0; k < p->landingCellsN; k++) { 
        cell *c = p->landingCells[k];
        if(!c) continue;

        c->landingSiteVisible = modeOn ? 1 : 0;
    }
}

void piecePrint(piece *p) {
    if(!p) return;

    printf("piece {");
    printf("i: %d, j: %d, active: %d, stepCount: %d,"
           "targetDir: {di: %d, dj: %d}",
        p->cell->i, p->cell->j, p->active,
        p->stepCount, p->targetDir.di, p->targetDir.dj);
    printf("}\n");
    fflush(stdout);
}

