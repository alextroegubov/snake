#include "control.h"
#include "game.h"
#include "text_ui.h"
#include <list>

#include <utility>

Player::Player(Game& game, TextUi::Color color = TextUi::RED):
		snake_(Snake(game.RandPos(), Snake::UP)){

	Ui::get()->OnKey(std::bind(&Player::KeyPressed, this, std::placeholders::_1));
	
	snake_.color = color;

	game.AddSnake(&snake_);
	game.GrowSnake(snake_);

	fprintf(Game::file, "Created Player, connected with snake[%p]\n", &snake_);
	fflush(Game::file);
}

void Player::KeyPressed(Ui::Key key){
	
	if(!this->snake_.is_dead){

		Snake::Dir dir = this->snake_.dir;

		switch(key){
			case Ui::UP:
				this->snake_.dir = (dir == Snake::DOWN)? dir : Snake::UP; 	 break;
			case Ui::DOWN: 	
				this->snake_.dir = (dir == Snake::UP)? dir : Snake::DOWN;    break;
			case Ui::LEFT: 	
				this->snake_.dir = (dir == Snake::RIGHT)? dir : Snake::LEFT; break;
			case Ui::RIGHT: 
				this->snake_.dir = (dir == Snake::LEFT)? dir : Snake::RIGHT; break;
		}
		fprintf(Game::file, "snake[%p], key pressed, changed dir to %d\n", &(this->snake_), this->snake_.dir);
	}

	fflush(Game::file);
}

Computer::Computer(Game& game,  int alg, TextUi::Color color /*= TextUi::BLUE*/):
		snake_(Snake(game.RandPos(), Snake::UP)),
		game_(game){
	
	switch(alg){
		case 1:
			Ui::get()->OnTimer(std::bind(&Computer::Move, this), 1);
			break;
		case 2:
			Ui::get()->OnTimer(std::bind(&Computer::Move2, this), 1);
			break;
		case 3:
			Ui::get()->OnTimer(std::bind(&Computer::Move3, this), 1);
			break;
		case 4:
			Ui::get()->OnTimer(std::bind(&Computer::Move4, this), 1);
			break;		
	}
	
	game.AddSnake(&snake_);
	game.GrowSnake(snake_);

	snake_.color = color;
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

	if(can_move[snake_.dir] && (snake_.dir == good_dir1 || snake_.dir == good_dir2)){
		return;
	}

	if(can_move[good_dir1])
		snake_.dir = good_dir1;

	else if(can_move[good_dir2])
		snake_.dir = good_dir2;

	else if(can_move[0])
		snake_.dir = Snake::UP;

	else if(can_move[1])
		snake_.dir = Snake::RIGHT;

	else if(can_move[2])
		snake_.dir = Snake::DOWN;
	
	else
		snake_.dir = Snake::LEFT;
}

void Computer::Move2(){

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

	std::function<bool(Vecti)> Ib = std::bind(&Game::IsBusy, &game_, std::placeholders::_1);
	std::function<bool(Vecti)> Ir = std::bind(&Game::IsRabbit, &game_, std::placeholders::_1);

	//0:UP, 1: RIGHT, 2:DOWN, 3:LEFT
	bool can_move[4] = { (!Ib({head.x - 1, head.y}) || Ir({head.x - 1, head.y})) &&
						 (!Ib({head.x - 2, head.y}) || Ir({head.x - 2, head.y})),
						 																				
						 (!Ib({head.x, head.y + 1}) || Ir({head.x, head.y + 1})) &&
						 (!Ib({head.x, head.y + 2}) || Ir({head.x, head.y + 2})),

						 (!Ib({head.x + 1, head.y}) || Ir({head.x + 1, head.y})) &&
						 (!Ib({head.x + 2, head.y}) || Ir({head.x + 2, head.y})),

						 (!Ib({head.x, head.y - 1}) || Ir({head.x, head.y - 1})) &&
						 (!Ib({head.x, head.y - 2}) || Ir({head.x, head.y - 2}))
					   };

	Snake::Dir good_dir1 = (nr.x <= head.x)? Snake::UP   : Snake::DOWN;
	Snake::Dir good_dir2 = (nr.y <= head.y)? Snake::LEFT : Snake::RIGHT;

	if(can_move[snake_.dir] && (snake_.dir == good_dir1 || snake_.dir == good_dir2)){
		return;
	}

	if(can_move[good_dir1])
		snake_.dir = good_dir1;

	else if(can_move[good_dir2])
		snake_.dir = good_dir2;

	else if(can_move[0])
		snake_.dir = Snake::UP;

	else if(can_move[1])
		snake_.dir = Snake::RIGHT;

	else if(can_move[2])
		snake_.dir = Snake::DOWN;
	
	else
		snake_.dir = Snake::LEFT;	
}

