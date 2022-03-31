#include "Color.h"

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

Color Color::operator*(float x) {
	return Color(r * x, g * x, b * x);
}