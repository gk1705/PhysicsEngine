#pragma once
#include "IShape.h"

class Rect : public IShape
{
public:
    Rect(RigidBody* body, float width, float height);
    int getEdgeCount() const override { return 4; }
    Vec2 getEdge(int index) const override;
    Vec2 getVertex(int index) const override { return vertices[index]; }
    Vec2 updateVertices() override;

private:
    Vec2 vertices[4]{};
};
