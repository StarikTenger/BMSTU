#include <GL/glut.h>
#include "Draw.h"

Draw draw;

void display() {
    draw.fill({0, 0, 0});
    //draw.line({10, 10}, {40, 200}, {255, 255, 0});

    Polygon p;
    p.color = {250, 250, 40};
    p.vertices = {{250, 250 - 250}, {250 + 20, 250}, {250, 250 - 50}, {250-200, 250} };
    draw.polygon(p);

    draw.display();
    glutSwapBuffers();
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

    draw.init(1000, 1000);

    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}