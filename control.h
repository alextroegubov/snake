#pragma once

#include "game.h"
#include "ui.h"
#include "text_ui.h"

class Player{
public:
	Player(Game& game, TextUi::Color color/*= TextUi::RED*/);
	~Player() = default;

	Player(const Player&) = delete;
	Player& operator=(const Player&) = delete;
	
	void KeyPressed(ui::Key key);

private:
	Snake snake_;
};


class Computer{
public:
	Computer(Game& game, TextUi::Color color = TextUi::BLUE);
	~Computer() = default;

	Computer(const Computer&) = delete;
	Computer& operator=(const Computer&) = delete;
	
	void Move();

protected:
	Snake snake_;
	Game& game_;
};
