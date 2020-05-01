#include <memory>
#include <iostream>
#include <chrono>
#include <thread>

#include "text_ui.h"
#include "game.h"
#include "control.h"

int main(){

	std::unique_ptr<Ui> my_ui(Ui::get("text_ui"));
	//calls delete automatically

	Game my_game;

	Player player1(my_game, TextUi::RED);

	Computer comp1(my_game, 1, TextUi::GREEN);
	Computer comp2(my_game, 2, TextUi::YELLOW);
	Computer comp3(my_game, 3, TextUi::BLUE);

	
	my_ui->Run(my_game);
	
	return 0;
}