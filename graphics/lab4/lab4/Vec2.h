#pragma once

template<typename T>
struct Vec2 {
	T x = 0;
	T y = 0;
	Vec2() {}
	Vec2(T x, T y) : x(x), y(y) {}

	friend Vec2 abs(const Vec2& a) {
		return {abs(a.x), abs(a.y)};
	}

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

	friend bool operator==(const Vec2& a, const Vec2& b) {
		return a.x == b.x && a.y == b.y;
	}
};
