#include <stdio.h>
#include <stdlib.h>

#ifndef BOARD_NODE
#define BOARD_NODE

#include "node.h"

node *boardNodeClone(node *nd);
void boardNodeInit();
void boardNodeShapeApply(node *nd);
node *boardNodeCreate(node *nd);
void boardNodeJointApply(node *nd);

#endif
