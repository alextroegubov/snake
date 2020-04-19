#include "game.h"
#include "ui.h"

#include <random>
#include <vector>
#include <functional>
#include <iostream>
#include <cassert>
#include <ctime>
#include <cstdio>

FILE *Game::file = nullptr;

//ok
Game::Game():
		n_rabbits(0),
		rabbits({}){

	if(ui::get() != nullptr){
		ui::get()->OnTimer(TICK, std::bind(&Game::Move, this));
		SetSize({ui::get()->GetWinX(), ui::get()->GetWinY()});
	}
	else{
		printf("Can't subscribe OnTimer \n");
	}

	srand(std::time(nullptr)); 
	file = fopen("log.txt","w");
}


Game::~Game(){}


void Game::SetSize(const Vecti& v){

	size.x = v.x;
	size.y = v.y;
}                


void Game::AddSnake(Snake* sk){

	snakes.push_back(sk);
	
	fprintf(file, "added snake[%p]\n", sk);
	fflush(Game::file);
}

/*
Snake& Game::AddSnake(){

	Vecti pos = std::move(RandPos());
	busy_cells.insert(pos);
	snakes.push_back(Snake(pos, Snake::Dir::UP));
	
	return snakes[snakes.size()-1];
}
*/

//FIXME
void Game::MoveSnake(Snake& sk){

	Vecti new_head(sk.segments.front());
		
	switch(sk.dir){
		case sk.UP: 	new_head.x -= 1; break;
		case sk.RIGHT: 	new_head.y += 1; break;
		case sk.DOWN: 	new_head.x += 1; break;	
		case sk.LEFT: 	new_head.y -= 1; break;
	}

	if(!IsBusy(new_head)){

		busy_cells.erase(sk.segments.back());
		sk.segments.pop_back();

		sk.segments.push_front(new_head);
		busy_cells.insert(new_head);
	}
	else if(IsRabbit(new_head)){

		RemoveRabbit(new_head);
		//FIXME copypaste
		busy_cells.erase(sk.segments.back());
		sk.segments.pop_back();

		sk.segments.push_front(new_head);
		busy_cells.insert(new_head);
	
		GrowSnake(sk);		
	}
	else{	
		sk.is_dead = true;
	}
}


void Game::GrowSnake(Snake& sk){
	
	Vecti d(0,0);
	int size = sk.segments.size();

	Vecti end(sk.segments.back());

	if(size == 1){ //only head
		switch(sk.dir){
			case Snake::Dir::UP: 	d.x = +1; break;
			case Snake::Dir::DOWN:  d.x = -1; break;
			case Snake::Dir::LEFT:  d.y = +1; break;
			case Snake::Dir::RIGHT: d.y = -1; break;
		}
	}
	else{
		//the last two segments
		sk.segments.pop_back();

		Vecti pre_end(sk.segments.back());
		sk.segments.push_back(end);

		if(end.y == pre_end.y)
			d.x = (end.x > pre_end.x)? +1 : -1;

		else
			d.y = (end.y > pre_end.y)? +1 : -1;
	}

	Vecti new_seg(end.x + d.x, end.y + d.y);
	sk.segments.push_back(new_seg);
	busy_cells.insert(new_seg);		
}

//ok
void Game::Move(){

	for(auto& sk_p: this->snakes){
		if(!sk_p->is_dead){
			this->MoveSnake(*sk_p);
		}
	}

	if((std::rand() % 100) < static_cast<int>(this->Settings::RABBIT_CHANCE)){
		this->AddRabbit();
	}
}


//returns random vector in x in [3, size.x - 2]
//						   y in [3, size.y - 2]
std::random_device rd;
std::mt19937 gen(rd());

//ok
Vecti Game::RandPos(){

	Vecti pos(0, 0);
	std::uniform_int_distribution<> xgen(3, size.x - 2);
	std::uniform_int_distribution<> ygen(3, size.y - 2);

	do{
		pos.x = xgen(gen);
		pos.y = ygen(gen);
	}
	while(busy_cells.count(pos) == 1);

	busy_cells.insert(pos);
	
	return Vecti(pos);
}

void Game::AddRabbit(){

	if(n_rabbits >= MAX_N_RABBIT)
		return;
	
	rabbits[n_rabbits].is_dead = false;
	rabbits[n_rabbits].cs = RandPos();
	n_rabbits++;
}

//FIXME
void Game::RemoveRabbit(const Vecti& v){
	for(auto& r: rabbits){
		if(r.cs == v){
			r.is_dead = true;

			Rabbit tmp_rabbit(rabbits[n_rabbits - 1]);
			rabbits[n_rabbits - 1] = r;
			r = tmp_rabbit;

			busy_cells.erase(v); //?!

			n_rabbits--;
			return;
		}
	}
}


bool Game::IsRabbit(const Vecti& v) const{
	
	for(auto& r: rabbits){
		if(r.cs == v)
			return true;
	}
	return false;
}


bool Game::IsBusy(const Vecti& v) const{

	if (busy_cells.count(v) == 0  && v.x != 1 && v.x != size.x && 
		v.y != 1 && v.y != size.y)
		
		return false;
	
	return true;
}

/*
void Game::RandomInit(const int n_snakes, const int n_rabbits){

	for(auto i = 0; i < n_snakes; i++){
		AddSnake();
	}
	
	for(auto i = 0; i < n_rabbits; i++){	
		AddRabbit();
	}
}
*/