#pragma once
#include <vector>

struct Vec2 {
	float x = 0;
	float y = 0;
	Vec2();
	Vec2(float _x, float _y);
};

Vec2 operator+ (const Vec2& a, const Vec2& b);
Vec2 operator- (const Vec2& a, const Vec2& b);
Vec2 operator* (const Vec2& a, float b);
Vec2 operator/ (const Vec2& a, float b);

float distance(const Vec2& a, const Vec2& b);

struct Point {
	Vec2 pos;
	Vec2 vel;
	Point();
	Point(Vec2 _pos, Vec2 _vel);
	void step(float dt);
};

class Model {
private:
	float dt = 0.002;
	float friction = 0.1;
	float bounceK = 0.5;
	Vec2 gravity = Vec2(0., 0);
	std::vector<Vec2> borders = {Vec2(-1, -1), Vec2(1, 1)};
	std::vector<std::vector<float>> spring_lengths;
	std::vector<Point> points;
public:
	Model();
	void set_vertices(float* vertices);
	void set_gravity(Vec2 g);
	void spin();
	void step();
};

