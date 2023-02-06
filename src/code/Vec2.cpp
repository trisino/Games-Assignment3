#include "../include/Vec2.h"
#include <cmath>

Vec2::Vec2() : x(0), y(0) {}

Vec2::Vec2(float xin, float yin) : x(xin), y(yin) {}

bool Vec2::operator==(const Vec2 &rhs) const {
  return (x == rhs.x && y == rhs.y);
}

bool Vec2::operator==(const float val) const { return (x == val && y == val); }

bool Vec2::operator!=(const Vec2 &rhs) const {
  return (x != rhs.x && y != rhs.y);
}

Vec2 Vec2::operator+(const Vec2 &rhs) const { return {x + rhs.x, y + rhs.y}; }

Vec2 Vec2::operator-(const Vec2 &rhs) const { return {x - rhs.x, y - rhs.y}; }

Vec2 Vec2::operator/(const float val) const { return {x / val, y / val}; }

Vec2 Vec2::operator*(const float val) const { return {x * val, y * val}; }

void Vec2::operator+=(const Vec2 &rhs) {
  x += rhs.x;
  y += rhs.y;
}

void Vec2::operator-=(const Vec2 &rhs) {
  x -= rhs.x;
  y -= rhs.y;
}

void Vec2::operator*=(const float val) {
  x *= val;
  y *= val;
}

void Vec2::operator/=(const float val) {
  x /= val;
  y /= val;
}

float Vec2::dist(const Vec2 &rhs) const {
  return sqrt(pow((x - rhs.x), 2) + pow(y - rhs.y, 2));
}
bool Vec2::operator>(const float rhs) const { return ((x > rhs) && (y > rhs)); }
