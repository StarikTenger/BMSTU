#pragma once
#include <algorithm>
#include <iostream>
#include <math.h>
#include "Vec2.h"
#include "Color.h"
#include "Polygon.h"

class Line {
private:
	Vec2<int> start;
	Vec2<int> finish;
public:
	struct Point {
		Vec2<int> pos;
		float opacity = 1.f;
	};

	class iterator : public std::iterator<
		std::input_iterator_tag,   // iterator_category
		Point,                      // value_type
		long,                      // difference_type
		const long*,               // pointer
		Point                       // reference
	> {
	private:
		Vec2<int> start;
		Vec2<int> finish;
		Vec2<int> tail;
		Vec2<int> delta;
		Vec2<int> sign;
		int error;
	public:
		Point point;
		explicit iterator(Vec2<int> start, Vec2<int> finish) : start(start), finish(finish) {
			tail = point.pos = start;
			delta = abs(start - finish);
			sign = Vec2<int>(
				start.x < finish.x ? 1 : -1,
				start.y < finish.y ? 1 : -1);
			error = delta.x - delta.y;
		}
		iterator& operator++() {
			tail = point.pos;
			point.pos = start;
			if (delta.x > delta.y) {
				float m = delta.y * sign.y * 1. / (delta.x * sign.x);
				float y1 = finish.y - delta.y * sign.y + (point.pos.x - finish.x + delta.x * sign.x) * m;
				point.opacity = .5 + (y1 - point.pos.y) * sign.y;
			}
			if (delta.x < delta.y) {
				float m = delta.x * sign.x * 1. / (delta.y * sign.y);
				float x1 = finish.x - delta.x * sign.x + (point.pos.y - finish.y + delta.y * sign.y) * m;
				point.opacity = .5 - (x1 - point.pos.x) * sign.x;
			}
			int error2 = error * 2;
			if (error2 > -delta.y) {
				error -= delta.y;
				start.x += sign.x;
			}
			if (error2 < delta.x) {
				error += delta.x;
				start.y += sign.y;
			}
			return *this;
		}
		iterator operator++(int) { iterator retval = *this; ++(*this); return retval; }
		bool operator==(iterator other) const {
			return tail == other.point.pos;
		}
		bool operator!=(iterator other) const { return !(*this == other); }
		reference operator*() const { return point; }
	};

	Line(Vec2<int> start, Vec2<int> finish) : start(start), finish(finish) {}
	iterator begin() { return iterator(start, finish); }
	iterator end() { return iterator(finish, finish); }
};