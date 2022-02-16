#include "Model.h"
#include <iostream>

Vec2 operator+(const Vec2& a, const Vec2& b) {
    return Vec2(a.x + b.x, a.y+b.y);
}

Vec2 operator-(const Vec2& a, const Vec2& b) {
    return Vec2(a.x - b.x, a.y - b.y);
}

Vec2 operator*(const Vec2& a, float b) {
    return Vec2(a.x * b, a.y * b);
}

Vec2 operator/(const Vec2& a, float b) {
    return Vec2(a.x / b, a.y / b);
}

float distance(const Vec2& a, const Vec2& b) {
    auto c = a - b;
    return sqrt(c.x * c.x + c.y*c.y);
}

Vec2::Vec2() {
}

Vec2::Vec2(float _x, float _y) {
    x = _x;
    y = _y;
}

Model::Model() {
    points = std::vector<Point>(4);
    points[0].pos = { 0.1, 0.1 };
    points[1].pos = { 0.1, -0.1 };
    points[2].pos = { -0.1, -0.1 };
    points[3].pos = { -0.1, 0.1 };

    // Set length
    spring_lengths = std::vector<std::vector<float>>(points.size());
    for (auto& col : spring_lengths) {
        col = std::vector<float> (points.size());
    }

    for (int i = 0; i < points.size(); i++) {
        for (int j = 0; j < points.size(); j++) {
               spring_lengths[i][j] = distance(points[i].pos, points[j].pos);
        }
    }
}

void Model::set_vertices(float* vertices) {
    float k = 0.1;
    for (int i = 0; i < points.size(); i++) {
        vertices[3 * i] = points[i].pos.x;
        vertices[3 * i + 1] = points[i].pos.y;
    }    
}

void Model::set_gravity(Vec2 g) {
    gravity = g;
}

void Model::spin() {
    std::swap(points[0].vel.x, points[0].vel.y);
}

void Model::step() {
    // Gravity
    for (auto& p : points) {
        p.vel = p.vel + gravity*dt;
    }
    
    // Borders
    for (auto& p : points) {
        if (p.pos.x < borders[0].x && p.vel.x < 0 || p.pos.x > borders[1].x && p.vel.x > 0)
            p.vel.x *= -1;
        if (p.pos.y < borders[0].y && p.vel.y < 0 || p.pos.y > borders[1].y && p.vel.y > 0)
            p.vel.y *= -1;
    }

    // Springs
    for (int i = 0; i < points.size(); i++) {
        for (int j = 0; j < points.size(); j++) {
            auto& p1 = points[i];
            auto& p2 = points[j];
            if (i == j) continue;
            float k = 0.8;
            float springlen = spring_lengths[i][j];        
            float dist = distance(p1.pos, p2.pos);
            float stretch = dist - springlen;
            //std::cout << i << " " << j << " " << dist << " " << springlen  << "\n";
            p1.vel = p1.vel + (p2.pos - p1.pos) / dist * stretch * k;
        }
    }

    // Friction
    for (auto& p : points) {
        p.vel = p.vel - p.vel * friction * dt;
    }

    // Movement
    for (auto& p : points)
        p.step(dt);
}

Point::Point() {
}

Point::Point(Vec2 _pos, Vec2 _vel) {
    pos = _pos;
    vel = _vel;
}

void Point::step(float dt) {
    pos = pos + vel*dt;
}
