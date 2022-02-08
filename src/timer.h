#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <time.h>

#ifndef TIMER
#define TIMER

#define FPS 60

void clockStart();
void clockFinish();
void timerFrameRequest(void (*callback)(void));
void timerDelay(void (*callback)(void));

#endif
