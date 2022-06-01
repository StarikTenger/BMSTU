#define _CRT_SECURE_NO_WARNINGS
#include <GL/glut.h>
#include <iostream>
#include <math.h>
#include <glm.hpp>
#include <vector>
#include <stdio.h>
#include <algorithm>
#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <cstdlib>
#include <sys/timeb.h>
#include "TextureLoader.h"
#include "Sphere.h"
#include "TimeCounter.h"



int time_prev = 0;
Sphere sphere;
TimeCounter time_counter;
int window;

void display(void) {
	time_counter.start_frame();
	glClearDepth(1);
	glClearColor(0.3, 0.3, 0.3, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glTranslatef(0, 0, -10);
	sphere.display();
	glutSwapBuffers();
	time_counter.end_frame();

	int time_cur = get_milli_count();
	if (time_cur - time_prev >= 20) {		
		time_prev = time_cur;
		for (int i = 0; i < 10; i++) {
			sphere.angle += 0.02;
			sphere.step();
		}
	}

	if (time_counter.check_frame_limit()) {
		std::cout << "Frames processed:   " << time_counter.get_frames() << "\n";
		std::cout << "Total time spent:   " << time_counter.get_ms_sum() << "\n";
		std::cout << "Average frame time: " << time_counter.get_average_time() << " ms\n";
		std::cout << "Standart deviation: " << time_counter.get_standart_deviation() << " ms\n";
		glutDestroyWindow(window);
	}
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
	sphere.radius = 2;
	sphere.grid_size = 5;
	sphere.create_vertices();
	sphere.prepare_list();
	sphere.pos.z = 3;
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

	window = glutCreateWindow("A basic OpenGL Window");

	init();

	glutDisplayFunc(display);

	glutIdleFunc(display);

	glutReshapeFunc(reshape);

	glutMainLoop();

	return 0;
}