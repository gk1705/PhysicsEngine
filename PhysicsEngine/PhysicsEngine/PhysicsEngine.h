#pragma once

#include <vector>
#include <iostream>

#include "IShape.h"
#include "RigidBody.h"
#include "Spring.h"
#include "Vec2.h"

class PhysicsEngine {
public:
    PhysicsEngine(int collisionIterations = 10) : collisionIterations(collisionIterations) {}

    void update(float deltaTime) const;
    void addShape(IShape* shape);
    void addRigidBody(RigidBody* body);
    void addSpring(Spring* spring);

private:
    std::vector<IShape*> shapes;
    std::vector<RigidBody*> rigidBodies;
    std::vector<Spring*> springs;
    int collisionIterations;

    std::pair<bool, Vec2> checkSATCollision(const IShape& shapeA, const IShape& shapeB) const;
};