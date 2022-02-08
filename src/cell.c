#include "cell.h"
#include "color.h"
#include "polygon.h"
#include "piece.h"
#include "texture.h"

cell *cellNewArray(size_t n) {
    if(n == 0) return NULL;

    cell *cells = malloc(sizeof(cell) * n);
    
    for(int k = 0; k < n; k++) {
        cell *c = &cells[k];

        c->i = 0;
        c->j = 0;

        c->piece = NULL;
        c->visited = 0;
        c->index = 0;
        c->lastWallDirname = 0;
        c->landingSiteVisible = 0;
        
        for(int r = 0; r < DIR_N; r++) {
            c->walls[r] = NULL;
        }

        c->shape = shapeNew();
        shape *sh = c->shape;

        sh->color = colors[COLOR_CELL];

        sh->draw = cellDraw;
        sh->object = c;
    }
    
    return cells;
}

void cellDestroyArray(cell *cells, size_t n) {
    if(!cells) return;

    for(int k = 0; k < n; k++) {
        cell *c = &cells[k];
        if(!c) continue;
            
        shapeDestroy(c->shape);
    }

    free(cells);
}

void cellDraw(shape *sh) {
    cell *c = sh->object;
    texture *tex = sh->texture;
    texture *texLanding = CELL_TEX_LANDING_SITE_INDEX < sh->subtexturesN ?
            sh->subtextures[CELL_TEX_LANDING_SITE_INDEX] : NULL;

    glPushMatrix();
    
    glColor3f(sh->color.r, sh->color.g, sh->color.b);

    glTranslatef(sh->pivot.x, sh->pivot.y, 0);
    glRotatef(sh->angle.phi, 0, 0, 1);

    if(!tex) { 
        rect(0, 0, sh->dim.w, sh->dim.h);
    } else {
        tex->mode = GL_MODULATE;
        texDrawRect(tex, 0, 0, sh->dim.w, sh->dim.h);
    }

    double landingSiteInnerR = sh->dim.w * CELL_PIECE_SCALE_FACTOR / 2;
    double landingSiteR = landingSiteInnerR * PIECE_ACTIVE_SCALE;

    if(c->landingSiteVisible) {
        setColor(COLOR_CELL_LANDING_SITE);

        if(!texLanding) {
            circle(0, 0, landingSiteR, CIRCLE_N);

            glColor3f(sh->color.r, sh->color.g, sh->color.b);
            circle(0, 0, landingSiteInnerR, CIRCLE_N);
        } else {
            tex->mode = GL_MODULATE; 
            texDrawRect(texLanding, 0, 0, landingSiteR * 2, landingSiteR * 2); 
        }
    }

    glPopMatrix();
}

void cellPrint(cell *c) {
    if(!c) return;

    printf("cell {");
    printf("i: %d, j: %d", c->i, c->j);

    if(c->piece) 
        printf(",\n\t*piece: {active: %d}", c->piece->active);
    else 
        printf(",\n\t*piece: NULL");
    
    printf(",\n\twalls: [");
    for(int k = 0; k < DIR_N; k++) {
        wall *w = c->walls[k];

        if(w) 
            printf("*wall: ...");
        else 
            printf("*wall: NULL");
        
        if(k < DIR_N - 1)
            printf(", ");
    }
    printf("]");

    printf(",\n\tvisited: %d", c->visited);
    printf(",\n\tlastWallDirname: %d", c->lastWallDirname);

    printf("\n}\n");
    fflush(stdout);
}
