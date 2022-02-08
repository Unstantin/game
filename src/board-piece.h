#include <stdio.h>
#include <stdlib.h>

#ifndef BOARD_PIECE
#define BOARD_PIECE

#include "cell.h"
#include "piece.h"

#define BOARD_PIECES_N 4 
#define BOARD_PIECES_SELECTED_N_MIN 2

void boardPieceInit();
char boardPieceActivate(piece *p);
void boardPieceActivateNext();
char boardPieceOppositeSideReachedQ(piece *p, cell *c);
char boardPieceJumpPossibleQ(piece *p, cell *c);
char boardPieceHitQ(piece *p, point pnt);
void boardPieceShapeXYApply(piece *p);
char boardPieceSelectionSwitch(piece *p);
void boardPieceSelectedToEnabled();
void boardPieceRemoveDisableOthers(piece *p);
void boardPieceRemoveDisable(piece *p);
char boardPieceLandingPossibleQ(piece *p, cell *c);

#endif
