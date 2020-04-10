//list of rabbits or vector
// pair<int, int>
//random coord for rabbits
//randxy return random pair in visivle part of screen
//one can draw by view or by game
//usually game calls view
//game speed - game, frames per second - view
//темп игры,  темп отображения
//
//snake - vector/list of segments, head <>^V, direction,
//each segment - a class sush as rabbit
//motion: head, segment, taily
//con-r: several rabbits and snakes at random coords.
#pragma once

#include <utility>
#include <algorithm>
#include <vector>
#include <set>
#include "units.h"

class Game{
public:
	Game();
	~Game();
	friend void TestGame();
	
	enum Settings{
		TICK = 1000,
		MAX_N_RABBIT = 8,
		RABBIT_CHANCE = 20//%
	};

	void RandomInit(const int n_snakes, const int n_rabbits = 3);
	void SetSize(const Vecti& v);
	void Move();

	size_t GetNSnakes() const{return snakes.size();}
	size_t GetNRabbits() const{return rabbits.size();}

	const std::vector<Snake>& GetSnakes() const{return snakes;}
	const std::vector<Rabbit>& GetRabbit() const{return rabbits;}

public:
	void AddRabbit();
	void AddSnake(const Snake& s);
	void AddSnake();
	void AddRabbit(const Rabbit& r);
	void MoveSnake(Snake& sk);
	void GrowSnake(Snake& sk);
	Vecti RandPos();
//	Vecti RandVecti(const int a, const int b);


	Vecti size;
	std::set<Vecti> busy_cells;
	std::vector<Snake> snakes;
	std::vector<Rabbit> rabbits;
};