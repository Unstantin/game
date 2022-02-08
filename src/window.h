#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>

#ifndef WINDOW
#define WINDOW

#include "texture.h"

#define WINDOW_W 500
#define WINDOW_H 500
#define WINDOW_INNER_W 2.
#define WINDOW_INNER_H 2.

#define WINDOW_SCALE_FACTOR 1.1
#define WINDOW_ROTATE_STEP 1

extern struct window_ {
    int windowname;
    float innerWidth;
    float innerHeight;
    int windowModeWidth;
    int windowModeHeight;
    int width;
    int height;
    texture *backgroundTexture;
    char fullscreen;
} window;


void windowInit();
void windowReshape(int w, int h);
void windowDrawBackground();
void windowFullScreenSwitch();

#endif
