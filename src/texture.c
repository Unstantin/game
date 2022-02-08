#include <GL/glew.h>
#include <GL/glut.h>

#include "texture.h"
#include "lodepng.h"
#include "window.h"

#define PATHNAME_SIZE 256
#define TEX_IMAGE_DIR "img"

int texLoadFileRGBA(texture *tex, unsigned char **pimage);
void texLoadImage(texture *tex, unsigned char *image, GLenum format);
void texSetOrientation(int orientation);

texture *texNew() {
    texture *tex = malloc(sizeof(texture));

    tex->texname = 0;
    tex->width = 0;
    tex->height = 0;
    tex->filename = NULL;
    tex->mode = GL_REPLACE;
    tex->orientation = TEX_IDENTITY;
    tex->mipmap = 0;

    return tex;
}

void texDestroy(texture *tex) {
    glDeleteTextures(1, &tex->texname);
    free(tex); 
}

int texInit(texture *tex) {
    unsigned char *image = NULL;

    int error = texLoadFileRGBA(tex, &image);
    if(error) return error;

    texLoadImage(tex, image, GL_RGBA);

    free(image);

    return 0;
}

int texLoadFileRGBA(texture *tex, unsigned char **pimage) {
    if(!tex->filename) {
        printf("texLoadFileRGBA ERROR: empty filename\n");
        fflush(stdout);
        return 1;
    }

    char pathname[PATHNAME_SIZE];

    sprintf(pathname, "%s/%s", TEX_IMAGE_DIR, tex->filename);

    int error = lodepng_decode32_file(pimage, &tex->width, &tex->height, pathname);
    if(error) {
        printf("texLoadFileRGBA ERROR: [%s]: ", pathname);
        printf("%s\n", lodepng_error_text(error));

        fflush(stdout);

        return error;
    }

    return 0;
}

void texLoadImage(texture *tex, unsigned char *image, GLenum format) {
    glGenTextures(1, &tex->texname);
    glBindTexture(GL_TEXTURE_2D, tex->texname);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if(tex->mipmap) 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    else
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

    glTexImage2D(GL_TEXTURE_2D, 0, format, tex->width, tex->height, 0,
                    format, GL_UNSIGNED_BYTE, image);

    if(tex->mipmap)
        glGenerateMipmap(GL_TEXTURE_2D);
}


void texDrawRect(texture *tex, double x, double y, double w, double h) {
    if(!tex) return;

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glBindTexture(GL_TEXTURE_2D, tex->texname);

    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, tex->mode);

    texSetOrientation(tex->orientation);

    glBegin(GL_POLYGON);
        glTexCoord2f(0, 1);
        glVertex3f(x - w / 2, y - h / 2, 0);

        glTexCoord2f(0, 0);
        glVertex3f(x - w / 2, y + h / 2, 0);

        glTexCoord2f(1, 0);
        glVertex3f(x + w / 2, y + h / 2, 0);
        
        glTexCoord2f(1, 1);
        glVertex3f(x + w / 2, y - h / 2, 0);
    glEnd();

    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);
}

void texSetOrientation(int orientation) {
    double angle = 0;
    double scaleX = 1;
    double scaleY = 1;
    switch(orientation) {
        case TEX_ROTATE_CW_90:
            angle = -90;
            break;
        case TEX_ROTATE_CCW_90: 
            angle = 90;
            break;
        case TEX_ROTATE_180: 
            angle = 180;
            break;
        case TEX_FLIP_X:
            scaleX = -1;
            break;
        case TEX_FLIP_Y:
            scaleY = -1;
            break;
    }

    glMatrixMode(GL_TEXTURE); 
        glLoadIdentity();
        glTranslatef(0.5, 0.5, 0);
        glRotatef(angle, 0, 0, 1);
        glScalef(scaleX, scaleY, 1);
        glTranslatef(-0.5, -0.5, 0);
    glMatrixMode(GL_MODELVIEW);
}
