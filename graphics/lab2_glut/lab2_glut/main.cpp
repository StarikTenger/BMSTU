#include <GL/glut.h>
#include <math.h>
#include <string>
#include <iostream>
#define PI 3.141592653

double  size = 0.2;
std::string polygonmode[2] = { "GL_LINE","GL_FILL" };
double rotate_y = PI / 4, rotate_x = PI / 8, l = 1.0, phi = PI / 4;
double cam_x = 0.f, cam_y = 0.f, cam_z = 0.f;

void xrot(float a) {
    GLdouble matrix[16] = {
       1,   0,       0,      0,
       0,   cos(a), -sin(a), 0,
       0,   sin(a),  cos(a), 0,
       0,   0,       0,      1,
    };
    glMultMatrixd(matrix);
}

void yrot(float a) {
    GLdouble matrix[16] = {
       cos(a),  0,   sin(a), 0,
       0,       1,   0,      0,
       -sin(a), 0,   cos(a), 0,
       0,       0,   0,      1,
    };
    glMultMatrixd(matrix);
}

void zrot(float a) {
    GLdouble matrix[16] = {
       cos(a), -sin(a), 0, 0,
       sin(a), cos(a),  0, 0,
       0,      0,       1, 0,
       0,      0,       0, 1
    };
    glMultMatrixd(matrix);
}

void translate(float x, float y, float z) {
    GLdouble matrix[16] = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        x, y, z, 1,
    };
    glMultMatrixd(matrix);
}

void scale(float x) {
    GLdouble matrix[16] = {
            x, 0, 0, 0,
            0, x, 0, 0,
            0, 0, x, 0,
           0, 0, 0, 1,
    };
    glMultMatrixd(matrix);
}

void render_cube(bool default_view = false) {
    glLoadIdentity();
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glEnable(GL_DEPTH_TEST);

    float p  = 1;
    float q  = 1;
    float r  = 1;
    
    GLdouble matrix[16] = {
            1, 0, 0, p,
            0, 1, 0, q,
            0, 0, 0, r,
            0, 0, 0, 1,
    };
    double xval = l * cos(PI / 4);
    double yval = l * sin(PI / 4);
    
    if (!default_view) {
        scale(4.f);
        translate(cam_x, cam_y, cam_z);
        glMultMatrixd(matrix);
        xrot(rotate_x);
        yrot(rotate_y);
    }
    else {
        translate(0.5, .5, 0.);
    }
    
    
    glScaled(size, size, size);


    glBegin(GL_LINES);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(0, 0, 0);
    glVertex3f(5, 0, 0);
    glEnd();

    glBegin(GL_LINES);
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 5, 0);
    glEnd();

    glBegin(GL_LINES);
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 10);
    glEnd();

    double zlen = l / 2;
    glBegin(GL_LINE_LOOP);
    glColor3f(0.0, 0.0, 1.0);
    glVertex3f(xval, 0, 1 * l / 2);
    glVertex3f(xval, yval, 1 * l / 2);
    glVertex3f(0, yval, 1 * l / 2);
    glVertex3f(0, 0, 1 * l / 2);

    glEnd();
    glBegin(GL_POLYGON);
    glColor3f(0.533, 0.533, 0.533);
    glVertex3f(xval, 0, 0);
    glVertex3f(xval, yval, 0);
    glVertex3f(0, yval, 0);
    glVertex3f(0, 0, 0);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(0.0, 1.0, 0.0);
    glVertex3f(xval, yval, 1 * l / 2);
    glVertex3f(xval, yval, 0);
    glVertex3f(0, yval, -0);
    glVertex3f(0, yval, 1 * l / 2);
    glEnd();

    glBegin(GL_POLYGON);
    glColor3f(1.0, 0.0, 0.0);
    glVertex3f(xval, 0, 0);
    glVertex3f(xval, yval, 0);
    glVertex3f(xval, yval, 1 * l / 2);
    glVertex3f(xval, 0, 1 * l / 2);
    glEnd();
}

void display() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    render_cube();
    render_cube(true);

    glFlush();
    glutSwapBuffers();

}
void specialKeys(int key, int x, int y) {
    switch (key) {
    case GLUT_KEY_F1:
        l += 0.1;
        break;
    case GLUT_KEY_F2:
        l -= 0.1;
        break;
    case GLUT_KEY_F3:
        phi += 3.1415926535 / 180;
        break;
    case GLUT_KEY_F4:
        phi -= 3.1415926535 / 180;
        break;
    case GLUT_KEY_RIGHT:
        rotate_y += 0.1;
        break;
    case GLUT_KEY_LEFT:
        rotate_y -= 0.1;
        break;
    case GLUT_KEY_UP:
        rotate_x += 0.1;
        break;
    case GLUT_KEY_DOWN:
        rotate_x -= 0.1;
        break;
    case GLUT_KEY_PAGE_UP:
        size += 0.01;
        break;
    case GLUT_KEY_PAGE_DOWN:
        size -= 0.01;
        break;
    }
    glutPostRedisplay();

}

void normalKeys(unsigned char key, int x, int y) {
    double cam_vel = 0.1;
    switch (key) {
        case 'w': cam_z += cam_vel; break;
        case 's': cam_z -= cam_vel; break;
        case 'a': cam_x += cam_vel; break;
        case 'd': cam_x -= cam_vel; break;
        case 'e': cam_y += cam_vel; break;
        case 'q': cam_y -= cam_vel; break;
    }
    glutPostRedisplay();
}

int main(int argc, char* argv[]) {

    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(600, 600);
    glutCreateWindow("lab2");

    //glEnable(GL_DEPTH_TEST);
    glutDisplayFunc(display);
    glutSpecialFunc(specialKeys);
    glutKeyboardFunc(normalKeys);
    glutMainLoop();
    return 0;

}