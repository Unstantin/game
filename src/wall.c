#include "wall.h"
#include "polygon.h"
#include "color.h"

wall *wallNew() {
    wall *w = malloc(sizeof(wall));

    w->begin.i = 0;
    w->begin.j = 0;
    w->begin.joint = 0;

    w->end.i = 0;
    w->end.j = 0;
    w->end.joint = 0;

    w->lengthW = 0;
    w->lengthH = 0;

    w->shape = shapeNew();
    shape *sh = w->shape;

    sh->color = colors[COLOR_WALL];

    sh->draw = wallDraw;
    sh->object = w;

    return w;
}

void wallDestroy(wall *w) {
    if(!w) return;

    shapeDestroy(w->shape);
    free(w);
}

void wallDraw(shape *sh) {
    texture *tex = sh->texture;

    double w = sh->dim.w;
    double h = sh->dim.h;

    glPushMatrix();
    
    glColor3f(sh->color.r, sh->color.g, sh->color.b);

    glTranslatef(sh->pivot.x, sh->pivot.y, 0);
    glRotatef(sh->angle.phi, 0, 0, 1);

    if(!tex) {
        rect(0, 0, w, h);
    } else {
        tex->orientation = h > w ? TEX_IDENTITY : TEX_ROTATE_CW_90;
        tex->mode = GL_MODULATE;
        texDrawRect(tex, 0, 0, w, h);
    }

    glPopMatrix();
}

void wallPrint(wall *w) {
    if(!w) return;

    printf("wall {\n");
    printf("\tbegin ");
    nodePrintChild(&w->begin);
    printf(",\n\tend ");
    nodePrintChild(&w->end);
    printf(",\n\tlengthW: %d, lengthH: %d", w->lengthW, w->lengthH);
    printf(",\n\t*shape: ...");
    printf("\n}\n");

    fflush(stdout);
}
