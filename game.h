#pragma once

#include <utility>
#include <algorithm>
#include <vector>
#include <set>
#include <cstdio>
#include <array>

#include "units.h"

class Game{
public:	
	enum Settings{
		TICK = 100,//ms
		MAX_N_RABBIT = 10,
		RABBIT_CHANCE = 20//%
	};

	Game();

	~Game();

	Game(const Game&) = delete;
	
	Game& operator=(const Game&) = delete;

	const std::vector<Snake*>& GetSnakes() const{ return snakes;}

	int GetNSnakes() const{ return snakes.size();}

	const std::array<Rabbit, MAX_N_RABBIT>& GetRabbit() const{ return rabbits;}

	void RandomInit(const int n_snakes, const int n_rabbits);

	void SetSize(const Vecti& v);

	void Move();

	void AddRabbit();

	void AddSnake(Snake*);

	bool IsRabbit(const Vecti& cell) const;

	bool IsBusy(const Vecti& v) const;

	void RemoveRabbit(const Vecti& cell);

	Vecti RandPos();

	void MoveSnake(Snake& sk);

	void GrowSnake(Snake& sk);

private:
	//size of window
	Vecti size;
	//set of busy cells: snakes segments, rabbits
	std::set<Vecti> busy_cells;
	//vector of snakes
	std::vector<Snake*> snakes;
	//current number of rabbits on the field
	int n_rabbits;
	//array of rabbits, consits of both dead and alive rabbits
	std::array<Rabbit, MAX_N_RABBIT> rabbits;
public:	
	static FILE *file;
};
