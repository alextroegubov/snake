#include "control.h"
#include "game.h"
#include "text_ui.h"

#include <utility>

Player::Player(Game& game, TextUi::Color color = TextUi::RED):
		snake_(Snake(game.RandPos(), Snake::UP)){

	ui::get()->OnKey(std::bind(&Player::KeyPressed, this, std::placeholders::_1));
	
	snake_.SetColor(color);

	game.AddSnake(&snake_);
	game.GrowSnake(snake_);

	fprintf(Game::file, "Created Player, connected with snake[%p]\n", &snake_);
	fflush(Game::file);
}

void Player::KeyPressed(ui::Key key){
	
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
		fprintf(Game::file, "snake[%p], key pressed, changed dir to %d\n", &(this->snake_), this->snake_.dir);
	}

	fflush(Game::file);
}

Computer::Computer(Game& game, TextUi::Color color /*= TextUi::BLUE*/):
		snake_(Snake(game.RandPos(), Snake::UP)),
		game_(game){
	
	ui::get()->OnTimer(Game::TICK, std::bind(&Computer::Move, this));
	game.AddSnake(&snake_);
	game.GrowSnake(snake_);

	snake_.SetColor(color);
	snake_.is_dead = false;

	fprintf(Game::file, "Created Computer, connected with snake[%p]\n", &snake_);
	fflush(Game::file);
}

void Computer::Move(){

	if(snake_.is_dead){
		return;
	}

	//find the nearest rabbit
	Vecti nr(1000,1000);

	Vecti head(snake_.segments.front());

	for(const auto& r: game_.GetRabbit()){
		if(r.is_dead == true)
			break;

		if(head.ComputeDistance(r.cs) < head.ComputeDistance(nr)){
			nr = r.cs;
		}	
	}
	//0:UP, 1: RIGHT, 2:DOWN, 3:LEFT
	bool can_move[4] = { (!game_.IsBusy({head.x - 1, head.y}) || game_.IsRabbit({head.x - 1, head.y})),
						 																				
						 (!game_.IsBusy({head.x, head.y + 1}) || game_.IsRabbit({head.x, head.y + 1})),

						 (!game_.IsBusy({head.x + 1, head.y}) || game_.IsRabbit({head.x + 1, head.y})),

						 (!game_.IsBusy({head.x, head.y - 1}) || game_.IsRabbit({head.x, head.y - 1}))
					   };

	Snake::Dir good_dir1 = (nr.x <= head.x)? Snake::UP   : Snake::DOWN;
	Snake::Dir good_dir2 = (nr.y <= head.y)? Snake::LEFT : Snake::RIGHT;

	if(can_move[good_dir1])
		snake_.SetDirection(good_dir1);

	else if(can_move[good_dir2])
		snake_.SetDirection(good_dir2);

	else if(can_move[0])
		snake_.SetDirection(Snake::UP);

	else if(can_move[1])
		snake_.SetDirection(Snake::RIGHT);

	else if(can_move[2])
		snake_.SetDirection(Snake::DOWN);
	
	else
		snake_.SetDirection(Snake::LEFT);
		//stay in previous direction
		;//add avoiding collisions
	
}