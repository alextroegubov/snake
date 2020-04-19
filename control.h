#pragma once

#include "game.h"
#include "ui.h"

class Player{
public:
	Player(Game& game);
	~Player() = default;

	Player(const Player&) = delete;
	Player& operator=(const Player&) = delete;
	
	void KeyPressed(ui::Key key);

private:
	Snake snake_;
};


class Computer{
public:
	Computer(Game& game);
	~Computer() = default;

	Computer(const Computer&) = delete;
	Computer& operator=(const Computer&) = delete;
	
	void Move();

protected:
	Snake snake_;
	Game& game_;
};
