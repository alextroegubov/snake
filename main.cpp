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

	my_game.RandomInit(2, 50);

	Player player1(my_game);
	
	my_ui->Run(my_game);
	
	return 0;
}
