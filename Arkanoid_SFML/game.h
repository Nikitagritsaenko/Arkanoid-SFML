#pragma once
#include <fstream>
#include <sstream>
#include <vector>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include "bonuses.h"
#include "bricks.h"
using namespace std;
using namespace sf;

class Game
{
public:
	Game();
	~Game();
	void Pause();
	void Play(RenderWindow *window, Ball *ball, Paddle* paddle, BricksVector *bricks, BonusVector *bonuses, unsigned &lifes);
	bool GameEnd(RenderWindow *window, Ball *ball, Paddle* paddle, BricksVector *bricks, unsigned &lifes);
	bool UsersQueryProcessing();
	void update(RenderWindow *window, Ball *ball, Paddle* paddle, BricksVector *bricks, BonusVector *bonuses);
	void collideChecker(RenderWindow *window, Ball *ball, Paddle* paddle, BricksVector *bricks, BonusVector *bonuses);
private:
	unsigned score;
	unsigned lifes;
	RenderWindow* window;
	Ball* ball;
	Paddle* paddle;
	BricksVector* bricks;
	BonusVector* bonuses;
	map <string, float> map;
};