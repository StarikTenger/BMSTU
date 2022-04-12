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
            if (p.vertices.size())
                p.vertices.pop_back();
            p.orientate();
        }
    }
}

void normal_keys(unsigned char key, int x, int y) {
    double cam_vel = 0.1;
    switch (key) {
        case 'x': draw.DEBUG_MODE = true; break;
        case 'z': draw.DEBUG_MODE = false; break;
    }
    glutPostRedisplay();
}

void resize(int width, int height) {
    draw.resize_window();
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

    draw.init(1000, 1000);
    p.color = { 250, 250, 40 };
    p.vertices = {  };

    glutDisplayFunc(display);
    glutMouseFunc(mouse_callback);
    glutReshapeFunc(resize);
    glutKeyboardFunc(normal_keys);
    glutMainLoop();
    return 0;
}