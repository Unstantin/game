#include "timer.h"

#define TIMER_DELAY_DEFAULT 150

void timerFrameRequestTimer(int timername);
void timerDelayTimer(int timername);

long clockStartValue = 0;

char timerFrameRequestLocked = 0;
void (*timerFrameRequestCallback)() = NULL;

void (*timerDelayCallback)() = NULL;

void clockStart() {
    clockStartValue = clock(); 
}

void clockFinish() {
    printf("CPU time (us): %g\n", (double)(clock() - clockStartValue) / CLOCKS_PER_SEC * 1e6);
    fflush(stdout);
}

void timerFrameRequest(void (*callback)(void)) {
    timerFrameRequestCallback = callback;
    if(!timerFrameRequestLocked)
        timerFrameRequestTimer(0);
}

void timerFrameRequestTimer(int timername) {
    if(timerFrameRequestCallback) {
        timerFrameRequestCallback();

        timerFrameRequestCallback = NULL;
        timerFrameRequestLocked = 1;

        glutTimerFunc(1000 / FPS, timerFrameRequestTimer, 0);
    } else {
        timerFrameRequestLocked = 0;
    }
} 

void timerDelay(void (*callback)(void)) {
    timerDelayCallback = callback;
    glutTimerFunc(TIMER_DELAY_DEFAULT, timerDelayTimer, 0);
}

void timerDelayTimer(int timername) {
    if(timerDelayCallback)
        timerDelayCallback(); 
}

