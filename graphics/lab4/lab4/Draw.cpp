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
	auto delta = abs(start - finish);
	if ((start.y < finish.y) ^ (start.x > finish.x) ^ (delta.x < delta.y))
		std::swap(start, finish);
	Line line(finish, start);
	for (const auto& p : line) {
		set_pixel(p.pos, col * p.opacity);
	}
} 

void Draw::polygon(Polygon polygon) {
	if (!polygon.vertices.size())
		return;

	// Top & bottom points
	int y_min, y_max;
	y_min = y_max = polygon.vertices[0].y;
	for (const auto& p : polygon.vertices) {
		y_min = std::min(y_min, p.y);
		y_max = std::max(y_max, p.y);
	}

	// Rasterize
	std::vector<std::priority_queue<std::pair<int, int>>> layers(y_max - y_min + 1);
	for (int i = 0; i < polygon.vertices.size(); i++) {
		int j = (i + 1) % polygon.vertices.size();
		int k = (polygon.vertices.size() + i - 1) % polygon.vertices.size();
		int y_prev = -1;
		auto v1 = polygon.vertices[i];
		auto v2 = polygon.vertices[j];
		if (v1.x < v2.x) std::swap(v1, v2);
		for (const auto& p : Line(v1, v2)) {
			if (y_prev != p.pos.y && p.pos.y != polygon.vertices[j].y)
				layers[p.pos.y - y_min].push({ p.pos.x, i });
			y_prev = p.pos.y;
		}		
		if ((polygon.vertices[i].y - polygon.vertices[j].y > 0) ^
			(polygon.vertices[i].y - polygon.vertices[k].y < 0)) {
			layers[polygon.vertices[i].y - y_min].push({ polygon.vertices[i].x, k });
		}
	}

	// Draw
	for (int i = 0; i < layers.size(); i++) {
		bool draw_flag = 1;
		
		while (layers[i].size() >= 2) {
			int x0 = layers[i].top().first;
			int ind0 = layers[i].top().second;
			
			while (!layers[i].empty() && layers[i].top().second == ind0) {
				layers[i].pop();
			}
			if (layers[i].empty())
				continue;

			int x1 = layers[i].top().first;
			int ind1 = layers[i].top().second;
			while (!layers[i].empty() && layers[i].top().second == ind1) {
				layers[i].pop();
			}
			

			for (int x = x1 + 1; x < x0; x++) {
				set_pixel({x, y_min + i}, polygon.color * (DEBUG_MODE ? 0.1 : 1));
			}

			if (DEBUG_MODE) {
				set_pixel({x0, y_min + i}, { 0, 180, 0 }, PixelMode::ADD);
				set_pixel({x1, y_min + i}, { 180, 0, 0 }, PixelMode::ADD);
			}
		}
	}

	// Borders
	for (int i = 0; i < polygon.vertices.size(); i++) {
		int j = (i + 1) % polygon.vertices.size();
		if(!DEBUG_MODE)
			line(polygon.vertices[i], polygon.vertices[j], polygon.color);
	}
}

void Draw::set_pixel(Vec2<int> pos, Color col, PixelMode mode) {
	pos.y = height - pos.y - 1;
	if (0 <= pos.x && pos.x < width && 0 <= pos.y && pos.y < height) {
		int position = (pos.x + pos.y * width) * 3;
		switch (mode) {
			case PixelMode::OVERWRITE:
				pixel_buffer[position] = col.r;
				pixel_buffer[position + 1] = col.g;
				pixel_buffer[position + 2] = col.b;
				break;
			case PixelMode::ADD:
				pixel_buffer[position] += col.r;
				pixel_buffer[position + 1] += col.g;
				pixel_buffer[position + 2] += col.b;
				break;
		}
	}
}

void Draw::display() {
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawPixels(width, height, GL_RGB, GL_UNSIGNED_BYTE, pixel_buffer);
}