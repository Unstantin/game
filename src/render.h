#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>

#ifndef RENDER
#define RENDER

void renderDefault();
void renderSwitch(void (*draw)());

extern char drawFuncEnabled;
extern char axesEnabled;
extern char frameboxEnabled;
extern char snowflakeEnabled;

#endif 
