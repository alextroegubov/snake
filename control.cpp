#include "control.h"
#include "game.h"
#include "text_ui.h"
#include <utility>

Player::Player(Game& game):
	snake_(game.AddSnake()){
	
	ui::get()->OnKey(std::bind(&Player::KeyPressed, this, std::placeholders::_1));
	snake_->SetColor(TextUi::RED);
}


void Player::KeyPressed(ui::Key key){
	if(!snake_->is_dead){
		switch(key){
			case ui::UP: 	snake_->SetDirection(Snake::UP); 	break;
			case ui::DOWN: 	snake_->SetDirection(Snake::DOWN); 	break;
			case ui::LEFT: 	snake_->SetDirection(Snake::LEFT); 	break;
			case ui::RIGHT: snake_->SetDirection(Snake::RIGHT); break;
		}
	}
}