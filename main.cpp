#include <memory>
#include <iostream>
#include <chrono>
#include <thread>

#include "text_ui.h"
#include "game.h"

int main(){

	std::unique_ptr<ui> my_ui(ui::get("text_ui"));
	//calls delete automatically

	Game my_game;

	my_game.RandomInit(5, 50);

	my_ui->Run(my_game);
	
	return 0;
}
