#include <stdio.h>
#include <stdlib.h>

#ifndef DIR
#define DIR

enum dirname {DIR_LEFT, DIR_RIGHT, 
               DIR_UP, DIR_DOWN, DIR_N}; 
typedef
struct dir_ {
    int di, dj;
} dir;

void dirInit(dir *d);
int dir2dirname(dir d);
dir dirname2dir(unsigned int dirname);

extern dir dirs[];
extern unsigned int dirnameOpposite[];

#endif

