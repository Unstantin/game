#include "slide.h"
#include "color.h"
#include "texture.h"

slide *slideNew() {
    slide *sl = malloc(sizeof(slide));

    sl->slidename = 0;
    sl->shapes = shapeNewArray(SLIDE_SHAPES_N);

    for(int k = 0; k < SLIDE_SHAPES_N; k++) {
        shape *sh = &sl->shapes[k];

        sh->object = sl; 
        sh->color = colors[COLOR_SLIDE];
        sh->color1 = colors[COLOR_SLIDE_BUTTON_FOCUSED];
        sh->color2 = colors[COLOR_SLIDE_BUTTON_INACTIVE];
    }

    return sl;
}

void slideDestroy(slide *sl) {
    if(!sl) return;

    shapeDestroyArray(sl->shapes, SLIDE_SHAPES_N);
    free(sl);
}

void drawSlide(slide *sl) {
    for(int k = 0; k < SLIDE_SHAPES_N; k++) {
        shape *sh = &sl->shapes[k];
        sh->draw(sh);
    }
}

shape *slideHitTarget(slide *sl, point pnt) {
    for(int k = SLIDE_SHAPES_N - 1; k >= 0; k--) {
        shape *sh = &sl->shapes[k];

        if(!sh || !sh->hit) continue; 
        if(sh->hit(sh, pnt)) {
            return sh;
        }
    }

    return NULL;
}
