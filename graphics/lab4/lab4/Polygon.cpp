#include "Polygon.h"

Polygon::Polygon() {
}

int Polygon::area() {
	if (vertices.size() < 3)
		return 0;
	int s = 0;
	for (int i = 0; i < vertices.size(); i++) {
		int j = (i + 1) % vertices.size();
		s += vertices[i].x * vertices[j].y - vertices[j].x * vertices[i].y;
	}
	
	return s;
}

Polygon Polygon::orientate() {
	Polygon p = *this;
	if (area() < 0)
		std::reverse(p.vertices.begin(), p.vertices.end());
	return p;
}
