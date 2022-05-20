#include "Sphere.h"


Sphere::Vertex Sphere::get_vert(double a, double b) {
	return Sphere::Vertex(
		glm::vec3(
			radius * sin((a) / 180 * PI) * sin((b) / 180 * PI),
			radius * cos((a) / 180 * PI) * sin((b) / 180 * PI),
			radius * cos((b) / 180 * PI)),
		glm::vec3(
			sin((a) / 180 * PI) * sin((b) / 180 * PI),
			cos((a) / 180 * PI) * sin((b) / 180 * PI),
			cos((b) / 180 * PI)),
		glm::vec2(
			(a) / 360,
			(2 * b) / 360
		));
}

Sphere::Sphere(){
}

void Sphere::create_vertices() {
	vertices = {};

	for (double b = 0; b <= 90 - grid_size; b += grid_size) {
		for (double a = 0; a <= 360 - grid_size; a += grid_size) {
			vertices.push_back(get_vert(a, b));
			vertices.push_back(get_vert(a, b + grid_size));
			vertices.push_back(get_vert(a + grid_size, b));
			vertices.push_back(get_vert(a + grid_size, b + grid_size));
		}
	}
}

void Sphere::display() {
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//glScalef(0.0125 * radius, 0.0125 * radius, 0.0125 * radius);
	glRotatef (-90, 1, 0, 0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_TRIANGLE_STRIP);

	for (const auto& vert : vertices) {
		glTexCoord2f(vert.tex.x, vert.tex.y);
		auto pos_cur = vert.pos;
		pos_cur.z *= -1 * stretching;
		pos_cur += pos;
		glVertex3f(pos_cur.x, pos_cur.y, pos_cur.z);
		glNormal3f(vert.norm.x, vert.norm.y, -vert.norm.z);
	}

	for (const auto& vert : vertices) {
		glTexCoord2f(vert.tex.x, -vert.tex.y);
		auto pos_cur = vert.pos;
		pos_cur.z *= 1 * stretching;
		pos_cur += pos;
		glVertex3f(pos_cur.x, pos_cur.y, pos_cur.z);
		glNormal3f(vert.norm.x, vert.norm.y, vert.norm.z * stretching);
	}

	glEnd();
}

void Sphere::step() {
	double dt = 0.003;
	// Gravity
	vel_a += acc * dt;
	vel_b += acc * dt;

	// Bounce
	if (pos_a < -2 && vel_a < 0) {
		vel_a *= -1;
	}
	if (pos_b < -2 && vel_b < 0) {
		vel_b *= -1;
	}

	// Spring
	//vel_a += (pos_b - pos_a - radius) * k;
	//vel_b += (pos_a - pos_b + radius) * k;

	// Movement
	pos_a += vel_a * dt;
	pos_b += vel_b * dt;

	// Attach to sphere
	pos.z = (pos_a + pos_a) / 2;
	//stretching = (pos_b - pos_a) / radius;
	desire_stretching = 1. - vel_a * 0.2;
	stretching += (desire_stretching - stretching) * dt * 2.;
}
