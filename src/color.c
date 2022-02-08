#include "color.h"

color colors[COLOR_N] = {
    [COLOR_BOARD_BACKGROUND] = {0.4, 0.4, 0.4},
    [COLOR_CELL] = {0.8, 0.7, 0.6},
    [COLOR_PIECE_ACTIVE] = {0.7, 0, 0},
    [COLOR_PIECE_DEFAULT] = {0.6, 0.6, 0.6},
    [COLOR_WALL] = {0.2, 0.6, 0.2},
    [COLOR_WALL_FLOAT_PLACEABLE] = {0.8, 0.8, 0},
    [COLOR_WALL_FLOAT_NON_PLACEABLE] = {0.7, 0.2, 0.2},
    [COLOR_SHAPE] = {1, 1, 1},
    [COLOR_PATH_DEFAULT] = {0.1, 0.1, 0.1},
    [COLOR_CELL_LANDING_SITE] = {0.6, 0.6, 0.6},
    [COLOR_NODE] = {0.2, 0.6, 0.2},
    [COLOR_SLIDE] = {1, 1, 1},
    [COLOR_SLIDE_BUTTON_FOCUSED] = {0.3, 0.3, 1},
    [COLOR_SLIDE_BUTTON_INACTIVE] = {0.5, 0.5, 0.5}
};

double randRange(double x, double y) {
    return ((double)rand() / RAND_MAX - 0.5) * (x - y) + (x + y) / 2;
}

char colorDistCheck(color col1, color col2, double mindist) {
    double ddr = col1.r - col2.r;
    ddr *= ddr;

    double ddg = col1.g - col2.g;
    ddg *= ddg;

    double ddb = col1.b - col2.b;
    ddb *= ddb;
    
    double dd = ddr + ddg + ddb;

    return dd >= mindist * mindist;
}

void setColor(unsigned int colorname) {
    if(colorname >= COLOR_N) return;

    color *clr = &colors[colorname];
    glColor3f(clr->r, clr->g, clr->b);
}
