#include "Sphere.h"
#include "Optimization.h"

glm::mat3x3 rotate_z(double angle) {
	return {
		cos(angle), -sin(angle), 0,
		sin(angle), cos(angle),  0,
		0,          0,           1
	};
}

struct RotateZ {
	double sin_a;
	double cos_a;

	RotateZ(double angle) {
		sin_a = sin(angle);
		cos_a = cos(angle);
	}

	RotateZ(double sin_a, double cos_a) : sin_a(sin_a), cos_a(cos_a) {};

	RotateZ invert() {
		return RotateZ(-sin_a, cos_a);
	}

	glm::vec3 operator()(const glm::vec3& pos) {
		return {
			pos.x * cos_a - pos.y * sin_a,
			pos.x * sin_a + pos.y * cos_a,
			pos.z
		};
	}
};

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

void Sphere::bind_arrays() {
	float* coords = new float[vertices.size() * 3];
	float* norms = new float[vertices.size() * 3];
	float* tex_coords = new float[vertices.size() * 2];
	for (int i = 0; i < vertices.size(); i++) {
		coords[i * 3 + 0] = vertices[i].pos.x;
		coords[i * 3 + 1] = vertices[i].pos.y;
		coords[i * 3 + 2] = vertices[i].pos.z;

		norms[i * 3 + 0] = vertices[i].norm.x;
		norms[i * 3 + 1] = vertices[i].norm.y;
		norms[i * 3 + 2] = vertices[i].norm.z;

		tex_coords[i * 2 + 0] = vertices[i].tex.x;
		tex_coords[i * 2 + 1] = -vertices[i].tex.y;
	}

	glVertexPointer(3, GL_FLOAT, 0, coords);
	glNormalPointer(GL_FLOAT, 0, norms);
	glTexCoordPointer(2, GL_FLOAT, 0, tex_coords);

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
}

Sphere::Sphere() {
}

void Sphere::create_vertices() {
	vertices = {};

	for (double b = 0; b <= 180 - grid_size; b += grid_size) {
		for (double a = 0; a <= 360 - grid_size; a += grid_size) {
			vertices.push_back(get_vert(a, b));
			vertices.push_back(get_vert(a, b + grid_size));
			vertices.push_back(get_vert(a + grid_size, b));
			vertices.push_back(get_vert(a + grid_size, b + grid_size));
		}
	}

	bind_arrays();
}

void Sphere::prepare_list() {
	list_index = glGenLists(1);
	glNewList(list_index, GL_COMPILE);
		if (ENABLE_VAO) {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glRotatef(-90, 1, 0, 0);

			glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.size());
		}
		else {
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glRotatef(-90, 1, 0, 0);
			glBindTexture(GL_TEXTURE_2D, texture);
			glBegin(GL_TRIANGLE_STRIP);
			for (auto vert : vertices) {
				glTexCoord2f(vert.tex.x, vert.tex.y);
				auto pos_cur = vert.pos;
				pos_cur.z *= -1 * stretching;
				pos_cur += pos;
				glVertex3f(pos_cur.x, pos_cur.y, pos_cur.z);
				glNormal3f(vert.norm.x, vert.norm.y, -vert.norm.z);
			}
			glEnd();
		}
	glEndList();
}

void Sphere::display() {
	if (ENABLE_DISPLAY_LIST) {
		glTranslated(0, pos.z, 0);
		glRotatef(-angle * 180 / PI, 0, 1, 0);
		glCallList(list_index);
		return;
	}
	if (ENABLE_VAO) {
		glTranslated(0, pos.z, 0);
		glRotatef(-angle * 180 / PI, 0, 1, 0);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glRotatef(-90, 1, 0, 0);

		glDrawArrays(GL_TRIANGLE_STRIP, 0, vertices.size());
		return;
	}

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	if (ENABLE_MATRIX_ROTATION)
		glRotatef(-angle * 180 / PI, 0, 1, 0);
	glRotatef(-90, 1, 0, 0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_TRIANGLE_STRIP);

	RotateZ rotation(angle);
	for (auto vert : vertices) {

		// Rotate
		if (!ENABLE_MATRIX_ROTATION) {
			vert.pos = rotation(vert.pos);
			vert.norm = rotation(vert.norm);
		}
		if (ENABLE_CLIPPING &&
			(!ENABLE_MATRIX_ROTATION && vert.pos.y > 0 ||
				ENABLE_MATRIX_ROTATION && (rotation.invert()(vert.pos)).y > 0))
			continue;
		glTexCoord2f(vert.tex.x, vert.tex.y);
		auto pos_cur = vert.pos;
		pos_cur.z *= -1 * stretching;
		pos_cur += pos;
		glVertex3f(pos_cur.x, pos_cur.y, pos_cur.z);
		glNormal3f(vert.norm.x, vert.norm.y, -vert.norm.z);
	}

	glEnd();
}

void Sphere::step() {
	double dt = 0.003;
	double gravity = -9.;
	double floor = -2;
	// Gravity
	vel += gravity * dt;

	// Bounce
	if (pos.z < floor && vel < 0) {
		vel *= -1;
	}

	// Movement
	pos.z += vel * dt;
}
