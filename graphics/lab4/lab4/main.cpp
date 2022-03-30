#include <GL/glut.h>
#include "Draw.h"

Draw draw;

void display() {
    draw.fill({40, 40, 120});
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