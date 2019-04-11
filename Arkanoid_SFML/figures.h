#pragma once
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
using namespace sf;

template <class T1, class T2> bool isCollide(T1* A, T2* B)
{
	return A->right() >= B->left() && A->left() <= B->right()
		&& A->bottom() >= B->top() && A->top() <= B->bottom();
}

class CircleFigure
{
public:
	CircleShape shape;
	virtual ~CircleFigure() {}
	void draw(RenderWindow* window) { window->draw(shape); }
	float x() { return shape.getPosition().x; }
	float y() { return shape.getPosition().y; }
	float left() { return x() - shape.getRadius(); }
	float right() { return x() + shape.getRadius(); }
	float top() { return y() - shape.getRadius(); }
	float bottom() { return y() + shape.getRadius(); }
};

class RectangleFigure
{
public:
	RectangleShape shape;
	virtual ~RectangleFigure() {}
	void draw(RenderWindow* window) { window->draw(shape); }
	float x() { return shape.getPosition().x; }
	float y() { return shape.getPosition().y; }
	float left() { return x() - shape.getSize().x / 2.f; }
	float right() { return x() + shape.getSize().x / 2.f; }
	float top() { return y() - shape.getSize().y / 2.f; }
	float bottom() { return y() + shape.getSize().y / 2.f; }
	void setSize(Vector2f newSize) { size = newSize; }
	void enlarge(float delta) { size.x += delta; }
	float getWidth() { return size.x; }
	float getHeight() { return size.y; }
private:
	Vector2f size;
};

class Ball : public CircleFigure
{
public:
	Ball(Vector2f &point, float radius, float velocity);
	void setVelocityX(float newVelocity) { velocity.x = newVelocity; }
	void setVelocityY(float newVelocity) { velocity.y = newVelocity; }
	void setVelocity(Vector2f newVelocity) { velocity = newVelocity; }
	void setVelocityModule(float newModule) { velocityModule = newModule; };
	void accelerate(float acceleration) { velocityModule += acceleration; };
	void redirectX() { velocity.x *= -1; }
	void redirectY() { velocity.y *= -1; }
	Vector2f getVelocity() { return velocity; }
	float getVelocityX() { return velocity.x; }
	float getVelocityY() { return velocity.y; }
	float getVelocityModule() { return velocityModule; }
private:
	Vector2f velocity;
	float velocityModule;
	float ballRadius;
};

class Bottom : public RectangleFigure
{
public:
	bool isActive() { return active; }
	void setActivity(bool var) { active = var; }
	Bottom(Vector2f &point, Vector2f size);
	void On() { setActivity(true); }
	void Off() { setActivity(false); }
private:
	bool active;
};

class Paddle : public RectangleFigure
{
public:
	Paddle(Vector2f &point, Vector2f size, float winW, float velocity);
	void setVelocityX(float newVelocity) { velocity.x = newVelocity; }
	void setVelocityModule(float newModule) { velocityModule = newModule; }
	Vector2f getVelocity() { return velocity; }
	float getVelocityModule() { return velocityModule; }
	Bottom* getBottom() { return shield; }
private:
	Bottom * shield;
	Vector2f velocity;
	float velocityModule;
};