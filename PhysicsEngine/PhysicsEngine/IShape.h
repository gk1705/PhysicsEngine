#pragma once
#include "RigidBody.h"

class IShape
{
public:
    virtual ~IShape() = default;
    IShape(RigidBody* body) : body(body) {}
    virtual int getEdgeCount() const = 0;
    virtual Vec2 getEdge(int index) const = 0;
    virtual Vec2 getVertex(int index) const = 0;
    virtual Vec2 updateVertices() = 0;

    IShape(const IShape&) = delete;
    IShape& operator=(const IShape&) = delete;
    IShape(IShape&&) = delete;
    IShape& operator=(IShape&&) = delete;

    RigidBody* body;
};
