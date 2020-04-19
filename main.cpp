#include <memory>
#include <iostream>
#include <chrono>
#include <thread>

#include "text_ui.h"
#include "game.h"
#include "control.h"

int main(){

	std::unique_ptr<ui> my_ui(ui::get("text_ui"));
	//calls delete automatically

	Game my_game;

//	my_game.RandomInit(2, 10);

	Player player1(my_game);
	Player player2(my_game);
	Player player3(my_game);
//	Computer comp1(my_game);
//	Computer comp2(my_game);
	
	my_ui->Run(my_game);
	
	return 0;
}
