#pragma once
#include <functional>
#include "game.h"

class ui{

public:
	enum Key{
		UP,
		DOWN,
		LEFT,
		RIGHT		
	};

	using EventFunc = std::function<void(Key)>;
	using TimeFunc = std::function<void(void)>;

	static ui* get(std::string type = "");
	static ui* sample;
	
	ui() = default;
	~ui(){sample = nullptr;}

	virtual void Run(Game&) = 0;

	virtual void Draw(Game&) = 0;

    virtual void ClearScreen() = 0;

    virtual int GetWinX() const = 0; //in cells

    virtual int GetWinY() const = 0; //in cells

    virtual void PutC(const Vecti& v, const char c) = 0;

	//painters
	virtual void Painter(const Snake& s) = 0;

	virtual void Painter(const Rabbit& r) = 0;
	
	//subscriber pattern
	virtual void OnTimer(int period, TimeFunc func) = 0;

	virtual void OnKey(EventFunc func) = 0;	

	virtual void DrawBoarder() = 0;

	virtual void Finish() = 0;

	virtual void Pause() = 0;

};
