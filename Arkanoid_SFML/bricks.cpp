#include "bricks.h"
void Brick::setParametres(Vector2f &point, Vector2f &size)
{
	setSize(size);
	shape.setPosition(point.x, point.y);
	shape.setSize(size);
	shape.setOrigin(size / 2.f);
	health = 1;
}

RegularBrick::RegularBrick(Vector2f &point, Vector2f &size)
{
	setParametres(point, size);
	shape.setFillColor(Color::Yellow);
}
IndestructibleBrick::IndestructibleBrick(Vector2f &point, Vector2f &size)
{
	setParametres(point, size);
	shape.setFillColor(Color::Cyan);
}
BoostBrick::BoostBrick(Vector2f &point, Vector2f &size)
{
	setParametres(point, size);
	shape.setFillColor(Color::Magenta);
}
BonusBrick::BonusBrick(Vector2f &point, Vector2f &size)
{
	setParametres(point, size);
	shape.setFillColor(Color::Green);
}

BricksVector::BricksVector(Vector2u &blockNum, Vector2f &size)
{
	for (unsigned i = 1; i <= blockNum.x; i++)
	{
		for (unsigned j = 1; j <= blockNum.y; j++)
		{
			Brick* brick;
			Vector2f bonusPoint(i*(size.x + 2) + 30, j*(size.y + 2));
			int r = rand() % 40;
			if (r == 0)
				brick = new IndestructibleBrick(bonusPoint, size);
			else if (r < 10)
				brick = new BonusBrick(bonusPoint, size);
			else if (r < 15)
				brick = new BoostBrick(bonusPoint, size);
			else
				brick = new RegularBrick(bonusPoint, size);
			push(brick);
		}
	}
}

void BricksVector::updateList()
{
	brickList.erase(remove_if(begin(brickList), end(brickList),
		[](Brick* brick) {return brick->getHealth() == 0; }),
		end(brickList));
}

BricksVector::~BricksVector()
{
	for (auto* brick : brickList)
		delete brick;
	brickList.clear();
}