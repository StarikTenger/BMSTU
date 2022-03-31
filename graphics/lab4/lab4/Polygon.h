#pragma once
#include <vector>
#include "Vec2.h"
#include "Color.h"

struct Polygon {
	std::vector<Vec2<int>> vertices;
	Color color;
	
	Polygon();
};

