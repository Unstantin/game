#include "shape.h"
#include "polygon.h"

shape *shapeNew() {
    return shapeNewArray(1);
}

shape *shapeNewArray(size_t n) {
    if(n == 0) return NULL;

    shape *shapes = malloc(sizeof(shape) * n);

    for(int k = 0; k < n; k++) {
        shape *sh = &shapes[k];

        sh->pivot.x = 0;
        sh->pivot.y = 0;
        sh->dim.w = 0;
        sh->dim.h = 0;
        sh->angle.phi = 0;

        sh->color = colors[COLOR_SHAPE];
        sh->color1 = colors[COLOR_SHAPE];
        sh->color2 = colors[COLOR_SHAPE];

        sh->draw = shapeDraw;
        sh->hit = NULL;
        sh->action = NULL;
        sh->texture = NULL;

        sh->subtextures = NULL;
        sh->subtexturesN = 0;

        sh->focused = 0;
        sh->inactive = 0;
    }

    return shapes;
}

void shapeSubtexNewArray(shape *sh, size_t n) {
    free(sh->subtextures);

    sh->subtextures = malloc(sizeof(texture *) * n);
    sh->subtexturesN = n;
}

void shapeDestroy(shape *sh) {
    shapeDestroyArray(sh, 1);
}

void shapeDestroyArray(shape *shapes, size_t n) {
    if(!shapes) return;

    for(int k = 0; k < n; k++) {
        shape *sh = &shapes[k];
        if(!sh) continue;

        free(sh->subtextures);
    }

    free(shapes);
}

void shapePrint(shape *sh) {
    if(!sh) return;

    printf("shape {\n");
    
    printf("\tpivot ");
    pointPrintChild(&sh->pivot);
    printf(",\n\tdim: {w: %g, h: %g}", sh->dim.w, sh->dim.h);
    printf(",\n\tangle: {phi: %g}", sh->angle.phi);
    printf(",\n\tcolor: [%g, %g, %g]", sh->color.r, sh->color.g, sh->color.b);
    printf("\n}\n");

    fflush(stdout);
}

void shapeDraw(shape *sh) {
    texture *tex = sh->focused && sh->subtexturesN ? 
        sh->subtextures[0] : sh->texture;

    glPushMatrix();

    glTranslatef(sh->pivot.x, sh->pivot.y, 0);
    glRotatef(sh->angle.phi, 0, 0, 1);

    color col;
    if(!sh->inactive) 
        col = sh->focused ? sh->color1 : sh->color;
    else
        col = sh->color2;
    

    glColor3f(col.r, col.g, col.b);

    if(!tex) { 
        rect(0, 0, sh->dim.w, sh->dim.h);
    } else {
        tex->mode = GL_MODULATE;
        texDrawRect(tex, 0, 0, sh->dim.w, sh->dim.h);
    }

    glPopMatrix();
}

char shapeHitCircleQ(shape *sh, point pnt) {
    if(!sh) return 0;

    double x = sh->pivot.x;
    double y = sh->pivot.y;
    double r = sh->dim.w / 2;

    double dx = pnt.x - x;
    double dy = pnt.y - y;

    return dx * dx + dy * dy < r * r;
}

char shapeHitRectQ(shape *sh, point pnt) {
    if(!sh) return 0;

    double x = sh->pivot.x;
    double y = sh->pivot.y;
    double w = sh->dim.w;
    double h = sh->dim.h; 

    double dx = pnt.x - x;
    double dy = pnt.y - y;

    return dx < w / 2 && dx > -w / 2 &&
           dy < h / 2 && dy > -h / 2;
}

void shapeFocus(shape *sh) {
    if(!sh) return;
    if(sh->inactive) return;

    sh->focused = 1;
}

void shapeBlur(shape *sh) {
    if(!sh) return;
    if(sh->inactive) return;

    sh->focused = 0;
}
