#include "point.h"
#include "game.h"

void pointPrint(point *pnt) {
    pointPrintTree(pnt, PRINT_ROOT);
} 

void pointPrintChild(point *pnt) {
    pointPrintTree(pnt, PRINT_CHILD);
}

void pointPrintTree(point *pnt, char root) {
    if(!pnt) return;
    
    if(root) 
        printf("point ");
    else 
        printf("<point>: ");

    printf("{x: %g, y: %g}", pnt->x, pnt->y);
    if(root) printf("\n");

    fflush(stdout);
}
