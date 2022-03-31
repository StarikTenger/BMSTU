#include "Draw.h"
#include <math.h>
#include <queue>

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

void Draw::line(Vec2<int> start, Vec2<int> finish, Color col) {
	Line line(start, finish);
	for (const auto& p : line) {
		set_pixel(p.pos, col * p.opacity);
	}
}

void Draw::polygon(Polygon polygon) {
	if (!polygon.vertices.size())
		return;

	// Top & bottom points
	int y_min, y_max = polygon.vertices[0].y;
	for (const auto& p : polygon.vertices) {
		y_min = std::min(y_min, p.y);
		y_max = std::max(y_max, p.y);
	}

	// Rasterize
	std::vector<std::priority_queue<int>> layers(y_max - y_min);
	
	

	// Sort

	// Draw
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