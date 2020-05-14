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
	
	void KeyPressed(Ui::Key key);

private:
	Snake snake_;
};


class Computer{
public:
	Computer() = default;
	~Computer() = default;

	Computer(const Computer&) = delete;
	Computer& operator=(const Computer&) = delete;
	
	virtual void Move() = 0;;
};


class Computer1 : public Computer{
public:
	Computer1(Game& game, TextUi::Color color = TextUi::BLUE);
	void Move();

private:
	Snake snake_;
	Game& game_;
};

class Computer2 : public Computer{
public:
	Computer2(Game& game, TextUi::Color color = TextUi::BLUE);
	void Move();

private:
	Snake snake_;
	Game& game_;
};

class Computer3 : public Computer{
public:
	Computer3(Game& game, TextUi::Color color = TextUi::BLUE);
	void Move();

private:
	Snake snake_;
	Game& game_;
};

class Computer4 : public Computer{
public:
	Computer4(Game& game, TextUi::Color color = TextUi::BLUE);
	void Move();
	void FillRouteXY(Vecti begin, Vecti end, std::list<Vecti>& route);
	void FillRouteYX(Vecti begin, Vecti end, std::list<Vecti>& route);
	void SetDir(std::list<Vecti>& my_route);

private:
	Snake snake_;
	Game& game_;
};

/*
class Computer2{
public:
	Computer(Game& game, int alg, TextUi::Color color = TextUi::BLUE);
	~Computer() = default;

	Computer(const Computer&) = delete;
	Computer& operator=(const Computer&) = delete;
	
	void Move();
	void Move2();
	void Move3();
	void Move4();
	void FillRouteXY(Vecti begin, Vecti end, std::list<Vecti>& route);
	void FillRouteYX(Vecti begin, Vecti end, std::list<Vecti>& route);
	void SetDir(std::list<Vecti>& my_route);

protected:
	Snake snake_;
	Game& game_;
};
*/