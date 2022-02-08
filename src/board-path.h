#include <stdio.h>
#include <stdlib.h>

#ifndef BOARD_PATH
#define BOARD_PATH

#include "piece.h"

extern struct board_path_ {
    long stepCount;
    char visible;
    piece *pieceAnimatedFirst; 
} board_path;

char boardPathToOppositeSideExistsQ(piece *p);
char boardPassableQ();    
void boardPathSearchAnimate();

#endif
