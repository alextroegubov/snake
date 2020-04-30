#pragma once
#include <functional>
#include <sys/ioctl.h>
#include "game.h"

class ui{

public:
	enum Key{
		UP,
		DOWN,
		LEFT,
		RIGHT		
	};

	struct TimeFunc{

		explicit TimeFunc(std::function<void(void)> f, int nt, int ntl):
				func(f),
				n_tick(nt),
				n_tick_left(ntl){
		}

		TimeFunc(TimeFunc&& tf):
				func(tf.func),
				n_tick(tf.n_tick),
				n_tick_left(tf.n_tick_left){
		}
		
		const std::function<void(void)> func;
		int n_tick;
		int n_tick_left;
	};

	using EventFunc = std::function<void(Key)>;
	//using TimeFunc = std::function<void(void)>;

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
	virtual void OnTimer(std::function<void(void)> func, int n_tick) = 0;

	virtual void OnKey(EventFunc func) = 0;	

	virtual void DrawBoarder() = 0;

	virtual void Finish() = 0;

	virtual void Exit() = 0;

	virtual void Pause() = 0;

protected:
	//current terminal size
	struct winsize win_sz;
	//Event subscribers
	std::vector<EventFunc> event_funcs;
	//timer subscribers
	std::vector<TimeFunc> time_funcs;
	//== true - game has been finished
	bool is_done;
	//== true - game has been paused
	bool is_paused;

};
