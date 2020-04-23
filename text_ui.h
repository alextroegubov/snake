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

class TextUi : public ui{
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
	
	//constructor
	TextUi();
	//destructor
	~TextUi();
	
	TextUi(const TextUi&) = delete;

	TextUi& operator=(const TextUi&) = delete;

	//returns y size of the window
    int GetWinY() const{ return win_sz.ws_col; }
	//returns x size of the window
    int GetWinX() const{ return win_sz.ws_row; }
	//draws boarder according to the window size
	void DrawBoarder();

	void Run(Game& my_game);
	
	//subscriber pattern
	void OnTimer(int period, TimeFunc func);
	void OnKey(EventFunc func);

	//draws game
	void Draw(Game& my_game);

	//painters
	void Painter(const Snake& s);
	void Painter(const Rabbit& r);

	void Finish();

private:
	void DrawVLine(const Vecti&, const int len) const;
	void DrawHLine(const Vecti&, const int len) const;
	//clears window
	void ClearScreen();
	//putc char <c> at (<x>, <y>) at the window
	void PutC(const Vecti&, const char c);
	void GoToxy(const Vecti&);
	bool GetEvent();
	void InitTextUi();
	void PrintScore(uint16_t score, uint8_t n);

	static void WinchHandler(int sign);
	static void TermHandler(int sign);


private:
	struct winsize win_sz;
	std::vector<EventFunc> event_funcs;
	std::vector<TimeFunc> time_funcs;
	bool is_done;
};



