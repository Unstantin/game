#include <stdio.h>
#include <stdlib.h>

#ifndef TEXTURE 
#define TEXTURE

typedef struct texture_ texture;
struct texture_ {
    int texname;
    unsigned int width;
    unsigned int height;
    char *filename;
    GLenum mode;
    int orientation;
    char mipmap;
};

enum tex_orientation {TEX_IDENTITY, TEX_ROTATE_CW_90,
                      TEX_ROTATE_CCW_90, TEX_ROTATE_180,
                      TEX_FLIP_X, TEX_FLIP_Y};

texture *texNew();
int texInit(texture *tex);
void texDrawRect(texture *tex, double x, double y, double w, double h);
void texDestroy(texture *tex);

#endif
