#include <stdio.h>
#include <stdlib.h>

#ifndef NODE
#define NODE

#include "shape.h"

typedef
struct node_ {
    int i, j;
    char joint;
    char jointX;
    char jointY;
    char enabled;
    shape *shape;
} node;

node *nodeNew();
node *nodeClone(node *nd);
void nodeDestroy(node *nd);
void nodePrint(node *nd);
void nodePrintChild(node *nd);
void nodePrintTree(node *nd, char root);


#endif
