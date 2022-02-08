#include "window.h"
#include "board.h"
#include "color.h"

void aspect(double w, double h, double w0, double h0, double *pw1, double *ph1);

struct window_ window = {
    .innerWidth = WINDOW_INNER_W,
    .innerHeight = WINDOW_INNER_H,
    .windowModeWidth = WINDOW_W,
    .windowModeHeight = WINDOW_H,
    .width = WINDOW_W,
    .height = WINDOW_H,
    .backgroundTexture = NULL,
    .fullscreen = 0
};

void windowInit() {
    texture *tex = texNew();
    tex->filename = "background.png";

    int error = texInit(tex);
    if(error) return;

    window.backgroundTexture = tex;
} 

void aspect(double w, double h, double w0, double h0, double *pw1, double *ph1) {
    double asp = (double)h / w;
    double asp0 = (double)h0 / w0;

    double w1, h1;

    if(asp > asp0) {
        w1 = w0;
        h1 = w0 * asp;
    } else {
        w1 = h0 / asp;
        h1 = h0;
    }

    *pw1 = w1;
    *ph1 = h1;
}

void windowReshape(int w, int h) {
    glViewport(0, 0, w, h);

    if(w == 0) w = 1;
    if(h == 0) h = 1;
    
    window.width = w;
    window.height = h;

    if(!window.fullscreen) {
        window.windowModeWidth = w;
        window.windowModeHeight = h;
    }

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float w0 = WINDOW_INNER_W;
    float h0 = WINDOW_INNER_H;

    double w1, h1;

    aspect(w, h, w0, h0, &w1, &h1);

    glOrtho(-w1 / 2, w1 / 2, -h1 / 2, h1 / 2, -1, 1);
//        glScalef(2 / w1, 2 / h1, 1);
    window.innerWidth = w1;
    window.innerHeight = h1;

    glMatrixMode(GL_MODELVIEW);
}

void windowDrawBackground() {
    texture *tex = window.backgroundTexture;
    if(!tex) return;

    float x = 0;
    float y = 0;
    float w = window.innerWidth;
    float h = window.innerHeight;
    float w0 = tex->width;
    float h0 = tex->height;

    double w1, h1;

    aspect(w0, h0, w, h, &w1, &h1);
    
    setColor(COLOR_BOARD_BACKGROUND);
    tex->mode = GL_MODULATE;
    texDrawRect(tex, x, y, w1, h1);
}

void windowFullScreenSwitch() {
    window.fullscreen = !window.fullscreen;

    if(!window.fullscreen) 
        glutReshapeWindow(window.windowModeWidth, window.windowModeHeight);
    else
        glutFullScreen();
}
