#pragma once
#include <cmath>

#include "Vec2.h"

class RigidBody
{
public:
    RigidBody(float mass, float cor, float inertia)
        : position(), velocity(), force(), rotation(0), angularVelocity(0), torque(0), inertia(inertia),
        cor(cor)
    {
        this->mass = isinf(1.f / mass) ? 0.f : 1.f / mass; // store inverse of mass, 0 for static bodies
    }

    Vec2 position, velocity, force;
    float mass, rotation, angularVelocity, torque, inertia, cor; // cor: coefficient of restitution [0;1] affects how bouncy the object is, 0 means no bounce, 1 means perfect bounce
};
