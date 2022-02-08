#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>

#ifndef KEYBOARD
#define KEYBOARD

#define KEYBOARD_SPECIAL_KEY_OFFSET 256

void keyboardDefault(int key, int x, int y);
void keyboardReceiverScenePieceMove(int key);
void keyboardReceiverSceneWallFloat(int key);
void keyboardReceiverScenePieceSelect(int key);
void keyboardReceiverSceneHelp(int key);
void keyboardSwitch(void (*receiver)(int key));
void boardPieceActiveFirstEnabled();

#endif
