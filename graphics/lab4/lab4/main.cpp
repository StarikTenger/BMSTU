#include <GL/glut.h>
#include <iostream>
#include "Draw.h"

Draw draw;
Polygon p;


void display() {
    draw.fill({0, 0, 0});
    //draw.line({10, 10}, {40, 200}, {255, 255, 0});
    draw.polygon(p);

    draw.display();
    glutPostRedisplay();
    glutSwapBuffers();
}

void mouse_callback(int button, int state, int x, int y) {
    
    if (button == GLUT_LEFT_BUTTON) {
        if (state == GLUT_DOWN) {
            p.vertices.push_back({x, y});
            p.orientate();
        }
    }
    if (button == GLUT_RIGHT_BUTTON) {
        if (state == GLUT_DOWN) {
            p.vertices.pop_back();
            p.orientate();
        }
    }
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

    draw.init(1000, 1000);
    p.color = { 250, 250, 40 };
    p.vertices = {  };

    glutDisplayFunc(display);
    glutMouseFunc(mouse_callback);
    glutMainLoop();
    return 0;
}