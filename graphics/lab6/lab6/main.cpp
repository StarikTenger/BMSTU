#define _CRT_SECURE_NO_WARNINGS
#include <GL/glut.h>
#include <math.h>
#include <glm.hpp>
#include <vector>
#include <stdio.h>
#include <algorithm>
#include <windows.h>
#include <stdio.h>
#include <math.h>
#include "TextureLoader.h"
#include "Sphere.h"

Sphere sphere;
double angle = 0;
void display(void) {
	glClearDepth(1);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(0, 0, -10);
	glRotatef(angle, 0, 1, 0);
	sphere.display();
	glutSwapBuffers();
	angle += 0.2;
}

void init(void) {
	// Configure light
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };
	GLfloat light_position[] = { 1.0, 0.0, 1.0, 0.0 };
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);

	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);


	glEnable(GL_TEXTURE_2D);
	glDepthFunc(GL_LEQUAL);

	sphere.texture = TextureLoader::load_bmp("texture.bmp", 1000, 1000);
	sphere.radius = 1;
	sphere.space = 5;
	sphere.create_vertices();
}
void reshape(int w, int h) {

	glViewport(0, 0, (GLsizei)w, (GLsizei)h);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	gluPerspective(60, (GLfloat)w / (GLfloat)h, 0.1, 100.0);

	glMatrixMode(GL_MODELVIEW);
}
int main(int argc, char** argv) {

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH);

	glutInitWindowSize(500, 500);

	glutInitWindowPosition(100, 100);

	glutCreateWindow("A basic OpenGL Window");

	init();

	glutDisplayFunc(display);

	glutIdleFunc(display);

	glutReshapeFunc(reshape);

	glutMainLoop();

	return 0;
}