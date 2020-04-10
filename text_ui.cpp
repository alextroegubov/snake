#include "text_ui.h"
#include "game.h"
#include "units.h"

#include <cassert>
#include <string>
#include <signal.h>
#include <poll.h>
#include <unistd.h>
#include <iostream>
#include <termios.h>

#include <algorithm>
#include <chrono>
#include <thread>

//ok
TextUi::TextUi(){
	ClearScreen();
	DrawBoarder();
	fflush(stdout);
	is_done = false;
	InitTextUi();
}

TextUi::~TextUi(){
	;
}

//ok
void TextUi::ClearScreen(){
    std::printf("\e[H\e[J");
}

//ok
void TextUi::PutC(const Vecti& v, const char c){
	assert(v.x > 0);
	assert(v.y > 0);
	assert(v.x <= win_sz.ws_row);
	assert(v.y <= win_sz.ws_col);

	std::printf("\e[%d;%dH%c", v.x, v.y, c);
}

//ok
void TextUi::DrawVLine(const Vecti& v, const int len) const{
	assert(v.x > 0);
	assert(v.y > 0);
	assert(v.x <= win_sz.ws_row);
	assert(v.y <= win_sz.ws_col);
	assert(len > 0);

	std::printf("\e[%d;%dH", v.x, v.y);

	std::string str = (v.y == win_sz.ws_col)? "" : "\e[1D";

	for(auto i = 0; i < len; i++){
		std::printf("|\e[1B%s", str.c_str());
	}

}

//ok
void TextUi::DrawHLine(const Vecti& v, const int len) const{
	assert(v.x > 0);
	assert(v.y > 0);
	assert(v.x <= win_sz.ws_row);
	assert(v.y <= win_sz.ws_col);
	assert(len > 0);

	std::printf("\e[%d;%dH", v.x, v.y);

	for(auto i = v.x; i < v.x + len; i++){
		std::printf("-");
	}
}

//ok
void TextUi::DrawBoarder(){
	ioctl(0, TIOCGWINSZ, &win_sz);

	//putting corners
	PutC({1, 1}, '+');
	PutC({1, win_sz.ws_col}, '+');
	PutC({win_sz.ws_row, 1}, '+');
	PutC({win_sz.ws_row, win_sz.ws_col}, '+');

	DrawHLine({1, 2}, win_sz.ws_col - 2);
	DrawHLine({win_sz.ws_row, 2}, win_sz.ws_col - 2);

	DrawVLine({2, 1}, win_sz.ws_row - 2);
	DrawVLine({2, win_sz.ws_col}, win_sz.ws_row - 2);

}
/*
int TextUi::ConvertToKey(char c){
	switch(c){
		case 'w':
			return ui::Key::UP;
		case 'd':
			return ui::Key::RIGHT;
		case 'a':
			return ui::Key::LEFT;
		case 's':
			return ui::Key::DOWN;
	}
}
*/
bool TextUi::GetEvent(){
	char entered_symb = getchar();
	ui::Key key;

	switch(entered_symb){
		case 'w':
			key = ui::Key::UP;
			break;
		case 'd':
			key = ui::Key::RIGHT;
			break;
		case 'a':
			key = ui::Key::LEFT;
			break;
		case 's':
			key = ui::Key::DOWN;
			break;
	}

	for(const auto& f: event_funcs){
		f(key);
	}

	return true;
}

void TextUi::Run(Game& my_game){
	//std::this_thread::sleep_for(std::chrono::milliseconds(10000));
	while(!is_done){
		Draw(my_game);
		fprintf(Game::file, "*****new iteration:\n");

		fprintf(Game::file, "busy cells are: \n");

		for(const auto& cell: my_game.busy_cells){
			fprintf(Game::file, "\t (%2d, %2d)\n", cell.x, cell.y);
		}

		struct pollfd poll_struct[1];
		poll_struct[1].fd = STDIN_FILENO; //std input
		poll_struct[1].events = POLLIN; //there is data to read;

//		struct timespec t1, t2;

//		clock_gettime(CLOCK_REALTIME, &t1);
		int has_event = poll(poll_struct, 1, static_cast<int>(Game::Settings::TICK));
//		clock_gettime(CLOCK_REALTIME, &t2);
/*		
		if(has_event){
			if(!GetEvent()){
				std::cout << "Error with poll \n";
			}
		}
*/
		//in ms
//		int passed_interval  = (t2.tv_sec - t1.tv_sec) * 1000 + (t2.tv_nsec - t1.tv_nsec) / 1000000 + 1;
//		printf("interal = %d\n", passed_interval);

//		if(passed_interval >= static_cast<int>(Game::Settings::TICK)){
			if(time_funcs.size() != 0){
				for(const auto& f: time_funcs){
					f();
				}
			}
//		}



	}	
}
	
//ok
void TextUi::InitTextUi(){
	struct termios sets;

	tcgetattr(0, &sets);
	
	//ICANON, ECHO - flags
	sets.c_lflag &= ~ICANON;
	sets.c_lflag &= ~ECHO;

	tcsetattr(STDIN_FILENO, TCSANOW, &sets);
	//TCSANOW - changes are applied immediately.
}

//ok
void TextUi::Draw(const Game& my_game){
	ClearScreen();
	DrawBoarder();

	for(const auto& item: my_game.GetRabbit()){
		Painter(item);
	}

	for(const auto& item: my_game.GetSnakes()){
		Painter(item);
	}

	fflush(stdout);
	GoToxy({win_sz.ws_row + 1, 1});
}

//ok
void TextUi::GoToxy(const Vecti& v){
	assert(v.x > 0);
	assert(v.y > 0);

	std::printf("\e[%d;%dH", v.x, v.y);
}

//ok
void TextUi::Painter(const Snake& s){
	char head = "A>V<"[s.dir];

	for(const auto& seg: s.segments){
		if(s.segments.front() == seg)
			PutC(seg, head);
		else
			PutC(seg, '#');
	}
}

//ok
void TextUi::Painter(const Rabbit& r){
	PutC({r.cs.x, r.cs.y}, '@');
}

/*
void TextUi::ClearObj(const Game& my_game){
	for(const auto& item: my_game.GetSnakes()){
		if(!item.is_dead){
			PutC(item.segments.front(),' ');
			PutC(item.segments.back(), ' ');
		}
	}
}
*/
//ok
void TextUi::OnTimer(int period, TimeFunc func){
	time_funcs.push_back(func);
	//std::sort(time_funcs.begin(), time_funcs.end());
}
//ok
void TextUi::OnKey(EventFunc func){
	event_funcs.push_back(func);
}
