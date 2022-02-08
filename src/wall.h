#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>

#ifndef WALL
#define WALL

#include "shape.h" 
#include "node.h"

typedef struct wall_ wall; 
struct wall_ {
    node begin, end;
    int lengthW, lengthH;
    shape *shape;
};

wall *wallNew();
void wallDestroy(wall *w);
void wallDraw(shape *sh);
void wallPrint(wall *w);

#endif
