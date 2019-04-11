#include "figures.h"
#include "game.h"

void::Game::update(RenderWindow *window, Ball *ball, Paddle* paddle, BricksVector *bricks, BonusVector *bonuses)
{
	Vector2u winSize = window->getSize();
	float winW = (float)winSize.x, winH = (float)winSize.y;
	//1. ball
	ball->shape.move(ball->getVelocity());
	if ((ball->left() <= 0 && ball->getVelocityX() < 0) || (ball->right() >= winW && ball->getVelocityX() > 0))
		ball->redirectX();
	if ((ball->top() <= 0 && ball->getVelocityY() < 0) || (ball->bottom() >= winH && ball->getVelocityY() < 0))
		ball->redirectY();
	//2. bonuses
	for (auto* bonus : bonuses->getList())
		bonus->shape.move(bonus->getVelocity());
	//3. paddle
	paddle->shape.move(paddle->getVelocity());
	if (Keyboard::isKeyPressed(Keyboard::Key::Left) && paddle->left() > 0)
		paddle->setVelocityX(-paddle->getVelocityModule());
	else if (Keyboard::isKeyPressed(Keyboard::Key::Right) && paddle->right() < winW)
		paddle->setVelocityX(paddle->getVelocityModule());
	else
		paddle->setVelocityX(0.f);
}

void Game::collideChecker(RenderWindow *window, Ball *ball, Paddle* paddle, BricksVector *bricks, BonusVector *bonuses)
{
	for (auto& brick : bricks->getList())
	{
		if (isCollide(brick, ball))
		{
			brick->action(score, map, bonuses, ball);
			float deltaLeft{ ball->right() - brick->left() };
			float deltaRight{ brick->right() - ball->left() };
			float deltaTop{ ball->bottom() - brick->top() };
			float deltaBottom{ brick->bottom() - ball->top() };
			float ballVelocity = abs(ball->getVelocityModule());
			float minDeltaX = min(abs(deltaLeft), abs(deltaRight));
			float minDeltaY = min(abs(deltaTop), abs(deltaBottom));
			if (minDeltaX < minDeltaY) //means we must change x-direction
			{
				if (abs(deltaLeft) == minDeltaX) //means that ball moved from leftside
					ball->setVelocityX(-ballVelocity);
				else //ball moved from rightside
					ball->setVelocityX(ballVelocity);
			}
			else //means we must change y-direction
			{
				if (abs(deltaTop) == minDeltaY) // ball moved from topside
					ball->setVelocityY(-ballVelocity);
				else // ball moved from bottomside
					ball->setVelocityY(ballVelocity);
			}
		}
	}

	for (auto& bonus : bonuses->getList())
		if (isCollide(paddle, bonus))
			bonus->setActivity(1);
	if (isCollide(paddle, ball))
	{
		if (ball->getVelocityY() > 0)
			ball->redirectY();
		if (ball->y() < paddle->y())
		{
			if (ball->x() < paddle->x())
				ball->setVelocityX(-abs(ball->getVelocityModule()));
			else
				ball->setVelocityX(abs(ball->getVelocityModule()));
		}
	}
}

Game::Game()
{
	srand((unsigned)time(NULL));
	score = 0;
	lifes = 3;
	//Read constants from file to map
	ifstream f("properties.txt");
	float n;
	string s;
	while (f >> s >> n)
		map.emplace(s, n);
	f.close();
	//Init objects
	Vector2f paddleSize, winSize;
	float ballR = map.find("BALL_RADIUS")->second, ballVel = map.find("BALL_VELOCITY")->second;
	float paddleVel = map.find("PADDLE_VELOCITY")->second;
	winSize.x = map.find("WINDOW_WIDTH")->second, winSize.y = map.find("WINDOW_HEIGHT")->second;
	paddleSize.x = map.find("PADDLE_WIDTH")->second, paddleSize.y = map.find("PADDLE_HEIGHT")->second;

	window = new RenderWindow(VideoMode((unsigned)winSize.x, (unsigned)winSize.y + 150), "Arkanoid");
	Vector2f ballPoint(winSize.x / 2, winSize.y / 2), paddlePoint(winSize.x / 2, winSize.y - 150);
	ball = new Ball(ballPoint, ballR, ballVel);
	paddle = new Paddle(paddlePoint, paddleSize, winSize.x, paddleVel);

	Vector2u bricksNum;
	bricksNum.x = (unsigned)map.find("BLOCK_COUNT_X")->second;
	bricksNum.y = (unsigned)map.find("BLOCK_COUNT_Y")->second;
	Vector2f blockSize;
	blockSize.x = map.find("BLOCK_WIDTH")->second, blockSize.y = map.find("BLOCK_HEIGHT")->second;

	bricks = new BricksVector(bricksNum, blockSize);
	bonuses = new BonusVector();
	window->setFramerateLimit(60);
	Play(window,ball,paddle,bricks,bonuses,lifes);
}

