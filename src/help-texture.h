#include <stdio.h>
#include <stdlib.h>

#ifndef HELP_TEXTURE
#define HELP_TEXTURE

#include <GL/glut.h>

#include "texture.h"

enum help_tex_names {
    HELP_TEX_SLIDE_MAIN_BACKGROUND,
    HELP_TEX_PIECE_MOVEMENT_BACKGROUND,
    HELP_TEX_WALL_PLACEMENT_BACKGROUND,
    HELP_TEX_BUTTON_START,
    HELP_TEX_BUTTON_START_ENTER,
    HELP_TEX_BUTTON_NEXT,
    HELP_TEX_BUTTON_PREV,
    HELP_TEX_N
};

void helpTexInit(texture **textures);

#endif
