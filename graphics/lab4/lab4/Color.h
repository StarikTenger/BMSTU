#pragma once

struct Color {
	int r, g, b;
	Color(int r = 0, int g = 0, int b = 0);
	void normalize();
	Color operator*(float x);
};

