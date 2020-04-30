#pragma once

#include <algorithm>
#include <vector>
#include <list>
#include "game.h"

template<typename T>
struct Vect{
	Vect(const int new_x = 0, const int new_y = 0):
			x(new_x), y(new_y){
	}
	
	Vect(const Vect<T>& v):
			x(v.x), y(v.y){
	}

	~Vect() = default;

	Vect<T>& operator=(const Vect<T>& v){
		x = v.x;
		y = v.y;
		return *this;
	}

	//in cells(!)
	int ComputeDistance(const Vect<T>& v) const{ 
		return std::abs(x - v.x) + std::abs(y - v.y);
	}
	//to have an opportunity to store vect in maps and sets
	bool operator<(const Vect<T>& v) const{
		return ((x + 2048 * y) < (v.x + 2048 * v.y));
	}

	bool operator==(const Vect<T>& v) const{
		return ((x == v.x) && (y == v.y)); 
	}

	T x;
	T y;
};

using Vecti = Vect<int>;

struct Rabbit{

	//default constructor
	Rabbit(){
		cs = {0, 0};
		is_dead = true;
	}
	//consturctor from vector
	explicit Rabbit(const Vecti& new_v):
			cs(new_v), is_dead(false){
	}
	//destructor
	~Rabbit() = default;
	//copy constructor
	explicit Rabbit(const Rabbit& r):
			cs(r.cs), is_dead(r.is_dead){
	}
	//copy operator
	Rabbit& operator=(const Rabbit& r){
		cs = r.cs;
		is_dead = r.is_dead;

		return *this;
	}

	Vecti cs;
	bool is_dead;
};

struct Snake{

	enum Dir{
		UP = 0,
		RIGHT = 1,
		DOWN = 2,
		LEFT = 3,
	};

	//default constructor
	explicit Snake():
			segments(std::list<Vecti>()),
			dir(UP),
			is_dead(false),
			color(0),
			score(0){
	}
	//constructor by vector
	explicit Snake(const Vecti& v, Dir d, short int c = 0):
			segments(std::list<Vecti>(1, v)), 
			dir(d),
			is_dead(false),
			color(c),
			score(0){
	}

	~Snake() = default;
	//copy constructor
	explicit Snake(const Snake& s):
			segments(s.segments),
			dir(s.dir),
			is_dead(s.is_dead),
			score(s.score){		
	}

	//copy operator
	Snake& operator=(const Snake& ) = delete;

	//all parts of snake
	std::list<Vecti> segments;
	//direction
	Dir dir;
	//
	bool is_dead = false;
	//
	short int color;
	//eaten rabbits
	int score;
};