void Computer::Move3(){

	static Vecti target(1000, 1000);


	if(snake_.is_dead){
		return;
	}

	Vecti head(snake_.segments.front());

	if(!game_.IsRabbit(target)){

		target = game_.GetRabbit()[0].cs;

	}

	std::function<bool(Vecti)> Ib = std::bind(&Game::IsBusy, &game_, std::placeholders::_1);
	std::function<bool(Vecti)> Ir = std::bind(&Game::IsRabbit, &game_, std::placeholders::_1);

	//0:UP, 1: RIGHT, 2:DOWN, 3:LEFT
	bool can_move[4] = { (!Ib({head.x - 1, head.y}) || Ir({head.x - 1, head.y})) &&
						 (!Ib({head.x - 2, head.y}) || Ir({head.x - 2, head.y})),
						 																				
						 (!Ib({head.x, head.y + 1}) || Ir({head.x, head.y + 1})) &&
						 (!Ib({head.x, head.y + 2}) || Ir({head.x, head.y + 2})),

						 (!Ib({head.x + 1, head.y}) || Ir({head.x + 1, head.y})) &&
						 (!Ib({head.x + 2, head.y}) || Ir({head.x + 2, head.y})),

						 (!Ib({head.x, head.y - 1}) || Ir({head.x, head.y - 1})) &&
						 (!Ib({head.x, head.y - 2}) || Ir({head.x, head.y - 2}))
					   };


	Snake::Dir good_dir1 = (target.x <= head.x)? Snake::UP   : Snake::DOWN;
	Snake::Dir good_dir2 = (target.y <= head.y)? Snake::LEFT : Snake::RIGHT;

	if(can_move[snake_.dir] && (snake_.dir == good_dir1 || snake_.dir == good_dir2)){
		return;
	}

	if(can_move[good_dir1])
		snake_.dir = good_dir1;

	else if(can_move[good_dir2])
		snake_.dir = good_dir2;

	else if(can_move[0])
		snake_.dir = Snake::UP;

	else if(can_move[1])
		snake_.dir = Snake::RIGHT;

	else if(can_move[2])
		snake_.dir = Snake::DOWN;
	
	else
		snake_.dir = Snake::LEFT;
}

void Computer::FillRouteXY(Vecti begin, Vecti end, std::list<Vecti>& route){
	
	int x_step = (begin.x > end.x)? -1 : 1;

	if(begin.x == end.x)
		x_step = 0;

	for(int i = begin.x + x_step; i != end.x; i += x_step){
		route.push_back({i, begin.y});
	}

	int y_step = (begin.y > end.y)? -1 : 1;
	
	if(begin.y == end.y)
		y_step = 0;

	for(int i = begin.y + y_step; i != end.y; i += y_step){
		route.push_back({end.x, i});
	}
}

void Computer::FillRouteYX(Vecti begin, Vecti end, std::list<Vecti>& route){
	int y_step = (begin.y > end.y)? -1 : 1;
	
	if(begin.y == end.y)
		y_step = 0;

	for(int i = begin.y + y_step; i != end.y; i += y_step){
		route.push_back({begin.x, i});
	}

	int x_step = (begin.x > end.x)? -1 : 1;

	if(begin.x == end.x)
		x_step = 0;

	for(int i = begin.x + x_step; i != end.x; i += x_step){
		route.push_back({i, end.y});
	}
}

void Computer::SetDir(std::list<Vecti>& my_route){

		Vecti dest = my_route.front();

		Vecti h(snake_.segments.front());

		Vecti delta(dest.x - h.x, dest.y - h.y);

		my_route.pop_front();

		if(delta == Vecti(1, 0))
			snake_.dir = Snake::DOWN;
		else if(delta == Vecti(-1,0))
			snake_.dir = Snake::UP;
		else if(delta == Vecti(0,1))
			snake_.dir = Snake::RIGHT;
		else 
			snake_.dir = Snake::LEFT;
}

bool Compare(const Vecti& h, const Vecti& a, const Vecti& b){
	return (h.ComputeDistance(a) < h.ComputeDistance(b));
}

void Computer::Move4(){
	
	if(snake_.is_dead)
		return;

	const std::function<bool(Vecti)> Ib = std::bind(&Game::IsBusy, &game_, std::placeholders::_1);
	const std::function<bool(Vecti)> Ir = std::bind(&Game::IsRabbit, &game_, std::placeholders::_1);	

	Vecti h(snake_.segments.front());

	std::vector<Vecti> my_rabbits;

	static std::list<Vecti> my_route;
	static Vecti target(500, 500);	

	bool good_route = true;

	for(auto& cell: my_route){
		if(!Ib(cell) || Ir(cell)){
			good_route = false;
		}
	}

	if(Ir(target) && good_route && !my_route.empty()){	
		SetDir(my_route);
		return;
	}

	for(const auto& r: game_.GetRabbit()){
		my_rabbits.push_back(r.cs);
	}

	if(my_rabbits.empty()){
		Move();
		return;
	}

	std::function<bool(const Vecti& a, const Vecti& b)> compare = std::bind(Compare, h, std::placeholders::_1, std::placeholders::_2);	

	if(my_rabbits.size() > 3)
		my_rabbits.resize(3);

	std::sort(my_rabbits.begin(), my_rabbits.end(), compare);


	for(auto& item: my_rabbits){

		target = item;
		FillRouteXY(h, target, my_route);
		good_route = true;

		for(auto& cell: my_route){
			if(!Ib(cell) || Ir(cell)){
				good_route = false;
			}
		}
/*
		if(!good_route){

			FillRouteYX(h, target, my_route);
			good_route = true;

			for(auto& cell: my_route){
				if(!Ib(cell) || Ir(cell)){
					good_route = false;
				}
			}
		}
*/
		if(!good_route)
			continue;
		else{
			SetDir(my_route);
			return;
		}
	}
	Move();
}
