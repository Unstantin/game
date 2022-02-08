#include "mouse.h"
#include "window.h"
#include "board.h"
#include "board-piece.h"
#include "board-piece-move.h"
#include "board-wall-float.h"
#include "cell.h"
#include "shape.h"
#include "color.h"
#include "node.h"
#include "scene.h"
#include "wall.h"
#include "board-path.h"
#include "timer.h"
#include "help.h"

point mouseCoords = {0};

void (*mouseReceiver)(int mode) = mouseReceiverDefault;

void mouseSwitch(void (*receiver)(int mode)) {
    mouseReceiver = receiver; 
}

void mouse(int mode, int x, int y) {
//    printf("mouse: %d\n", mode);
//    fflush(stdout);
    
    double upp = window.innerWidth / window.width;

    double x1 = upp * x - 0.5 * window.innerWidth;
    double y1 = -(upp * y - 0.5 * window.innerHeight);

    mouseCoords.x = x1;
    mouseCoords.y = y1;

    mouseReceiver(mode);
}

void mouseReceiverDefault(int mode) {
//    printf("mouseReceiverDefault: %3d % f % f\n", mode, mouseCoords.x, mouseCoords.y);
//    fflush(stdout);

    switch(mode) {
        case MOUSE_MODE(GLUT_LEFT_BUTTON, GLUT_DOWN, MOUSE_EVENT_CLICK):
            break;
        case MOUSE_MODE(GLUT_RIGHT_BUTTON, GLUT_DOWN, MOUSE_EVENT_CLICK):
            break;
        case MOUSE_MODE_DRAG:
//            printf("drag\n");
//            fflush(stdout);
            break;
        case MOUSE_MODE_MOVE:
//            printf("move\n");
//            fflush(stdout);
            break;
    }
    
    glutPostRedisplay();
}
void mouseReceiverScenePieceMove(int mode) {
//    printf("mouseReceiverDefault: %3d % f % f\n", mode, mouseCoords.x, mouseCoords.y);
//    fflush(stdout);

    cell *c;
    piece *p;
    shape *sh;
    char hit;
    int i, j;

    switch(mode) {
        case MOUSE_MODE(GLUT_LEFT_BUTTON, GLUT_DOWN, MOUSE_EVENT_CLICK):
            hit = point2cell(mouseCoords, &c);
            piece *p = board.pieceActive;
            if(hit) {
                char success = boardPieceActiveJumpToCell(c);
                if(success) {
#ifdef GAME_DEV_MODE 
                    board.pieceActive->stepCount++;
#else
                    boardPieceActivateNext();
#endif
                }
            }

            break;
        case MOUSE_MODE(GLUT_RIGHT_BUTTON, GLUT_DOWN, MOUSE_EVENT_CLICK):
            if(!board.pieceActive->landing) {
                sceneChange(SCENE_WALL_FLOAT);
            }
            break;
        case MOUSE_MODE_DRAG:
//            printf("drag\n");
//            fflush(stdout);
            return;
        case MOUSE_MODE_MOVE:
//            printf("move\n");
//            fflush(stdout);
            return;
    }
    
    timerFrameRequest(glutPostRedisplay);
}

void mouseReceiverSceneWallFloat(int mode) {
//    printf("mouseReceiverDefault: %3d % f % f\n", mode, mouseCoords.x, mouseCoords.y);
//    fflush(stdout);

    char success;

    switch(mode) {
        case MOUSE_MODE(GLUT_LEFT_BUTTON, GLUT_UP, MOUSE_EVENT_CLICK):
            success = boardWallFloatPlace();
            if(success) {
                sceneChange(SCENE_PIECE_MOVE);
                boardPieceActivateNext();
            }

            break;

        case MOUSE_MODE(GLUT_RIGHT_BUTTON, GLUT_DOWN, MOUSE_EVENT_CLICK):
            boardWallFloatTranspose();
            break;

        case MOUSE_MODE(MOUSE_WHEEL_UP, GLUT_DOWN, MOUSE_EVENT_CLICK):
            boardWallFloatResize(BOARD_WALL_FLOAT_LENGTH_DELTA);
//            printf("wheelup\n");
//            fflush(stdout);
            break;

        case MOUSE_MODE(MOUSE_WHEEL_DOWN, GLUT_DOWN, MOUSE_EVENT_CLICK):
            boardWallFloatResize(-BOARD_WALL_FLOAT_LENGTH_DELTA);
//            printf("wheeldown\n");
//            fflush(stdout);
            break;

        case MOUSE_MODE_DRAG:
        case MOUSE_MODE_MOVE:
            boardWallFloatMouseMove();
//            printf("move\n");
//            fflush(stdout);
            break;
    }

    timerFrameRequest(glutPostRedisplay);
}

void mouseReceiverScenePieceSelect(int mode) {
    piece *p;
    switch(mode) {
        case MOUSE_MODE(GLUT_LEFT_BUTTON, GLUT_UP, MOUSE_EVENT_CLICK):
            if(point2piece(mouseCoords, &p)) {
                boardPieceSelectionSwitch(p);
                boardWallBorderWallColorFromPiece(p);
            }
            break;

        case MOUSE_MODE_MOVE:
        case MOUSE_MODE_DRAG:
            return;
    }

    timerFrameRequest(glutPostRedisplay);
}

void mouseReceiverSceneHelp(int mode) {
    switch(mode) {
        case MOUSE_MODE(GLUT_LEFT_BUTTON, GLUT_UP, MOUSE_EVENT_CLICK):
//            printf("mousecoords: %g, %g\n", mouseCoords.x, mouseCoords.y);
//            fflush(stdout);

            helpSlideClick(mouseCoords);
            break;

        case MOUSE_MODE_MOVE:
        case MOUSE_MODE_DRAG: {
            char result = helpSlideHover(mouseCoords);
            if(!result) return;
            break;
        }
    }

    timerFrameRequest(glutPostRedisplay);
}
