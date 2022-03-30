#pragma once
#include <GL/glut.h>
#include "Vec2.h"

struct Color {
	int r, g, b;
	Color (int r = 0, int g = 0, int b = 0);
	void normalize();
};

class Draw {
private:
	int width = 400;
	int height = 400;
	int window = 0;
	GLubyte* pixel_buffer = nullptr;

public:
	Draw() {};
	void init(int width = 100, int height = 100);
	void set_pixel(Vec2<int> pos, Color col);
	void fill(Color col);
	void line(Vec2<int> begin, Vec2<int> end, Color col);
	void display();
};

