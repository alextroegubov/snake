#include "game.h"
#include "text_ui.h"

#include <random>
#include <vector>
#include <functional>
#include <iostream>

Game::Game(){
	if(ui::get() != nullptr){
		ui::get()->OnTimer(Settings::TICK, std::bind(&Game::Move, this));
	}
	else
		printf("Can't subscribe OnTimer \n");
}

Game::~Game(){
	;
}

void Game::SetSize(const Vecti& v){
	size.x = v.x;
	size.y = v.y;

//	printf("x = %d, y = %d", size.x, size.y);
}

void Game::AddSnake(const Snake& s){
	//check coords
	snakes.push_back(s);
	for(const auto& item: s.segments){
		busy_cells.insert(item);
	}

}
/*
void Game::MoveSnake(Snake& sk){

	Vecti new_head(sk.segments.front());
		
	switch(sk.dir){
		case sk.UP:
			new_head.x -= 1;
			break;
		case sk.RIGHT:
			new_head.y += 1;
			break;
		case sk.DOWN:
			new_head.x += 1;
			break;
		case sk.LEFT:
			new_head.y -= 1;
			break;
	}
		
	if(busy_cells.count(new_head) == 0){
		sk.segments.pop_back();
		sk.segments.push_front(new_head);
	}
	else{
		sk.is_dead = true;
	}

}
*/

void Game::Move(){
	for(auto& sk: snakes){
		if(!sk.is_dead){
			//MoveSnake(sk);
			Vecti new_head(sk.segments.front());
		
			switch(sk.dir){
				case sk.UP: 	new_head.x -= 1; break;
				case sk.RIGHT: 	new_head.y += 1; break;
				case sk.DOWN: 	new_head.x += 1; break;
				case sk.LEFT: 	new_head.y -= 1; break;
			}
		
			if(this->busy_cells.count(new_head) == 0){
				this->busy_cells.erase(sk.segments.back());
				sk.segments.pop_back();

				sk.segments.push_front(new_head);
				this->busy_cells.insert(new_head);
			}
			else{
				sk.is_dead = true;
				printf("snake is dead\n");
			}
		}
	}
	if((std::rand() % 100) < static_cast<int>(this->Settings::RABBIT_CHANCE)){
		FILE *file = fopen("log", "a");
		fprintf(file, "add rabbit\n");
		this->AddRabbit();
	}
}

//returns random vector in x in [2, size.x - 1]
//						   y in [2, size.y - 1]
Vecti Game::RandPos(){
	Vecti pos(0, 0);

	do{
		pos.x = (std::rand() % (size.x - 2)) + 2;
		pos.y = (std::rand() % (size.y - 2)) + 2;
	}
	while(busy_cells.count(pos) == 1);
	
	return Vecti(pos);
}


void Game::AddRabbit(){
	if(rabbits.size() >= MAX_N_RABBIT)
		return;

	Vecti pos = RandPos();
	rabbits.push_back(Rabbit(pos));
	busy_cells.insert(pos);
}


void Game::RandomInit(const int n_snakes, const int n_rabbits){
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> xgen(3, size.x - 2);
	std::uniform_int_distribution<> ygen(3, size.y - 2);

	for(auto i = 0; i < n_snakes; i++){
		AddSnake(Snake(RandPos(), Snake::Dir::UP));
	}
	
	snakes.at(1).Grow();
	snakes.at(2).Grow();
	snakes.at(0).Grow();
	snakes.at(3).Grow();


	AddRabbit();
	AddRabbit();
	AddRabbit();
	AddRabbit();
}
