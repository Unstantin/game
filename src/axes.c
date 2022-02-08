#include "axes.h"
#include "polygon.h"
#include "window.h"

#define FRAMEBOX_WIDTH 0.025

#define AXES_WIDTH 0.02
#define AXES_TICK_W 0.0075
#define AXES_TICK_H 0.06

void framebox() {
    glColor3f(1, 1, 1);

    rect(0., (-WINDOW_INNER_H + FRAMEBOX_WIDTH) / 2, WINDOW_INNER_W, FRAMEBOX_WIDTH);
    rect((-WINDOW_INNER_W + FRAMEBOX_WIDTH) / 2, 0., FRAMEBOX_WIDTH, WINDOW_INNER_H);
    rect(0., (WINDOW_INNER_H - FRAMEBOX_WIDTH) / 2, WINDOW_INNER_W, FRAMEBOX_WIDTH);
    rect((WINDOW_INNER_W - FRAMEBOX_WIDTH) / 2, 0., FRAMEBOX_WIDTH, WINDOW_INNER_H);
}

void axes() {
    glColor3f(0, 1, 0);
    rect(0., 0., AXES_WIDTH, window.innerHeight);
    
    for(int i = 1; i < window.innerHeight / AXES_STEP / 2; i++) {
        rect(0.,  i * AXES_STEP, AXES_TICK_H, AXES_TICK_W);
        rect(0., -i * AXES_STEP, AXES_TICK_H, AXES_TICK_W);
    }


    glColor3f(1, 0, 0);
    rect(0., 0., window.innerWidth, AXES_WIDTH);

    for(int i = 1; i < window.innerWidth / AXES_STEP / 2; i++) {
        rect( i * AXES_STEP, 0., AXES_TICK_W, AXES_TICK_H);
        rect(-i * AXES_STEP, 0., AXES_TICK_W, AXES_TICK_H);
    }
}

