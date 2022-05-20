#pragma once
#include <GL/glut.h>
#include <math.h>
#include <glm.hpp>
#include <vector>
#define PI 3.1415926535897

class Sphere {
private:
	// Physics parameters
	double pos_a = 2;
	double pos_b = 1;
	double vel_a = 0;
	double vel_b = 0;
	double acc = -15;
	double k = 1.;
	double desire_stretching = 1.;

	struct Vertex {
		glm::vec3 pos;
		glm::vec3 norm;
		glm::vec2 tex;
		Vertex() {};
		Vertex(glm::vec3 pos, glm::vec3 norm, glm::vec2 tex) : pos(pos), norm(norm), tex(tex) {};
	};

	// Creates position from angles a, b using sphere radius
	Vertex get_vert(double a, double b);
public:
	double radius = 1;
	glm::vec3 pos = { 0, 0, 0 };
	int grid_size = 10; // in degrees
	double stretching = 1.;
	
	GLuint texture = 0;
	std::vector<Vertex> vertices;

	Sphere();

	void create_vertices();
	void display();
	void step();
};

