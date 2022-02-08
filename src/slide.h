#include <stdio.h>
#include <stdlib.h>

#ifndef SLIDE
#define SLIDE

#include "shape.h"
#include "point.h"

enum slide_shapes {
    SLIDE_SHAPE_BACKGROUND,
    SLIDE_SHAPE_START,
    SLIDE_SHAPE_NEXT,
    SLIDE_SHAPE_PREV,
    SLIDE_SHAPES_N
};

typedef
struct slide_ {
    int slidename;
    shape *shapes;
} slide;

slide *slideNew();
void slideDestroy(slide *sl);
void drawSlide(slide *sl);
shape *slideHitTarget(slide *sl, point pnt);

#endif
