#include <stdio.h>
#include <stdlib.h>

#ifndef SHAPE
#define SHAPE

#include "color.h"
#include "point.h"
#include "texture.h"

typedef
struct {
    double w, h;
} dim;

typedef
struct {
    double phi;
} angle;

typedef struct shape_ shape;
struct shape_ {
    pivot pivot;
    dim dim; 
    color color;
    color color1;
    color color2;
    angle angle;
    void (*draw)(shape *sh);
    char (*hit)(shape *sh, point pnt);
    void (*action)();
    void *object;
    texture *texture;
    texture **subtextures;
    int subtexturesN;
    char focused;
    char inactive;
};

shape *shapeNew();
shape *shapeNewArray(size_t n);
void shapeDraw(shape *sh);
void shapeSubtexNewArray(shape *sh, size_t n);
void shapeDestroy(shape *sh);
void shapeDestroyArray(shape *shapes, size_t n);
void shapePrint(shape *sh);
char shapeHitCircleQ(shape *sh, point pnt);
char shapeHitRectQ(shape *sh, point pnt);
void shapeFocus(shape *sh);
void shapeBlur(shape *sh);

#endif

