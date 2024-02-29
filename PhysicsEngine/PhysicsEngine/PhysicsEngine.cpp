#include "PhysicsEngine.h"

void PhysicsEngine::update(float deltaTime) const
{
    // update rigid bodies
    for (RigidBody* body : rigidBodies) {
        if (body->mass == 0.f) continue; // skip static bodies
        body->velocity.x += (body->force.x / body->mass) * deltaTime;
        body->velocity.y += (body->force.y / body->mass) * deltaTime;
        body->position.x += body->velocity.x * deltaTime;
        body->position.y += body->velocity.y * deltaTime;
        body->angularVelocity += (body->torque / body->inertia) * deltaTime;
        body->rotation += body->angularVelocity * deltaTime;
    }

    // update springs
	for (const Spring* spring : springs) {
		const Vec2 delta = { spring->bodyB->position.x - spring->bodyA->position.x, spring->bodyB->position.y - spring->bodyA->position.y };
		const float distance = sqrt(delta.x * delta.x + delta.y * delta.y);
    		float forceMagnitude = (distance - spring->restLength) * spring->stiffness;
    		forceMagnitude += (spring->bodyB->velocity.x - spring->bodyA->velocity.x) * delta.x * spring->damping;
    		forceMagnitude += (spring->bodyB->velocity.y - spring->bodyA->velocity.y) * delta.y * spring->damping;
    		forceMagnitude /= distance;
		const Vec2 force = { delta.x * forceMagnitude, delta.y * forceMagnitude };
    		spring->bodyA->force.x += force.x;
    		spring->bodyA->force.y += force.y;
    		spring->bodyB->force.x -= force.x;
    		spring->bodyB->force.y -= force.y;
	}

    for (IShape* shape : shapes)
    {
		shape->updateVertices();
	}

	 // update collisions using an iterative solver
    for (int iteration = 0; iteration < collisionIterations; ++iteration) {
		for (size_t i = 0; i < shapes.size() - 1; ++i) {
			for (size_t j = i + 1; j < shapes.size(); ++j) {
				const std::pair<bool, Vec2> collision = checkSATCollision(*shapes[i], *shapes[j]);
				if (collision.first) {
					const Vec2 collisionDirection = collision.second;
					const float penetration = std::sqrt(collisionDirection.x * collisionDirection.x + collisionDirection.y * collisionDirection.y);

					// resolve penetration
					const float totalMassInverse = shapes[i]->body->mass + shapes[j]->body->mass;
                    if (totalMassInverse == 0.f) continue; // skip static bodies
                    Vec2 correction = collisionDirection; // constexpr float percent = 0.8f; collisionDirection * (penetration * percent / totalMassInverse);
					shapes[i]->body->position -= correction * shapes[i]->body->mass / totalMassInverse;
					shapes[j]->body->position += correction * shapes[j]->body->mass / totalMassInverse;

					// apply impulses (adjusted for elasticity)
					const float e = std::min(shapes[i]->body->cor, shapes[j]->body->cor);
                    Vec2 normal = collisionDirection;
                    if (penetration != 0.f)
                    {
						normal.x /= penetration;
						normal.y /= penetration;
                    }
					const float relativeVelocity = Vec2::dotProduct(normal, shapes[j]->body->velocity - shapes[i]->body->velocity);
					if (relativeVelocity > 0) continue; // objects are moving apart

					const float impulse = -(1 + e) * relativeVelocity;
					Vec2 impulseVector = normal * impulse;
					shapes[i]->body->velocity -= impulseVector * shapes[i]->body->mass / totalMassInverse;
					shapes[j]->body->velocity += impulseVector * shapes[j]->body->mass / totalMassInverse;

                    for (IShape* shape : shapes)
                    {
                        shape->updateVertices();
                    }
				}
			}
		}
	}

	// reset forces and torque after applying them
    for (RigidBody* body : rigidBodies) {
        body->force = { 0.0f, 0.0f };
        body->torque = 0.0f;
    }
}

void PhysicsEngine::addShape(IShape* shape) {
    shapes.push_back(shape);
}

void PhysicsEngine::addRigidBody(RigidBody* body) {
    rigidBodies.push_back(body);
}

void PhysicsEngine::addSpring(Spring* spring) {
    springs.push_back(spring);
}

std::pair<bool, Vec2> PhysicsEngine::checkSATCollision(const IShape& shapeA, const IShape& shapeB) const
{
    float minOverlap = std::numeric_limits<float>::infinity();
    Vec2 mtv{};
    int minOverlapShapeIndex = 0;

    // check for overlap along the axes of each shape's edges
    for (int shapeIndex = 0; shapeIndex < 2; ++shapeIndex) {
        const IShape& shape = (shapeIndex == 0) ? shapeA : shapeB;
        const int edgeCount = shape.getEdgeCount();

        for (int edgeIndex = 0; edgeIndex < edgeCount; ++edgeIndex) {
            const Vec2 edge = shape.getEdge(edgeIndex);
            Vec2 axis = { -edge.y, edge.x };
			axis.normalize();

            // project vertices onto the axis and find the minimum and maximum
            float minA = std::numeric_limits<float>::infinity();
            float maxA = -std::numeric_limits<float>::infinity();
            float minB = std::numeric_limits<float>::infinity();
            float maxB = -std::numeric_limits<float>::infinity();

            for (int i = 0; i < shapeA.getEdgeCount(); ++i) {
                const Vec2 vertex = shapeA.getVertex(i);
                float projection = vertex.x * axis.x + vertex.y * axis.y;
                minA = std::min(minA, projection);
                maxA = std::max(maxA, projection);
            }

            for (int i = 0; i < shapeB.getEdgeCount(); ++i) {
                const Vec2 vertex = shapeB.getVertex(i);
                float projection = vertex.x * axis.x + vertex.y * axis.y;
                minB = std::min(minB, projection);
                maxB = std::max(maxB, projection);
            }

            // check for overlap along the current axis
            if (minA > maxB || minB > maxA) {
                return { false, mtv };
            } else {
	            const float overlap = std::min(maxA, maxB) - std::max(minA, minB);
                if (overlap < minOverlap) {
                    minOverlap = overlap;
                    mtv = axis;
                    minOverlapShapeIndex = shapeIndex;
                }
            }
        }
    }

    // normalize MTV and scale by the minimum overlap
    mtv.normalize();
    mtv.x *= minOverlap;
    mtv.y *= minOverlap;

    // ensure MTV points from shapeA to shapeB
    const Vec2 shapeCenters = { shapeB.body->position.x - shapeA.body->position.x, shapeB.body->position.y - shapeA.body->position.y };
    if (Vec2::dotProduct(shapeCenters, mtv) < 0) {
        mtv.x = -mtv.x;
        mtv.y = -mtv.y;
    }

    return { true, mtv };
}
