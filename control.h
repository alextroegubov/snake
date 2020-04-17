#pragma once

#include "game.h"
#include "ui.h"

class Player{
public:
	Player(Game& game);
	void KeyPressed(ui::Key key);

private:
	Snake* snake_;
};


class Computer{
public:
	Computer(Game& game);
	void Move();

protected:
	Snake* snake_;
	Game& game_;
};
