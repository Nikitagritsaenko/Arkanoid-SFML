#pragma once
#include <vector>
#include "figures.h"
using namespace std;
using namespace sf;

enum bonus_t
{
	PADDLE_SIZE_UP,
	BALL_VELOCITY_UP,
	BOTTOM_BONUS,
	CHANGE_TRAJECTOTY,
	STICK_BONUS,
	BONUS_NUMBER
};

class Bonus : public CircleFigure
{
public:
	virtual ~Bonus() {};
	void setParametres(Vector2f &point, float radius, float velocity);
	bool isActive() { return active == 1; }
	bool toRemove() { return active == 2; }
	void setVelocityY(float newVelocity) { velocity.y = newVelocity; }
	void setActivity(char var) { active = var; }
	Vector2f getVelocity() { return velocity; }
	virtual void action(Ball *ball, Paddle *paddle) = 0;
protected:
	Vector2f velocity;
	char active;
};

class BonusVector
{
public:
	~BonusVector();
	void addBonus(Vector2f &point, float radius, float velocity);
	void push(Bonus* bonus) { bonusList.push_back(bonus); }
	void updateList();
	vector <Bonus*> getList() { return bonusList; }
private:
	vector <Bonus*> bonusList;
};

class PaddleSizeUp : public Bonus
{
public:
	PaddleSizeUp(Vector2f &point, float radius, float velocity) { setParametres(point, radius, velocity); }
	virtual void action(Ball *ball, Paddle *paddle)
	{
		setActivity(2);
		if (paddle->getWidth() > 400)
			return;
		paddle->enlarge(25.f);
		paddle->shape.setSize(Vector2f(paddle->getWidth(), paddle->getHeight()));
		paddle->shape.setOrigin(paddle->getWidth() / 2.f, paddle->getHeight() / 2.f);
	}
};

class BallVelocityUp : public Bonus
{
public:
	BallVelocityUp(Vector2f &point, float radius, float velocity) { setParametres(point, radius, velocity); }
	virtual void action(Ball *ball, Paddle *paddle) { ball->accelerate(0.4f); setActivity(2); }
};

class BottomBonus : public Bonus
{
public:
	BottomBonus(Vector2f &point, float radius, float velocity) { setParametres(point, radius, velocity); }
	virtual void action(Ball *ball, Paddle *paddle) {
		paddle->getBottom()->On();
		if (isCollide(paddle->getBottom(), ball))
		{
			ball->setVelocityY(-ball->getVelocityModule());
			paddle->getBottom()->Off();
			setActivity(2);
		}
	};
};

class ChangeTrajectory : public Bonus
{
public:
	ChangeTrajectory(Vector2f &point, float radius, float velocity) { setParametres(point, radius, velocity); }
	virtual void action(Ball *ball, Paddle *paddle) {
		if (Keyboard::isKeyPressed(Keyboard::Key::Numpad0)) {
			ball->setVelocityY(-ball->getVelocityModule());
			if (rand() % 2 != 0)
				ball->redirectX();
			setActivity(2);
		}
	}
};

class StickBonus : public Bonus
{
public:
	StickBonus(Vector2f &point, float radius, float velocity) { setParametres(point, radius, velocity); }
	StickBonus() { setActivity(1); }
	virtual void action(Ball *ball, Paddle *paddle) {
		if (isCollide(paddle, ball))
		{
			ball->setVelocity(paddle->getVelocity());
			if (Keyboard::isKeyPressed(Keyboard::Key::Space))
			{
				ball->setVelocityY(-ball->getVelocityModule());
				if (ball->x() < paddle->x())
					ball->setVelocityX(-ball->getVelocityModule());
				else
					ball->setVelocityX(ball->getVelocityModule());
				
				setActivity(2);
			}
		}
	};
};
