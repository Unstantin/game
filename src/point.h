#include <stdio.h>
#include <stdlib.h>

#ifndef POINT
#define POINT

typedef
struct {
    double x, y;
} point;

typedef point pivot;

void pointPrint(point *pnt);
void pointPrintChild(point *pnt);
void pointPrintTree(point *pnt, char root);

#endif 
