#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>

#ifndef MOUSE
#define MOUSE

#include "point.h"
    
#define MOUSE_STATE_SHIFT 4
#define MOUSE_EVENT_SHIFT 5

#define MOUSE_WHEEL_UP 3
#define MOUSE_WHEEL_DOWN 4

#define MOUSE_MODE(button, state, event) \
            (button) | (state) << MOUSE_STATE_SHIFT \
            | (event) << MOUSE_EVENT_SHIFT  

#define MOUSE_MODE_DRAG (MOUSE_MODE(0, 0, MOUSE_EVENT_DRAG))
#define MOUSE_MODE_MOVE (MOUSE_MODE(0, 0, MOUSE_EVENT_MOVE))

enum mouse_event {MOUSE_EVENT_CLICK, MOUSE_EVENT_DRAG, MOUSE_EVENT_MOVE};

void mouse(int mode, int x, int y);
void mouseReceiverDefault(int mode);
void mouseReceiverScenePieceMove(int mode);
void mouseReceiverSceneWallFloat(int mode);
void mouseReceiverScenePieceSelect(int mode);
void mouseReceiverSceneHelp(int mode);
void mouseSwitch(void (*receiver)(int mode));

extern point mouseCoords;

#endif
