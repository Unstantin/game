#include "help-texture.h"

char *helpTexFilenames[] = {
    [HELP_TEX_SLIDE_MAIN_BACKGROUND] = "slide1.png",
    [HELP_TEX_PIECE_MOVEMENT_BACKGROUND] = "slide2.png",
    [HELP_TEX_WALL_PLACEMENT_BACKGROUND] = "slide3.png",
    [HELP_TEX_BUTTON_START] = "button-start.png",
    [HELP_TEX_BUTTON_START_ENTER] = "button-start-enter.png",
    [HELP_TEX_BUTTON_NEXT] = "button-next.png",
    [HELP_TEX_BUTTON_PREV] = "button-prev.png"
};

void helpTexInit(texture **textures) {
    for(int k = 0; k < HELP_TEX_N; k++) {
        texture *tex = texNew();
        tex->filename = helpTexFilenames[k];
        tex->mipmap = 1;

        int error = texInit(tex);
        if(error) continue;

        textures[k] = tex;
    }
}
