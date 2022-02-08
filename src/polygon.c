#include "polygon.h"
 
void circle(float x, float y, float r, int n) {
    glBegin(GL_POLYGON);
    for(int i = 0; i < n; i++) {
        glVertex3f(x + r * cos((2. * M_PI * i) / n), y + r * sin((2. * M_PI * i) / n), 0);    
    }
    glEnd();
}

void rect(float x, float y, float w, float h) {
    glBegin(GL_POLYGON);
        glVertex3f(x - w / 2, y - h / 2, 0);
        glVertex3f(x - w / 2, y + h / 2, 0);
        glVertex3f(x + w / 2, y + h / 2, 0);
        glVertex3f(x + w / 2, y - h / 2, 0);
    glEnd();
}

