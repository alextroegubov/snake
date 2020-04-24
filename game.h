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
		MAX_N_RABBIT = 20,
		RABBIT_CHANCE = 15//%
	};
	//constructor
	Game();
	//destructor
	~Game() = default;

	Game(const Game&) = delete;
	Game& operator=(const Game&) = delete;

	//get vector of snakes
	const std::vector<Snake*>& GetSnakes() const{ return snakes;}
	//get number of snakes
	int GetNSnakes() const{ return snakes.size();}

	int GetNRabbits() const{ return n_rabbits;}
	//get array of rabbits
	const std::array<Rabbit, MAX_N_RABBIT>& GetRabbit() const{ return rabbits;}
	//set size of the game field
	void SetSize(const Vecti& v);
	//move everything
	void Move();
	//add rabbit at random position
	void AddRabbit();
	//add existing snake in vector
	void AddSnake(Snake*);
	//checks if there is a rabbit on the cell
	bool IsRabbit(const Vecti& cell) const;
	//check if the cell is busy by any object
	bool IsBusy(const Vecti& v) const;
	//removes rabbit from cell
	void RemoveRabbit(const Vecti& cell);
	//return random position to put smth
	Vecti RandPos();
	//adds a segment to snake
	void GrowSnake(Snake& sk);

private:
	//moves snake
	void MoveSnake(Snake& sk);
private:
	//size of game field
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
