#include "text_ui.h"

#include <cassert>
#include <string>
#include <csignal>
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
	DrawBorder();
	fflush(stdout);
	is_done = false;
	is_paused = false;
	InitTextUi();
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

	//signals:
//	signal(SIGWINCH, WinchHandler);
	signal(SIGTERM, TermHandler);
	signal(SIGINT, TermHandler);
	signal(SIGTSTP, TermHandler);
}

/*
void TextUi::WinchHandler(int sign){
	Ui::get()->Pause();
	Ui::get()->DrawBoarder();
}
*/

void TextUi::TermHandler(int sign){
	if(sign == SIGTSTP)
		Ui::get()->Pause();
	else
		Ui::get()->Finish();
}


void TextUi::Finish(){
	is_done = true;

	struct termios sets;
	tcgetattr(0, &sets);
	
	//ICANON, ECHO - flags
	sets.c_lflag |= ICANON;
	sets.c_lflag |= ECHO;

	tcsetattr(STDIN_FILENO, TCSANOW, &sets);

//	signal(SIGWINCH, SIG_DFL);
	signal(SIGTERM, SIG_DFL);
	signal(SIGINT, SIG_DFL);
	signal(SIGTSTP, SIG_DFL);
	//TCSANOW - changes are applied immediately
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
void TextUi::DrawBorder(){
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

bool TextUi::GetEvent(){
	char entered_symb = getchar();
	
	Ui::Key key;

	switch(entered_symb){
		case 'w': key = Ui::Key::UP; 	break;
		case 'd': key = Ui::Key::RIGHT; break;
		case 'a': key = Ui::Key::LEFT;	break;
		case 's': key = Ui::Key::DOWN;	break;

		case 'p': is_paused = !is_paused; 	return true;
		case 27 : is_done = true; 			return true;

		default: return true;
	}

	if(!is_paused){
		for(const auto& f: event_funcs){
			f(key);
		}
	}

	return true;
}

void TextUi::Run(Game& my_game){
	//std::this_thread::sleep_for(std::chrono::milliseconds(10000));

	int time_keeper = 0;
	Draw(my_game);

	while(!is_done){

		while(is_paused)
			GetEvent();

		struct pollfd poll_struct[1] = {};
		poll_struct[0].fd = STDIN_FILENO; //std input
		poll_struct[0].events = POLLIN; //there is data to read;

		struct timespec t1, t2;

		clock_gettime(CLOCK_REALTIME, &t1);
		int has_event = poll(poll_struct, 1, static_cast<int>(Game::Settings::TICK));
		clock_gettime(CLOCK_REALTIME, &t2);

		//in ms
		int passed_interval  = (t2.tv_sec - t1.tv_sec) * 1000 + (t2.tv_nsec - t1.tv_nsec) / 1000000 + 1;
		time_keeper += passed_interval;

		if(time_keeper >= static_cast<int>(Game::Settings::TICK)){

			time_keeper -= static_cast<int>(Game::Settings::TICK);

			for(auto& f: time_funcs){

				if(--f.n_tick_left == 0){
					f.func();
					f.n_tick_left = f.n_tick;
				}
			}
		}

		if(has_event){
			if(!GetEvent()){
				is_done = true;
			}
		}

		PainterScore(my_game);
		fflush(stdin);
	}

	Finish();
	fclose(Game::file);
	ShowResults(my_game);
}
	

void TextUi::ShowResults(const Game& game){
	
	ClearScreen();
	printf("Well done!\n\n");
	printf("Results:\n");

	int player_n = 1; 

	for(const auto& s: game.GetSnakes()){
		printf("\e[1;%dm", s->color);
		printf("\tPlayer %d: %3d points\n", player_n++, s->score);	
	}

	printf("\e[1;%dm", WHITE);
}

//ok
void TextUi::Draw(const Game& my_game){

	ClearScreen();
	DrawBorder();

//	my_game.SetSize({win_sz.ws_row, win_sz.ws_col});

	for(const auto& item: my_game.GetRabbit()){
		//all active rabbits are in the beginning of the array
		if(item.is_dead == true)
			break;

		Painter(item);
	}

	int snake_n = 0;
	//FIXME PainterScore
	for(auto& item: my_game.GetSnakes()){

		fprintf(Game::file, "drawing snake[%p]\n", item);
		fflush(Game::file);

		GoToxy({win_sz.ws_row, 4 + 4 * (snake_n++)});
		Painter(*item);
	}

	fflush(stdout);
	GoToxy({win_sz.ws_row + 1, 1});
}


void TextUi::PainterScore(const Game& my_game){
	
	int snake_n = 1;

	for(const auto& s: my_game.GetSnakes()){

		GoToxy({win_sz.ws_row, 4 * (snake_n++)});

		printf("\e[1;%dm", s->color);
		printf("%3u", s->score);
		printf("\e[1;%dm", WHITE);
	}
	
}


//ok
void TextUi::GoToxy(const Vecti& v){
	assert(v.x > 0);
	assert(v.y > 0);

	std::printf("\e[%d;%dH", v.x, v.y);
}

//ok
void TextUi::Painter(const Snake& s){

	printf("\e[1;%dm", s.color);

	printf("%3u", s.score);

	char head = "A>V<"[s.dir];

	for(const auto& seg: s.segments){
		if(s.segments.front() == seg)
			PutC(seg, head);
		else
			PutC(seg, '#');
	}
	
	printf("\e[1;%dm", WHITE);
	fflush(stdout);
}

void TextUi::PainterChange(const Snake& s){
	
	printf("\e[1;%dm", s.color);
	char head = "A>V<"[s.dir];

	PutC(s.segments.front(), head);
	PutC(*(++s.segments.begin()), '#'); 

	printf("\e[1;%dm", WHITE);
	fflush(stdout);
}

//ok
void TextUi::Painter(const Rabbit& r){
	printf("\e[1;%dm", PURPLE);
	PutC({r.cs.x, r.cs.y}, '@');
	printf("\e[0;%dm", WHITE);
	fflush(stdout);
}

//ok
void TextUi::OnTimer(std::function<void(void)> func, int n_tick){
	assert(n_tick > 0);
	
	TimeFunc time_func(func, n_tick, n_tick);
	time_funcs.push_front(std::move(time_func));
}


void TextUi::OnKey(EventFunc func){
	event_funcs.push_back(func);
}
