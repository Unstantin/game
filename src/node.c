#include "node.h"
#include "game.h"
#include "polygon.h"
#include "texture.h"

void nodeDraw(shape *sh);
shape *nodeShapeNew(node *nd);

node *nodeNew() {
    node *nd = malloc(sizeof(node));

    nd->i = 0;
    nd->j = 0;
    nd->joint = 0;
    nd->jointX = 0;
    nd->jointY = 0;
    nd->enabled = 0;

    nd->shape = nodeShapeNew(nd);

    return nd;
}

node *nodeClone(node *nd) {
    if(!nd) return NULL;

    node *nd1 = malloc(sizeof(node));
    
    *nd1 = *nd;

    nd1->shape = nodeShapeNew(nd1);

    return nd1;
}

void nodeDestroy(node *nd) {
    if(!nd) return; 

    shapeDestroy(nd->shape);
    free(nd);
}

shape *nodeShapeNew(node *nd) {
    shape *sh = shapeNew();

    sh->color = colors[COLOR_NODE];
    sh->draw = nodeDraw;
    sh->object = nd;

    return sh;
}

void nodePrint(node *nd) {
    nodePrintTree(nd, PRINT_ROOT); 
}

void nodePrintChild(node *nd) {
    nodePrintTree(nd, PRINT_CHILD);
}

void nodePrintTree(node *nd, char root) {    
    if(!nd) return;

    if(root) 
        printf("node ");
    else 
        printf("<node>: ");

    printf("{i: %d, j: %d, joint: %d}", nd->i, nd->j, nd->joint);
    if(root) printf("\n");

    fflush(stdout);
}

void nodeDraw(shape *sh) {
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
        tex->mode = GL_MODULATE;
        texDrawRect(tex, 0, 0, w, h);
    }

    glPopMatrix();
}
