#include <stdio.h>
#include <stdlib.h>

#ifndef SCENE
#define SCENE

typedef struct scene_ scene;
struct scene_ {
    void (*mouseReceiver)(int mode);
    void (*keyboardReceiver)(int key);
    void (*render)();
    void (*open)();
    void (*close)();
};

enum scene_names {SCENE_PIECE_MOVE, SCENE_WALL_FLOAT, 
                  SCENE_PIECE_SELECT, SCENE_HELP, 
                  SCENE_N};

void sceneInit();
void sceneChange(unsigned int scenename);

#endif
