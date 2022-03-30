#include "Draw.h"
#include <math.h>

Color::Color(int r, int g, int b) : r(r), g(g), b(b) {
	normalize();
};

void Color::normalize() {
	if (r < 0) r = 0;
	if (r > 255) r = 255;
	if (g < 0) g = 0;
	if (g > 255) g = 255;
	if (b < 0) b = 0;
	if (b > 255) b = 255;
}

void Draw::init(int _width, int _height) {
	// Size
	width = _width;
	height = _height;

	// Set up window
	glutInitWindowSize(width, height);
	glutInitWindowPosition(100, 100);
	window = glutCreateWindow("Foo");
	// Clear color
	glClearColor(0.0, 0.0, 0.0, 0);
	// Init pixel buffer
	pixel_buffer = new GLubyte[width * height * 3];
}

void Draw::fill(Color col) {
	for (int i = 0; i < width * height; i++) {
		pixel_buffer[i * 3 + 0] = col.r;
		pixel_buffer[i * 3 + 1] = col.g;
		pixel_buffer[i * 3 + 2] = col.b;
	}
}

void Draw::line(Vec2<int> begin, Vec2<int> end, Color col) {
	auto delta = begin - end;
		delta.x = abs(delta.x);
		delta.y = abs(delta.y);
	Vec2<int> sign(
		begin.x < end.x ? 1 : -1, 
		begin.y < end.y ? 1 : -1);
	int error = delta.x - delta.y;
	set_pixel(end, col);
	while (begin.x != end.x || begin.y != end.y) {
		set_pixel(begin, col);
		int error2 = error * 2;
		if (error2 > -delta.y) {
			error -= delta.y;
			begin.x += sign.x;
		}
		if (error2 < delta.x) {
			error += delta.x;
			begin.y += sign.y;
		}
	}
}

void Draw::set_pixel(Vec2<int> pos, Color col) {
	pos.y = height - pos.y - 1;
	if (0 <= pos.x && pos.x < width && 0 <= pos.y && pos.y < height) {
		int position = (pos.x + pos.y * width) * 3;
		pixel_buffer[position] = col.r;
		pixel_buffer[position + 1] = col.g;
		pixel_buffer[position + 2] = col.b;
	}
}

void Draw::display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, pixel_buffer);
}