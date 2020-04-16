#include "game.h"
#include "text_ui.h"

#include <random>
#include <vector>
#include <functional>
#include <iostream>
#include <cassert>
#include <ctime>
#include <cstdio>

FILE *Game::file = nullptr;

//ok
Game::Game(){
	if(ui::get() != nullptr){
		ui::get()->OnTimer(Settings::TICK, std::bind(&Game::Move, this));
		SetSize({ui::get()->GetWinX(), ui::get()->GetWinY()});
	}
	else
		printf("Can't subscribe OnTimer \n");
	
	srand(std::time(nullptr));
	file = fopen("log.txt","w");
}

//ok
Game::~Game(){}
//ok
void Game::SetSize(const Vecti& v){
	size.x = v.x;
	size.y = v.y;
}                

//ok
void Game::AddSnake(){
	Vecti pos = std::move(RandPos());
	busy_cells.insert(pos);

	snakes.push_back(Snake(std::move(pos), Snake::Dir::UP));
}

/*
void Game::AddSnake(const Snake& s){
	snakes.push_back(s);
	for(const auto& item: s.segments){
		busy_cells.insert(item);
	}
}
*/

//ok
void Game::MoveSnake(Snake& sk){

	Vecti new_head(sk.segments.front());
		
	switch(sk.dir){
		case sk.UP: 	new_head.x -= 1; break;
		case sk.RIGHT: 	new_head.y += 1; break;
		case sk.DOWN: 	new_head.x += 1; break;	
		case sk.LEFT: 	new_head.y -= 1; break;
	}
	
	fprintf(file, "snake[%p] is moving to (%2d, %2d), dead(%d)\n", &sk, new_head.x, new_head.y, sk.is_dead);

	if(	busy_cells.count(new_head) == 0 && 
		new_head.x != 1 && new_head.x != size.x && 
		new_head.y != 1 && new_head.y != size.y){

		fprintf(file, "snake[%p] moved to (%2d, %2d)\n", &sk, new_head.x, new_head.y);

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
		fprintf(file, 	"busy_cells.count(new_head) = %lu\n"
						"new_head.x = %d\n"
						"new_head.y = %d\n"
						"size = (%d, %d)\n",
						busy_cells.count(new_head), new_head.x, new_head.y, size.x, size.y);
						
		fprintf(file, "snake[%p] is dead\n", &sk);
		sk.is_dead = true;
	}
}

void Game::GrowSnake(Snake& sk){
	
	Vecti d(0,0);
	int size = sk.segments.size();

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
		Vecti end(sk.segments.back());
		sk.segments.pop_back();

		Vecti pre_end(sk.segments.back());
		sk.segments.push_back(end);

		if(end.y == pre_end.y){
			d.x = (end.x > pre_end.x)? +1 : -1;
		}
		else{
			d.y = (end.y > pre_end.y)? +1 : -1;
		}
	}
		
	Vecti new_seg(sk.segments.back().x + d.x, sk.segments.back().y + d.y);

	sk.segments.push_back(Vecti(new_seg));
	busy_cells.insert(new_seg);		
}

//ok
void Game::Move(){

	for(auto& sk: this->snakes){

		if(!sk.is_dead){
			this->MoveSnake(sk);
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
	
	return Vecti(pos);
}


void Game::AddRabbit(){
	if(rabbits.size() >= MAX_N_RABBIT)
		return;
	
	Vecti pos = std::move(RandPos());
	busy_cells.insert(pos);
	rabbits.push_back(Rabbit(pos));
}

//FIXME
void Game::RemoveRabbit(const Vecti& v){
	for(auto& r: rabbits){
		if(r.cs == v){
			r.is_dead = true;
		}
	}
}
//FIXME
bool Game::IsRabbit(const Vecti& v){
	for(auto& r: rabbits){
		if(r.cs == v){
			return true;
		}
	}
	return false;
}

void Game::RandomInit(const int n_snakes, const int n_rabbits){
/*
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> xgen(3, size.x - 2);
	std::uniform_int_distribution<> ygen(3, size.y - 2);
*/
	fprintf(file, "Created %d snakes:\n", n_snakes);
	for(auto i = 0; i < n_snakes; i++){
		AddSnake();
		GrowSnake(snakes.at(i));
	}
	
	for(const auto& s: snakes){
		fprintf(file, "snake[%p], dead?(%d)\n", &s, s.is_dead);
	}

	for(auto i = 0; i < n_rabbits; i++){	
		AddRabbit();
	}
}
