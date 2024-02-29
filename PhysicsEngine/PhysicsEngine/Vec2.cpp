#include "Vec2.h"

#include <cmath>

float Vec2::dotProduct(const Vec2& a, const Vec2& b)
{
	return a.x * b.x + a.y * b.y;
}

void Vec2::normalize()
{
	const float length = std::sqrt(x * x + y * y);
	x /= length;
	y /= length;
}

float Vec2::length() const
{
	return std::sqrt(x * x + y * y);
}

Vec2 Vec2::operator+(const Vec2& other) const
{
	return { x + other.x, y + other.y };
}

Vec2& Vec2::operator+=(const Vec2& other)
{
	x += other.x;
	y += other.y;
	return *this;
}

Vec2 Vec2::operator-(const Vec2& other) const
{
	return { x - other.x, y - other.y };
}

Vec2& Vec2::operator-=(const Vec2& other)
{
	x -= other.x;
	x -= other.y;
	return *this;
}

Vec2 Vec2::operator*(float scalar) const
{
	return { x * scalar, y * scalar };
}

Vec2 Vec2::operator/(float scalar) const
{
	return { x / scalar, y / scalar };
}