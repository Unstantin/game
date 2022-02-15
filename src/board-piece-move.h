#include <stdio.h>
#include <stdlib.h>

#ifndef BOARD_PIECE_MOVE
#define BOARD_PIECE_MOVE

#include "cell.h"
#include "dir.h"
#include "piece.h"

void boardPieceStep(unsigned int dirname);
void boardPieceActiveJumpToCell(cell *c);
char boardPieceActiveJump(dir d);
char boardPieceActiveJumpAny(dir d);
char boardPieceActiveMoveSingle(dir d);
char boardPieceActiveMove(dir d);
char boardPieceActiveMoveRecursive(dir d);
char boardPieceActiveMoveRecursiveStep(cell *c, dir d);
char boardPieceActiveLandingJump(dir d);
void boardPieceJump(piece *p, cell *c);

#endif
