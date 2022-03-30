#pragma once

template<typename T>
struct Vec2 {
	T x = 0;
	T y = 0;
	Vec2() {}
	Vec2(T x, T y) : x(x), y(y) {}

	Vec2 operator+ (const Vec2& b) {
		return Vec2(x + b.x, y + b.y);
	}

	Vec2 operator- (const Vec2& b) {
		return Vec2(x - b.x, y - b.y);
	}

	Vec2 operator*(T b) {
		return Vec2(x * b, y * b);
	}

	Vec2 operator/(T b) {
		return Vec2(x / b, y / b);
	}
};

