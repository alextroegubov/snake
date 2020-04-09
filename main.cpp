#include <memory>
#include <iostream>
#include <chrono>
#include <thread>

#include "text_ui.h"
#include "game.h"

int main(){

//	TextUi my_ui;
	ui* my_ui = ui::get("text_ui");
	if(my_ui == nullptr){
		std::cout << "Trouble!!!" << std::endl;
	}

	Game my_game;

	my_game.SetSize({my_ui->GetWinX(), my_ui->GetWinY()});


	my_game.RandomInit(4, 4);

/* //works!
	my_ui->Draw(my_game);
	fflush(stdout);
	std::this_thread::sleep_for(std::chrono::milliseconds(3000));	
	dynamic_cast<TextUi*>(my_ui)->time_funcs[0]();
*/
	my_ui->Draw(my_game);
	std::this_thread::sleep_for(std::chrono::milliseconds(3000));

	my_game.AddRabbit();
	my_game.AddRabbit();
	my_game.AddRabbit();
	my_ui->Draw(my_game);
	std::this_thread::sleep_for(std::chrono::milliseconds(3000));

//	my_ui->Run(my_game);
	
	return 0;
}