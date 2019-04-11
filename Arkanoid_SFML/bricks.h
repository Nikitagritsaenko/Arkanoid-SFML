#pragma once
#include <SFML/Graphics.hpp>
#include "figures.h"
#include "bonuses.h"
#include <iostream>
using namespace std;
using namespace sf;

class Brick : public RectangleFigure
{
public:
	virtual ~Brick() {};
	virtual void action(unsigned &score, const map <string, float> &map, BonusVector* bonuses, Ball *ball) = 0;
	unsigned getHealth() { return health; }
	void setParametres(Vector2f &point, Vector2f &size);
	void beat() { health--; }
private:
	unsigned health;
};

class RegularBrick : public Brick
{
public:
	RegularBrick(Vector2f &point, Vector2f &size);
	void action(unsigned &score, const map <string, float> &map, BonusVector* bonuses, Ball *ball) { beat(); score++; }
};

class IndestructibleBrick : public Brick
{
public:
	IndestructibleBrick(Vector2f &point, Vector2f &size);
	void action(unsigned &score, const map <string, float> &map, BonusVector* bonuses, Ball *ball) {}
};

class BonusBrick : public Brick
{
public:
	BonusBrick(Vector2f &point, Vector2f &size);
	void action(unsigned &score, const map <string, float> &map, BonusVector* bonuses, Ball *ball)
	{
		beat();
		score++;
		float bonusR = map.find("BONUS_RADIUS")->second, bonusVel = map.find("BONUS_VELOCITY")->second;
		Vector2f bonusPoint(x(), y());
		bonuses->addBonus(bonusPoint, bonusR, bonusVel);
	}
};

class BoostBrick : public Brick
{
public:
	BoostBrick(Vector2f &point, Vector2f &size);
	void action(unsigned &score, const map <string, float> &map, BonusVector* bonuses, Ball *ball)
	{
		beat();
		score++;
		ball->accelerate(0.1f);
	}
};

class BricksVector
{
public:
	BricksVector(Vector2u &blockNum, Vector2f &size);
	~BricksVector();
	void push(Brick* brick) { brickList.push_back(brick); }
	vector <Brick*> getList() { return brickList; }
	void updateList();
private:
	vector <Brick*> brickList;
};
