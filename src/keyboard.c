#include "keyboard.h"
#include "window.h"
#include "board.h"
#include "board-piece.h"
#include "board-piece-move.h"
#include "board-cell.h"
#include "board-wall.h"
#include "board-wall-float.h"
#include "board-wall-rand.h"
#include "board-wall-borderwall.h"
#include "cell.h"
#include "axes.h"
#include "render.h"
#include "scene.h"
#include "path.h"
#include "board-path.h"
#include "help.h"

#define KSKO (KEYBOARD_SPECIAL_KEY_OFFSET)

void (*keyboardReceiver)(int key) = NULL;

void keyboardSwitch(void (*receiver)(int key)) {
    keyboardReceiver = receiver;
}

void keyboardDefault(int key, int x, int y) {
    int modif = glutGetModifiers();
    char notFound = 0;

    if(modif == GLUT_ACTIVE_SHIFT) 
        switch(key) {
#ifdef GAME_DEV_MODE
            case GLUT_KEY_RIGHT + KSKO:
                glTranslatef(-AXES_STEP, 0, 0);
                break;

            case GLUT_KEY_LEFT + KSKO:
                glTranslatef(AXES_STEP, 0, 0);
                break;

            case GLUT_KEY_UP + KSKO:
                glTranslatef(0, -AXES_STEP, 0);

                break;
            case GLUT_KEY_DOWN + KSKO:
                glTranslatef(0, AXES_STEP, 0);
                break;

            case '+':
                glScalef(WINDOW_SCALE_FACTOR, WINDOW_SCALE_FACTOR, 1);
                break;

            case 'P':
                boardPathSearchAnimate();

                break;
#endif

            default:
                notFound = 1;
        }
    else if(modif == GLUT_ACTIVE_CTRL)
        switch(key) {
            default:
                notFound = 1;
        }
    else if(!modif)
        switch(key) {
            case 'q': 
                glutDestroyWindow(window.windowname);
                return;

            case GLUT_KEY_F11 + KSKO:
                windowFullScreenSwitch();
                break;

#ifdef GAME_DEV_MODE
            case 'd':
                drawFuncEnabled = !drawFuncEnabled;
                break;

            case 'a':
                axesEnabled = !axesEnabled;
                break;

            case 'f':
                frameboxEnabled = !frameboxEnabled;
                break;

            case 's':
                snowflakeEnabled = !snowflakeEnabled;
                break;

            case '\t':
                if(board.pieceActive->landing)
                    pieceLandingOff(board.pieceActive); 

                boardPieceActivateNext();
                break;

            case 'w':
                boardWallRandCreate();
                break;

            case 'c':
                boardCellColorReset();
                
                break;

            case '+':
                glScalef(WINDOW_SCALE_FACTOR, WINDOW_SCALE_FACTOR, 1);
                break;

            case '-':
                glScalef(1 / WINDOW_SCALE_FACTOR, 1 / WINDOW_SCALE_FACTOR, 1);
                break;

            case '[':
                glRotatef(WINDOW_ROTATE_STEP, 0, 0, 1);
                break;

            case ']':
                glRotatef(WINDOW_ROTATE_STEP, 0, 0, -1);
                break;

            case 'p':
                board.path->visible = !board.path->visible;
                boardWallFloatMouseMove();
                break;
#endif

            default:
                notFound = 1;
        }

    if(notFound && keyboardReceiver)
        keyboardReceiver(key);
    
    if(!notFound) 
        glutPostRedisplay();
}

/****************************************/

void keyboardReceiverScenePieceMove(int key) {
    int modif = glutGetModifiers();

    switch(key) {
        case GLUT_KEY_UP + KSKO: 
            boardPieceStep(DIR_UP);
            break;

        case GLUT_KEY_DOWN + KSKO: 
            boardPieceStep(DIR_DOWN);
            break;

        case GLUT_KEY_LEFT + KSKO: 
            boardPieceStep(DIR_LEFT);
            break;

        case GLUT_KEY_RIGHT + KSKO: 
            boardPieceStep(DIR_RIGHT);
            break;

        default: 
            return;
    }

    glutPostRedisplay();
}

void keyboardReceiverSceneWallFloat(int key) {
    int modif = glutGetModifiers();

    switch(key) {
        case 27:
            sceneChange(SCENE_PIECE_MOVE);
            break;

        case ' ':
            boardWallFloatTranspose();
            break; 

        default: 
            return;
    }

    glutPostRedisplay();
}

void keyboardReceiverScenePieceSelect(int key) {
    piece *p;

    switch(key) {
        case '\r':
            if(board.piecesSelectedN < BOARD_PIECES_SELECTED_N_MIN)
                return;

            sceneChange(SCENE_PIECE_MOVE); 
            break;

        case ' ':
            p = board.pieceActive;

            boardPieceSelectionSwitch(p);
            boardWallBorderWallColorFromPiece(p);
            break;

        case '\t':
            boardPieceActivateNext();
            break;

        default:
            return;
    }

    glutPostRedisplay();
}

void keyboardReceiverSceneHelp(int key) {
    switch(key) {
        case '\r':
            helpActionStart();
            break;

        case GLUT_KEY_RIGHT + KSKO:
            helpActionNextSlide();
            break;

        case GLUT_KEY_LEFT + KSKO:
            helpActionPrevSlide();
            break;
        
        default:
            return;
    }

    glutPostRedisplay();
}
