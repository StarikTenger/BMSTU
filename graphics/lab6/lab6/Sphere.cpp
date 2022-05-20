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

	for (double b = 0; b <= 90 - space; b += space) {
		for (double a = 0; a <= 360 - space; a += space) {
			vertices.push_back(get_vert(a, b));
			vertices.push_back(get_vert(a, b + space));
			vertices.push_back(get_vert(a + space, b));
			vertices.push_back(get_vert(a + space, b + space));
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
		glVertex3f(vert.pos.x, vert.pos.y, -vert.pos.z);
		glNormal3f(vert.norm.x, vert.norm.y, -vert.norm.z);
	}

	for (const auto& vert : vertices) {
		glTexCoord2f(vert.tex.x, -vert.tex.y);
		glVertex3f(vert.pos.x, vert.pos.y, vert.pos.z);
		glNormal3f(vert.norm.x, vert.norm.y, vert.norm.z);
	}

	glEnd();
}