Game::~Game()
{
	delete ball;
	delete paddle;
	delete window;
	delete bonuses;
	delete bricks;
}

void Game::Pause()
{
	while (true)
	{
		if (Keyboard::isKeyPressed(Keyboard::Key::RShift))
			break;
	}
}

bool Game::GameEnd(RenderWindow *window, Ball *ball, Paddle* paddle, BricksVector *bricks, unsigned &lifes)
{
	Vector2u winSize = window->getSize();
	float winH = (float)winSize.y;
	float paddleH = paddle->getHeight();
	if (ball->bottom() >= paddleH + winH - 50)
	{
		lifes--;
		if (lifes == 0)
			return 1;
		Vector2f posPaddle = paddle->shape.getPosition();
		ball->shape.setPosition(posPaddle.x, posPaddle.y - paddleH);
		Bonus* bonus = new StickBonus();
		bonuses->push(bonus);
	}
	for (auto *brick : bricks->getList())
		if (brick->getHealth() != 0 && brick->shape.getFillColor() != Color::Cyan)
			return 0;
	return 1;
}

bool Game::UsersQueryProcessing()
{
	if (Keyboard::isKeyPressed(Keyboard::Key::Escape))
		return false;
	if (Keyboard::isKeyPressed(Keyboard::Key::RControl))
		Pause();
	return true;
}

void Game::Play(RenderWindow *window, Ball *ball, Paddle* paddle, BricksVector *bricks, BonusVector *bonuses, unsigned &lifes)
{
	Vector2u winSize = window->getSize();
	float winW = (float)winSize.x, winH = (float)winSize.y - 150;
	Font font;
	font.loadFromFile("font.otf");
	Text message("", font, 17);
	message.setFillColor(Color::White);
	message.setPosition(winW / 4.f, winH);
	string settings = "Esc - exit.\nRCtrl / RShift - pause / continue.\n'0' - trajectory bonus.\nspace - stick bonus.\n\nSCORE: ";
	string exit = "End of game. Press RShift to exit.\nSCORE: ";
	while (true)
	{
		window->clear(Color::Black);
		//1. check user's query
		if (UsersQueryProcessing() == false)
			break;
		//2. update all objects (move them)
		update(window, ball, paddle, bricks, bonuses);
		//3. check if objects collide
		collideChecker(window, ball, paddle, bricks, bonuses);
		//4. delete perished objects
		bricks->updateList();
		//5. draw all objects
		ball->draw(window);
		paddle->draw(window);
		if (paddle->getBottom()->isActive())
			paddle->getBottom()->draw(window);

		for (auto* bonus : bonuses->getList())
		{
			if (!bonus->isActive() && !bonus->toRemove())
				bonus->draw(window);
			if (bonus->isActive())
	 			bonus->action(ball, paddle);
		}
		bonuses->updateList();
		for (auto* brick : bricks->getList())
			brick->draw(window);
		ostringstream playerScoreString;
		ostringstream playerLifesString;
		playerScoreString << score;
		playerLifesString << lifes;
		message.setString(settings + playerScoreString.str() + ";  LIFES: " + playerLifesString.str());
		//6. check if game has ended
		if (GameEnd(window, ball, paddle, bricks, lifes))
		{
			message.setCharacterSize(30);
			message.setString(exit + playerScoreString.str());
			window->draw(message);
			window->display();
			Pause();
			break;
		}
		else
		{
			window->draw(message);
			window->display();
		}
		playerScoreString.clear();
		playerLifesString.clear();
	}
}

int main(void)
{
	Game* game = new Game();
	delete game;
	return 0;
}