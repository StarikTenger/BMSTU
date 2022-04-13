#pragma once
#include <vector>
#include <algorithm>
#include "Vec2.h"
#include "Color.h"

struct Polygon {
	std::vector<Vec2<int>> vertices;
	Color color;
	
	Polygon();
	int area(); // Returns double area of polygon
	Polygon orientate(); // Orientates polygon clocksise
};

