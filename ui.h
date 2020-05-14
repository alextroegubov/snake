#pragma once
#include <functional>
#include <string>
#include <sys/ioctl.h>
#include "game.h"

class Ui{

public:
	enum Key{
		UP,
		DOWN,
		LEFT,
		RIGHT		
	};

	using EventFunc = std::function<void(Key)>;

	static Ui* get(std::string type = "");
	static Ui* sample;
	
	Ui() = default;
	~Ui(){sample = nullptr;}

	virtual void Run(Game&) = 0;

	virtual void Draw(const Game&) = 0;

    virtual int GetWinX() const = 0; //in cells

    virtual int GetWinY() const = 0; //in cells

	//painters
	virtual void Painter(const Snake& s) = 0;

	virtual void Painter(const Rabbit& r) = 0;

	virtual void PainterChange(const Snake& s) = 0;

	virtual void PainterErase(const Vecti& v) = 0;

	virtual void PainterScore(const Game& my_game) = 0;
	
	//subscriber pattern
	virtual void OnTimer(std::function<void(void)> func, int n_tick) = 0;

	virtual void OnKey(EventFunc func) = 0;	

	virtual void DrawBorder() = 0;

	virtual void Finish() = 0;

	virtual void Pause() = 0;
};
