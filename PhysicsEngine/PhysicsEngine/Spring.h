#pragma once
#include "RigidBody.h"

class Spring
{
public:
    RigidBody* bodyA, * bodyB;
    float stiffness, damping, restLength;
};
