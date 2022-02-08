#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>

#ifndef COLOR
#define COLOR

typedef
struct {
    float r, g, b;
} color;

#define PIECE_COLOR_RANGE_MIN 0.2 
#define PIECE_COLOR_RANGE_MAX 0.8
#define PIECE_COLOR_DIST_MIN 0.4

double randRange(double x, double y);
char colorDistCheck(color col1, color col2, double mindist);
void setColor(unsigned int colorname);

enum color_names {
    COLOR_BOARD_BACKGROUND, COLOR_WALL, COLOR_WALL_FLOAT_PLACEABLE, 
    COLOR_WALL_FLOAT_NON_PLACEABLE, COLOR_PIECE_ACTIVE,
    COLOR_PIECE_DEFAULT, COLOR_SHAPE, COLOR_CELL, 
    COLOR_PATH_DEFAULT, COLOR_CELL_LANDING_SITE, 
    COLOR_NODE, COLOR_SLIDE, COLOR_SLIDE_BUTTON_FOCUSED, 
    COLOR_SLIDE_BUTTON_INACTIVE, COLOR_N
}; 

extern color colors[COLOR_N];

#endif
