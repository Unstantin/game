#include "snowflake.h"
#include "polygon.h"

void snowflakeNeedle(float l) {
    rect(l / 2, 0, l, SNOWFLAKE_W);
    circle(l, 0, SNOWFLAKE_W / 2, CIRCLE_N);
}

void snowflake() {
    glPushMatrix();

    glColor3f(0.2, 0, 0.7);
    for(int i = 0; i < 6; i++) {
        glRotatef(60, 0, 0, 1);
        snowflakeNeedle(SNOWFLAKE_L);
        
        for(int dir = 1, j = 0; j < 2; j++, dir *= -1) {
            glPushMatrix();

            glTranslatef(SNOWFLAKE_L * 0.75, 0, 0);
            glRotatef(60, 0, 0, dir);
            snowflakeNeedle(SNOWFLAKE_L / 4);
            
            glRotatef(-60, 0, 0, dir);
            glTranslatef(-SNOWFLAKE_L * 0.25, 0, 0);
            glRotatef(60, 0, 0, dir);
            snowflakeNeedle(SNOWFLAKE_L / 4);

            glPopMatrix();
        }
    }
    
    glPopMatrix();
}

