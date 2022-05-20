#pragma once
#include <GL/glut.h>
#include <math.h>
#include <glm.hpp>

class TextureLoader {
public:
	static GLuint load_bmp(const char* filename, int width, int height);
};

