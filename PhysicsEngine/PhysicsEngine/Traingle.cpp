#include "Triangle.h"

Triangle::Triangle(RigidBody* body, const Vec2& a, const Vec2& b, const Vec2& c) : IShape(body)
{
	// repetition to avoid calling a virtual function in the constructor
	const Vec2 vertexArray[3] = { a, b, c };

	// rotate the vertices by the body's rotation angle and store them
	for (int i = 0; i < 3; ++i)
	{
		const float dx = vertexArray[i].x - body->position.x;
		const float dy = vertexArray[i].y - body->position.y;
		const float rotatedX = dx * cos(body->rotation) - dy * sin(body->rotation);
		const float rotatedY = dx * sin(body->rotation) + dy * cos(body->rotation);
		vertices[i] = { body->position.x + rotatedX, body->position.y + rotatedY };
	}
}

Vec2 Triangle::getEdge(int index) const
{
	return { (vertices[(index + 1) % 3].x - vertices[index].x), (vertices[(index + 1) % 3].y - vertices[index].y) };
}

Vec2 Triangle::updateVertices()
{
	const Vec2 vertexArray[3] = { vertices[0], vertices[1], vertices[2] };

	// rotate the vertices by the body's rotation angle and store them
	for (int i = 0; i < 3; ++i)
	{
		const float dx = vertexArray[i].x - body->position.x;
		const float dy = vertexArray[i].y - body->position.y;
		const float rotatedX = dx * cos(body->rotation) - dy * sin(body->rotation);
		const float rotatedY = dx * sin(body->rotation) + dy * cos(body->rotation);
		vertices[i] = { body->position.x + rotatedX, body->position.y + rotatedY };
	}

	return { 0, 0 };
}