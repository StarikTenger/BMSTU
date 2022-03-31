#pragma once
#include <GL/glut.h>
#include <vector>
#include <algorithm>
#include <iostream>
#include "Vec2.h"
#include "Color.h"
#include "Polygon.h"
#include "Line.h"

class Draw {
private:
	int width = 400;
	int height = 400;
	int window = 0;
	GLubyte* pixel_buffer = nullptr;

	std::vector<Vec2<int>> rasterize_line(Vec2<int> start, Vec2<int> finish);

public:
	Draw() {};
	void init(int width = 100, int height = 100);
	void set_pixel(Vec2<int> pos, Color col);
	void fill(Color col);
	void line(Vec2<int> start, Vec2<int> finish, Color col);
	void polygon(Polygon polygon);
	void display();
};
