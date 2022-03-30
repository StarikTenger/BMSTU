#include <GL/glut.h>
#include <math.h>
#include <string>
#include <iostream>
#define M_PI 3.141592653

double  size = 0.2;
std::string polygonmode[2] = { "GL_LINE","GL_FILL" };
double rotate_y = M_PI / 4, rotate_x = M_PI / 8, l = 1.0, phi = M_PI / 4;
double cam_x = 0.f, cam_y = 0.f, cam_z = 0.f;
bool fill_mode = false;

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

void draw_conus(double r, int lats, int longs) {
    int i, j;
    for (i = 0; i <= lats / 2; i++) {
        double lat0 = M_PI * (-0.5 + (double)(i - 1) / lats);
        double z0 = sin(lat0);
        double zr0 = cos(lat0);

        double lat1 = M_PI * (-0.5 + (double)i / lats);
        double z1 = sin(lat1);
        double zr1 = cos(lat1);

        glBegin(GL_QUAD_STRIP);
        for (j = 0; j <= longs; j++) {
            double lng = 2 * M_PI * (double)(j - 1) / longs;
            double x = cos(lng);
            double y = sin(lng);

            glNormal3f(x * zr0, y * zr0, z0);
            glVertex3f(r * x * zr0, r * y * zr0, r * z0);
            glNormal3f(x * zr1, y * zr1, z1);
            glVertex3f(r * x * zr1, r * y * zr1, r * z1);
        }
        glEnd();
    }
}

void render_conus(bool default_view = false) {
    glLoadIdentity();
    if (fill_mode)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glEnable(GL_DEPTH_TEST);

    float p = 1;
    float q = 1;
    float r = 1;

    GLdouble matrix[16] = {
            1, 0, 0, p,
            0, 1, 0, q,
            0, 0, 0, r,
            0, 0, 0, 1,
    };
    double xval = l * cos(M_PI / 4);
    double yval = l * sin(M_PI / 4);

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


    draw_conus(1., 2, 100);
}

void display() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    render_conus();
    render_conus(true);

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
    case 'd': cam_x += cam_vel; break;
    case 'a': cam_x -= cam_vel; break;
    case 'w': cam_y += cam_vel; break;
    case 's': cam_y -= cam_vel; break;
    case 'z': fill_mode = false; break;
    case 'x': fill_mode = true; break;
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