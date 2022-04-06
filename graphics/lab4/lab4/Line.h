#pragma once
#include <algorithm>
#include <iostream>
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
		int error0;
	public:
		Point point;
		explicit iterator(Vec2<int> start, Vec2<int> finish) : start(start), finish(finish) {
			tail = point.pos = start;
			delta = abs(start - finish);
			sign = Vec2<int>(
				start.x < finish.x ? 1 : -1,
				start.y < finish.y ? 1 : -1);
			error = delta.x - delta.y;
			error0 = error;
		}
		iterator& operator++() {
			tail = point.pos;
			point.pos = start;
			point.opacity = error0 == 0 ? 1. : (error * 1. / error0);
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