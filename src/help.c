#include "help.h"
#include "scene.h"
#include "board.h"
#include "shape.h"

#define HELP_SLIDE_BUTTON_START_X 0.66
#define HELP_SLIDE_BUTTON_START_Y -0.75
#define HELP_SLIDE_BUTTON_START_D 0.175
#define HELP_SLIDE_BUTTON_D_FACTOR 0.6
#define HELP_SLIDE_BUTTON_OFFSET_FACTOR 0.87 

void helpSlideInit();
void helpSlideTextureInit(slide *sl);
void helpSlideLayoutInit(slide *sl);
void helpSlideShapeHitInit(slide *sl);
slide *helpSlideFindByOffsetCyclic(slide *sl0, int offset);
slide *helpSlideFindByOffset(slide *sl, int offset);

enum help_tex_names helpSlideBackground2TextureName[] = {
    [SLIDE_MAIN] = HELP_TEX_SLIDE_MAIN_BACKGROUND,
    [SLIDE_PIECE_MOVEMENT] = HELP_TEX_PIECE_MOVEMENT_BACKGROUND,
    [SLIDE_WALL_PLACEMENT] = HELP_TEX_WALL_PLACEMENT_BACKGROUND
};

enum slide_buttons {
    SLIDE_BUTTON_START,
    SLIDE_BUTTON_NEXT,
    SLIDE_BUTTON_PREV,
    SLIDE_BUTTONS_N
};

enum slide_buttons helpSlideButton2TextureName[] = {
    [SLIDE_BUTTON_START] = HELP_TEX_BUTTON_START,
    [SLIDE_BUTTON_NEXT] = HELP_TEX_BUTTON_NEXT,
    [SLIDE_BUTTON_PREV] = HELP_TEX_BUTTON_PREV
};

enum slide_shapes helpSlideButton2ShapeName[] = {
    [SLIDE_BUTTON_PREV] = SLIDE_SHAPE_PREV,
    [SLIDE_BUTTON_NEXT] = SLIDE_SHAPE_NEXT,
    [SLIDE_BUTTON_START] = SLIDE_SHAPE_START
};

enum slide_shape_start_subtex_names {
    SLIDE_SHAPE_START_ENTER_SUBTEX,
    SLIDE_SHAPE_START_SUBTEX_N
};

/***************************************/

struct help_ help = {
    .textures = {NULL},
    .slides = {NULL},
    .slideActive = NULL,
    .targetPrev = NULL
};

void helpInit() {
    helpTexInit(help.textures);
    helpSlideInit();
}

void helpSlideInit() {
    for(int k = 0; k < SLIDES_N; k++) {
        slide *sl = slideNew();
        help.slides[k] = sl;
        sl->slidename = k;
        
        helpSlideTextureInit(sl);
        helpSlideLayoutInit(sl);
        helpSlideShapeHitInit(sl);
    }

    shape *shprev = 
        &help.slides[0]->shapes[SLIDE_SHAPE_PREV]; 
    shprev->inactive = 1;

    shape *shnext = 
        &help.slides[SLIDES_N - 1]->shapes[SLIDE_SHAPE_NEXT]; 
    shnext->inactive = 1;

    help.slideActive = help.slides[SLIDE_MAIN];
}

void helpSlideTextureInit(slide *sl) {
    shape *sh = &sl->shapes[SLIDE_SHAPE_BACKGROUND];
    sh->texture = help.textures[helpSlideBackground2TextureName[sl->slidename]];

    for(int k = 0; k < SLIDE_BUTTONS_N; k++) {
        shape *sh = &sl->shapes[helpSlideButton2ShapeName[k]];
        sh->texture = help.textures[helpSlideButton2TextureName[k]];
    }

    shape *shstart = &sl->shapes[SLIDE_SHAPE_START];
    shapeSubtexNewArray(shstart, SLIDE_SHAPE_START_SUBTEX_N);

    shstart->subtextures[SLIDE_SHAPE_START_ENTER_SUBTEX] = 
        help.textures[HELP_TEX_BUTTON_START_ENTER];
}

