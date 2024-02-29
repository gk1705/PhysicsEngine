#pragma once
#include "IShape.h"
#include "RigidBody.h"
#include "Vec2.h"

class Triangle : public IShape
{
public:
    Triangle(RigidBody* body, const Vec2& a, const Vec2& b, const Vec2& c);
    int getEdgeCount() const override { return 3; }
    Vec2 getEdge(int index) const override;
    Vec2 getVertex(int index) const override { return vertices[index]; }
    Vec2 updateVertices() override;

private:
    Vec2 vertices[3]{};
};
