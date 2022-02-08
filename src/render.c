#include "render.h"
#include "board.h"
#include "axes.h"
#include "snowflake.h"
#include "window.h"

char drawFuncEnabled = 1;
char axesEnabled = 0;
char frameboxEnabled = 0;
char snowflakeEnabled = 0;

void (*drawFunc)() = NULL;

void renderSwitch(void (*draw)()) {
    drawFunc = draw;
}

void renderDefault() {
    windowDrawBackground();

    if(drawFuncEnabled) 
        drawFunc();

    if(axesEnabled)
        axes();

    if(frameboxEnabled)
        framebox();

    if(snowflakeEnabled)
        snowflake();
}
