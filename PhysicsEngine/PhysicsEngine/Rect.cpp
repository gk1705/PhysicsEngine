#include "Rect.h"

Rect::Rect(RigidBody* body, float width, float height) : IShape(body)
{
	// repetition to avoid calling a virtual function in the constructor
	// compute half-width and half-height
	const float halfWidth = width * 0.5f;
	const float halfHeight = height * 0.5f;

	// compute the four vertices of the rectangle
	for (int i = 0; i < 4; ++i)
	{
		const float dx = (i % 2 ? halfWidth : -halfWidth);
		const float dy = (i / 2 ? halfHeight : -halfHeight);
		const float rotatedX = dx * cos(body->rotation) - dy * sin(body->rotation);
		const float rotatedY = dx * sin(body->rotation) + dy * cos(body->rotation);
		vertices[i] = { body->position.x + rotatedX, body->position.y + rotatedY };
	}
}

Vec2 Rect::getEdge(int index) const
{
	return { (vertices[(index + 1) % 4].x - vertices[index].x), (vertices[(index + 1) % 4].y - vertices[index].y) };
}

Vec2 Rect::updateVertices()
{
	// compute half-width and half-height
	const float halfWidth = 0.5f * (vertices[1].x - vertices[0].x);
	const float halfHeight = 0.5f * (vertices[3].y - vertices[0].y);

	// compute the four vertices of the rectangle
	for (int i = 0; i < 4; ++i)
	{
		const float dx = (i % 2 ? halfWidth : -halfWidth);
		const float dy = (i / 2 ? halfHeight : -halfHeight);
		const float rotatedX = dx * cos(body->rotation) - dy * sin(body->rotation);
		const float rotatedY = dx * sin(body->rotation) + dy * cos(body->rotation);
		vertices[i] = { body->position.x + rotatedX, body->position.y + rotatedY };
	}

	return { halfWidth, halfHeight };
}