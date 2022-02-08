#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <GL/glew.h>
#include <GL/glut.h>

#include "game.h"
#include "window.h"
#include "snowflake.h"
#include "axes.h"
#include "board.h"
#include "keyboard.h"
#include "render.h"
#include "mouse.h"
#include "scene.h"
#include "color.h"
#include "timer.h"
#include "texture.h"
#include "help.h"

void reshape(int w, int h);
void display(void);
void keyboard(unsigned char key, int x, int y);
void special(int key, int x, int y);
void mouseclick(int button, int state, int x, int y);
void mousedrag(int x, int y);
void mousemove(int x, int y);

void init();


void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);
    
//    clockStart();
    renderDefault();

    glutSwapBuffers();
//    clockFinish();

//    printf("display\n");
//    fflush(stdout);
}

void reshape(int w, int h) {
    windowReshape(w, h);

//    printf("reshape\n");
//    fflush(stdout);
}

void keyboard(unsigned char key, int x, int y) {
    printf("keyboard: %d\n", key);
    fflush(stdout);

    keyboardDefault(key, x, y);
}

void special(int key, int x, int y) {
    printf("special: %d\n", key);
    fflush(stdout);
    
    keyboardDefault(key + KEYBOARD_SPECIAL_KEY_OFFSET, x, y);
}

void mouseclick(int button, int state, int x, int y) {
//    printf("mouseclick: %3d %3d %3d %3d\n", button, state, x, y); 
//    fflush(stdout);

    mouse(MOUSE_MODE(button, state, MOUSE_EVENT_CLICK), x, y);
}

void mousedrag(int x, int y) {
//    printf("mousedrag: %3d %3d\n", x, y);
//    fflush(stdout);

    mouse(MOUSE_MODE(0, 0, MOUSE_EVENT_DRAG), x, y);
}

void mousemove(int x, int y) {
//    printf("mousemove: %3d %3d\n", x, y);
//    fflush(stdout);

    mouse(MOUSE_MODE(0, 0, MOUSE_EVENT_MOVE), x, y);
}

void init() {
    color *clr = &colors[COLOR_BOARD_BACKGROUND];

    glClearColor(clr->r, clr->g, clr->b, 1);
    
    srand(time(0));
    
    windowInit();

    sceneInit();
    boardInit();
    helpInit();
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_MULTISAMPLE);

    glutInitWindowSize(WINDOW_W, WINDOW_H);
    window.windowname = glutCreateWindow(GAME_TITLE);
    
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special);
    glutMouseFunc(mouseclick);
    glutMotionFunc(mousedrag);
    glutPassiveMotionFunc(mousemove);
    
    int error = glewInit();
    if(error) {
        printf("GLEW failed\n");
        return 1;
    }

    init();

    glutMainLoop();
}
