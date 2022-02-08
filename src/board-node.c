#include "board-node.h"
#include "board.h"

#define BOARD_NODE_TEX_FILENAME "node.png"

void boardNodeTextureInit();

node *boardNodeClone(node *nd) {
    node *nd1 = nodeClone(nd);

    shape *sh = nd1->shape;

    sh->texture = board.nodeTexture;

    return nd1;
}

void boardNodeInit() {
    boardNodeTextureInit();
}

void boardNodeTextureInit() {
    texture *tex = texNew();
    tex->filename = BOARD_NODE_TEX_FILENAME;

    int error = texInit(tex);
    if(error) return;

    board.nodeTexture = tex;
}

void boardNodeShapeApply(node *nd) {
    shape *sh = nd->shape;

    node2pivot(*nd, &sh->pivot);

    sh->dim.w = board.spaceX; 
    sh->dim.h = board.spaceY;
}

node *boardNodeCreate(node *nd) {
    node *nd1 = boardNodeClone(nd);

    int i = nd1->i;
    int j = nd1->j;

    board.nodes[(board.m + 1) * i + j] = nd1;

    boardNodeShapeApply(nd1);

    return nd1;
}

void boardNodeJointApply(node *nd) {
    if(nd->jointX && nd->jointY) 
        nd->enabled = 1;
}
