#include "figures.h"

Ball::Ball(Vector2f &point, float radius, float velocity)
{
	setVelocityX(-velocity);
	setVelocityY(-velocity);
	setVelocityModule(velocity);
	shape.setPosition(point.x, point.y);
	shape.setRadius(radius);
	shape.setFillColor(Color::Red);
	shape.setOrigin(radius, radius);
}

Paddle::Paddle(Vector2f &point, Vector2f size, float winW, float velocity)
{
	setSize(size);
	setVelocityX(velocity);
	setVelocityModule(velocity);
	shape.setPosition(point.x, point.y);
	shape.setSize(size);
	shape.setFillColor(Color::Magenta);
	shape.setOrigin(size / 2.f);
	Vector2f bottomPos = point, bottomSize = size;
	bottomPos.y += size.y;
	bottomSize.x = winW;
	shield = new Bottom(bottomPos, bottomSize);
}

Bottom::Bottom(Vector2f &point, Vector2f size)
{
	setSize(size);
	shape.setPosition(point.x, point.y);
	shape.setSize(size);
	shape.setFillColor(Color::White);
	shape.setOrigin(size/2.f);
	active = false;
}