#include "control.h"
#include "game.h"
#include "text_ui.h"

#include <utility>

Player::Player(Game& game):
		snake_(){

	ui::get()->OnKey(std::bind(&Player::KeyPressed, this, std::placeholders::_1));
	
	Vecti pos = game.RandPos();

	game.busy_cells.insert(pos);

	snake_.segments.push_front(pos);

	snake_.SetColor(TextUi::RED);

		
	fprintf(Game::file, "Created Player, connected with snake[%p]\n", &snake_);
	fflush(Game::file);
}

void Player::KeyPressed(ui::Key key){
	fprintf(Game::file, "snake[%p], key pressed\n", &(this->snake_));
	fflush(Game::file);

	if(!this->snake_.is_dead){

		Snake::Dir dir = this->snake_.dir;

		switch(key){
			case ui::UP:
				this->snake_.SetDirection(dir == Snake::DOWN? dir : Snake::UP); 	
				break;
			case ui::DOWN: 	
				this->snake_.SetDirection(dir == Snake::UP? dir : Snake::DOWN);
				break;
			case ui::LEFT: 	
				this->snake_.SetDirection(dir == Snake::RIGHT? dir : Snake::LEFT); 	
				break;
			case ui::RIGHT: 
				this->snake_.SetDirection(dir == Snake::LEFT? dir : Snake::RIGHT); 
				break;
		}
	}
}
/*
Computer::Computer(Game& game):
		game_(game),
		snake_(game.AddSnake()){
	
	ui::get()->OnTimer(Game::TICK, std::bind(&Computer::Move, this));
	snake_->SetColor(TextUi::GREEN);
}

void Computer::Move(void){

	if(!snake_->is_dead){
		return;
	}
	//find the nearest rabbit
	Vecti nr(1000,1000);

	Vecti head(snake_->segments.front());

	for(const auto& r: game_.GetRabbit()){
		if(r.is_dead == true)
			break;

		if(head.ComputeDistance(r.cs) < head.ComputeDistance(nr)){
			nr = r.cs;
		}	
	}
	//0:UP, 1: RIGHT, 2:DOWN, 3:LEFT
	bool can_move[4] = { !game_.IsBusy({head.x - 1, head.y}) || game_.IsRabbit({head.x - 1, head.y}),
						 !game_.IsBusy({head.x, head.y + 1}) || game_.IsRabbit({head.x, head.y + 1}),
						 !game_.IsBusy({head.x + 1, head.y}) || game_.IsRabbit({head.x + 1, head.y}),
						 !game_.IsBusy({head.x, head.y - 1}) || game_.IsRabbit({head.x, head.y - 1}),
					   };

	short int good_dir1 = (nr.x <= head.x)? Snake::UP   : Snake::DOWN;
	short int good_dir2 = (nr.y <= head.y)? Snake::LEFT : Snake::RIGHT;
m
	if(can_move[good_dir1])
		snake_->SetDirection(static_cast<Snake::Dir>(good_dir1));

	else if(can_move[good_dir2])
		snake_->SetDirection(static_cast<Snake::Dir>(good_dir2));

	else{
		;//add avoiding collisions
	}
}*/