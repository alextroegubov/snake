#pragma once

#include "ui.h"
#include "game.h"

#include <cstdio>
#include <sys/ioctl.h>
#include <signal.h>
/*
		+---------------->y
		|
		|
		|
		|
		|
	   xV
				A UP
				|
		 LEFT<--|-->RIGHT
				|
				V DOWN
*/

class TextUi : public Ui{
public:
	//colors
	enum Color{
		RED = 31,
		GREEN = 32,
		YELLOW = 33,
		BLUE = 34,
		PURPLE = 35,
		WHITE = 37
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
	//constructor
	TextUi();
	//destructor
	~TextUi() = default;

	TextUi(const TextUi&) = delete;
	TextUi& operator=(const TextUi&) = delete;

	//returns y size of the window
    int GetWinY() const{ return win_sz.ws_col; }
	//returns x size of the window
    int GetWinX() const{ return win_sz.ws_row; }
	//draws boarder according to the window size
	void DrawBorder();
	//run game
	void Run(Game& my_game);
	//subscriber pattern
	void OnTimer(std::function<void(void)> func, int n_tick);
	void OnKey(EventFunc func);
	//draws game
	void Draw(const Game& my_game);
	//painters
	void Painter(const Snake& s);
	void Painter(const Rabbit& r);
	void PainterChange(const Snake& s);
	void PainterErase(const Vecti& v){ PutC(v, ' ');}
	void PainterScore(const Game& my_game);
	//resets made settings
	void Finish();
	//finishes game
	void Exit(){ is_done = true;}
	//pauses the game
	void Pause(){ is_paused = !is_paused;}
	//prints results on screen
	void ShowResults(const Game&);
private:
	//draw vertical line
	void DrawVLine(const Vecti&, const int len) const;
	//draw horizontal line
	void DrawHLine(const Vecti&, const int len) const;
	//clears window
	void ClearScreen();
	//putc char <c> at (<x>, <y>) at the window
	void PutC(const Vecti&, const char c);
	//set cursor at (x,y)
	void GoToxy(const Vecti&);
	//check for event and call subcribed functions
	bool GetEvent();
	//init text user interface
	void InitTextUi();
	//print score
	void PrintScore(uint16_t score, uint8_t n);
	//some signal handlers
	static void WinchHandler(int sign);
	static void TermHandler(int sign);

private:
	//current terminal size
	struct winsize win_sz;
	//Event subscribers
	std::vector<EventFunc> event_funcs;
	//timer subscribers
	std::list<TimeFunc> time_funcs;
	//== true - game has been finished
	bool is_done;
	//== true - game has been paused
	bool is_paused;

};



