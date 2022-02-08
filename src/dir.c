#include "dir.h"

dir dirs[] = {
    [DIR_LEFT] = {0, -1},
    [DIR_RIGHT] = {0, 1},
    [DIR_UP] = {-1, 0},
    [DIR_DOWN] = {1, 0}
};

unsigned int dirnameOpposite[] = {
    [DIR_LEFT] = DIR_RIGHT,
    [DIR_RIGHT] = DIR_LEFT,
    [DIR_UP] = DIR_DOWN,
    [DIR_DOWN] = DIR_UP
};

void dirInit(dir *d) {
    d->di = 0;
    d->dj = 0;
}

int dir2dirname(dir d) {
    if(d.di > 0 && d.dj == 0) return DIR_DOWN;
    if(d.di == 0 && d.dj > 0) return DIR_RIGHT;
    if(d.di < 0 && d.dj == 0) return DIR_UP;
    if(d.di == 0 && d.dj < 0) return DIR_LEFT;

    return DIR_N;
}

dir dirname2dir(unsigned int dirname) {
    return dirs[dirname % DIR_N];
} 

