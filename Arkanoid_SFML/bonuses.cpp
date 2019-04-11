#include "bonuses.h"

void Bonus::setParametres(Vector2f &point, float radius, float velocity)
{
	shape.setPosition(point);
	shape.setRadius(radius);
	shape.setFillColor(Color::Blue);
	shape.setOrigin(radius, radius);
	setVelocityY(velocity);
	setActivity(false);
}

void BonusVector::addBonus(Vector2f &point, float radius, float velocity)
{
	int r = rand() % BONUS_NUMBER;
	Bonus* bonus = NULL;
	if (r == PADDLE_SIZE_UP)
		bonus = new PaddleSizeUp(point, radius, velocity);
	else if (r == BALL_VELOCITY_UP)
		bonus = new BallVelocityUp(point, radius, velocity);
	else if (r == BOTTOM_BONUS)
		bonus = new BottomBonus(point, radius, velocity);
	else if (r == CHANGE_TRAJECTOTY)
		bonus = new ChangeTrajectory(point, radius, velocity);
	else if (r == STICK_BONUS)
		bonus = new StickBonus(point, radius, velocity);
	push(bonus);
}

void BonusVector::updateList()
{
	bonusList.erase(remove_if(begin(bonusList), end(bonusList),
		[](Bonus* bonus) {return bonus->toRemove(); }),
		end(bonusList));
}

BonusVector::~BonusVector()
{
	for (auto* bonus : bonusList)
		delete bonus;
	bonusList.clear();
}