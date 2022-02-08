#include <stdio.h>
#include <stdlib.h>

#ifndef HELP
#define HELP

#include "slide.h"
#include "help-texture.h"
#include "point.h"

enum slide_names {
    SLIDE_MAIN,
    SLIDE_PIECE_MOVEMENT,
    SLIDE_WALL_PLACEMENT,
    SLIDES_N
};

struct help_ {
    texture *textures[HELP_TEX_N];
    slide *slides[SLIDES_N];
    slide *slideActive;
    shape *targetPrev;
};

void helpInit();
void drawHelp();
void helpActionStart();
void helpActionNextSlide();
void helpActionPrevSlide();
void helpSlideClick(point pnt);
char helpSlideHover(point pnt);

#endif
