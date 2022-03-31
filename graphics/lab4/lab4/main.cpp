#include <GL/glut.h>
#include "Draw.h"

Draw draw;

void display() {
    draw.fill({0, 0, 0});
    draw.line({10, 10}, {40, 200}, {255, 255, 0});
    draw.display();
    glutSwapBuffers();
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

    draw.init(500, 500);

    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}