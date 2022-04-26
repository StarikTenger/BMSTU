#include <GL/glut.h>
#include <math.h>
#include <glm.hpp>
#include <vector>
#include <algorithm>
#define M_PI 3.141592653
#define WIDTH 600
#define HEIGHT 600

const int INSIDE = 0;  // 000000
const int LEFT = 1;    // 000001
const int RIGHT = 2;   // 000010
const int BOTTOM = 4;  // 000100
const int TOP = 8;     // 001000
const int NEAR = 16;   // 010000
const int FAR = 32;    // 100000

struct Cam {
    float angle = 0;
    float dist = 2;
};
Cam cam;

struct Clipper {
    glm::vec3 corner1 = { -1, -1, -1 };
    glm::vec3 corner2 = { 1, 1, 1 };
    void set_cube(float r) {
        corner1 = r * glm::vec3(-1, -1, -1);
        corner2 = r * glm::vec3(1, 1, 1);
    }

    int compute_code(glm::vec3 point) {
        int code = INSIDE;

        if (point.x < corner1.x)
            code |= LEFT;
        else if (point.x > corner2.x)
            code |= RIGHT;
        if (point.y < corner1.y)
            code |= BOTTOM;
        else if (point.y > corner2.y)
            code |= TOP;
        if (point.z < corner1.z)
            code |= NEAR;
        else if (point.z > corner2.z)
            code |= FAR;

        return code;
    }

    glm::vec3 intersect_plane_x(glm::vec3 a, glm::vec3 b, float x) {
        float k = (x - a.x) / (b.x - a.x);
        return a + (b - a) * k;
    }
    glm::vec3 intersect_plane_y(glm::vec3 a, glm::vec3 b, float y) {
        float k = (y - a.y) / (b.y - a.y);
        return a + (b - a) * k;
    }
    glm::vec3 intersect_plane_z(glm::vec3 a, glm::vec3 b, float z) {
        float k = (z - a.z) / (b.z - a.z);
        return a + (b - a) * k;
    }
};
double clipper_size = 2;
Clipper clipper;

void draw_line(glm::vec3 p1, glm::vec3 p2) {
    glBegin(GL_LINES);
    glVertex3f(p1.x, p1.y, p1.z);
    glVertex3f(p2.x, p2.y, p2.z);
    glEnd();
}

void draw_line_clipped(glm::vec3 p1, glm::vec3 p2, Clipper& clipper) {
    auto code1 = clipper.compute_code(p1);
    auto code2 = clipper.compute_code(p2);
    if (code1 & code2) return;
    if (code1 | code2) {
        if (code1 > code2) std::swap(p1, p2);
        auto code_out = code1 > code2 ? code1 : code2;
        if (code_out & LEFT) {
            p2 = clipper.intersect_plane_x(p1, p2, clipper.corner1.x);
        } 
        else if (code_out & RIGHT) {
            p2 = clipper.intersect_plane_x(p1, p2, clipper.corner2.x);
        }
        else if (code_out & BOTTOM) {
            p2 = clipper.intersect_plane_y(p1, p2, clipper.corner1.y);
        }
        else if (code_out & TOP) {
            p2 = clipper.intersect_plane_y(p1, p2, clipper.corner2.y);
        }
        else if (code_out & NEAR) {
            p2 = clipper.intersect_plane_z(p1, p2, clipper.corner1.z);
        }
        else if (code_out & FAR) {
            p2 = clipper.intersect_plane_z(p1, p2, clipper.corner2.z);
        }
    }

    draw_line(p1, p2);
}

void draw_sphere(double r, int lats, int longs) {
    std::vector<std::pair<glm::vec3, glm::vec3>> lines;

    for (int i = 0; i <= lats; i++) {
        double lat0 = M_PI * (-0.5 + (double)(i - 1) / lats);
        double z0 = sin(lat0);
        double zr0 = cos(lat0);

        double lat1 = M_PI * (-0.5 + (double)i / lats);
        double z1 = sin(lat1);
        double zr1 = cos(lat1);


       
        double lng = 2 * M_PI * (double)(- 1) / longs;
        double x = cos(lng);
        double y = sin(lng);
        glm::vec3 p1_prev(r * x * zr0, r* y* zr0, r* z0);
        glm::vec3 p2_prev(r * x * zr1, r * y * zr1, r * z1);
        for (int j = 1; j <= longs; j++) {
            double lng = 2 * M_PI * (double)(j - 1) / longs;
            double x = cos(lng);
            double y = sin(lng);
            glm::vec3 p1(r * x * zr0, r * y * zr0, r * z0);
            glm::vec3 p2(r * x * zr1, r * y * zr1, r * z1);

            draw_line_clipped(p1, p2, clipper);
            draw_line_clipped(p1, p1_prev, clipper);
            draw_line_clipped(p2, p2_prev, clipper);

            p1_prev = p1;
            p2_prev = p2;
        }
    }
}

void render() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, WIDTH/HEIGHT, 1, 1000);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(
        cos(cam.angle)* cam.dist, 0, sin(cam.angle) * cam.dist,
        0, 0, 0,
        0, 1, 0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    draw_sphere(2, 16, 16);
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    clipper.set_cube(clipper_size);
    render();

    glFlush();
    glutSwapBuffers();
}

void normalKeys(unsigned char key, int x, int y) {
    double cam_ang_vel = 0.1;
    double cam_dist_vel = 0.1;
    switch (key) {
    case 'a': cam.angle -= cam_ang_vel; break;
    case 'd': cam.angle += cam_ang_vel; break;
    case 'w': cam.dist -= cam_dist_vel; break;
    case 's': cam.dist += cam_dist_vel; break;
    case 'z': clipper_size -= cam_dist_vel * 0.1; break;
    case 'x': clipper_size += cam_dist_vel * 0.1; break;
    }
    glutPostRedisplay();
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Lab 5");

    //glEnable(GL_DEPTH_TEST);
    glutDisplayFunc(display);
    //glutSpecialFunc(specialKeys);
    glutKeyboardFunc(normalKeys);
    glutMainLoop();
    return 0;

}