void helpSlideLayoutInit(slide *sl) {
    shape *sh = &sl->shapes[SLIDE_SHAPE_BACKGROUND];
    
    sh->pivot.x = board.x;
    sh->pivot.y = board.y;
    sh->dim.w = board.w;
    sh->dim.h = board.h;

    shape *shstart = &sl->shapes[SLIDE_SHAPE_START];

    shstart->pivot.x = HELP_SLIDE_BUTTON_START_X;
    shstart->pivot.y = HELP_SLIDE_BUTTON_START_Y;

    shstart->dim.w = HELP_SLIDE_BUTTON_START_D;
    shstart->dim.h = shstart->dim.w;

    shape *shnext = &sl->shapes[SLIDE_SHAPE_NEXT];
    
    shnext->pivot.x = shstart->pivot.x 
        + HELP_SLIDE_BUTTON_OFFSET_FACTOR * shstart->dim.w;
    shnext->pivot.y = shstart->pivot.y;

    shnext->dim.w = shstart->dim.w 
        * HELP_SLIDE_BUTTON_D_FACTOR;
    shnext->dim.h = shnext->dim.w;

    shape *shprev = &sl->shapes[SLIDE_SHAPE_PREV];

    shprev->pivot.x = shstart->pivot.x 
        - HELP_SLIDE_BUTTON_OFFSET_FACTOR * shstart->dim.w;
    shprev->pivot.y = shstart->pivot.y;

    shprev->dim.w = shstart->dim.w 
        * HELP_SLIDE_BUTTON_D_FACTOR;
    shprev->dim.h = shprev->dim.w;
}

void helpSlideShapeHitInit(slide *sl) {
    shape *shbg = &sl->shapes[SLIDE_SHAPE_BACKGROUND];
    shbg->hit = NULL;

    for(int k = 0; k < SLIDE_BUTTONS_N; k++) {
        shape *sh = &sl->shapes[helpSlideButton2ShapeName[k]];
        sh->hit = shapeHitCircleQ;
    }

    shape *shstart = &sl->shapes[SLIDE_SHAPE_START];
    shape *shnext = &sl->shapes[SLIDE_SHAPE_NEXT];
    shape *shprev = &sl->shapes[SLIDE_SHAPE_PREV];

    shstart->action = helpActionStart;
    shnext->action = helpActionNextSlide;
    shprev->action = helpActionPrevSlide;
}

void drawHelp() {
    drawSlide(help.slideActive); 
}

void helpActionStart() {
    sceneChange(SCENE_PIECE_SELECT);
}

void helpActionNextSlide() {
    slide *sl = help.slideActive;
//    slide *sl1 = helpSlideFindByOffsetCyclic(sl, 1);
    slide *sl1 = helpSlideFindByOffset(sl, 1);

    if(!sl1) return;
    help.slideActive = sl1;

    if(help.targetPrev) shapeBlur(help.targetPrev);
    help.targetPrev = NULL;
}

void helpActionPrevSlide() {
    slide *sl = help.slideActive;
//    slide *sl1 = helpSlideFindByOffsetCyclic(sl, -1);
    slide *sl1 = helpSlideFindByOffset(sl, -1);

    if(!sl1) return;
    help.slideActive = sl1;

    if(help.targetPrev) shapeBlur(help.targetPrev);
    help.targetPrev = NULL;
}

slide *helpSlideFindByOffsetCyclic(slide *sl, int offset) {
    if(!sl) return NULL;

    int pos = sl->slidename + offset;
    int n = SLIDES_N;
    int pos1 = (pos % n + n) % n;

    return help.slides[pos1];
}

slide *helpSlideFindByOffset(slide *sl, int offset) {
    if(!sl) return NULL;

    int pos = sl->slidename + offset;
    int n = SLIDES_N;
    if(pos >= n || pos < 0) {
        return NULL;
    } else {
        return help.slides[pos];
    };
}

void helpSlideClick(point pnt) {
    slide *sl = help.slideActive;

    shape *target = slideHitTarget(sl, pnt); 
    if(!target) return;

    if(target->action) target->action();
}

char helpSlideHover(point pnt) {
    slide *sl = help.slideActive;

    shape *target = slideHitTarget(sl, pnt); 

    if(target == help.targetPrev) return 0;

    if(help.targetPrev) shapeBlur(help.targetPrev);
    if(target) shapeFocus(target);
    
    help.targetPrev = target;

    return 1;
}
