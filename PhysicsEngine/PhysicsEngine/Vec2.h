#pragma once

class Vec2
{
public:
    float x, y;

    static float dotProduct(const Vec2& a, const Vec2& b);
    void normalize();
    float length() const;
    Vec2 operator+(const Vec2& other) const;
    Vec2& operator+=(const Vec2& other);
    Vec2 operator-(const Vec2& other) const;
    Vec2& operator-=(const Vec2& other);
    Vec2 operator*(float scalar) const;
    Vec2 operator/(float scalar) const;
};