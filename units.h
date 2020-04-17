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


	bool operator<(const Vect<T>& v) const{
		return ((x + 2048 * y) < (v.x + 2048 * v.y));//true;//((y < v.y) && (x < v.x)); 
	}

	bool operator==(const Vect<T>& v) const{
		return ((x == v.x) && (y == v.y)); 
	}

	T x;
	T y;
};

using Vecti = Vect<int>;
using Vectf = Vect<float>;


struct Rabbit{
	Rabbit(){
		cs = {0, 0};
		is_dead = true;
	}
	explicit Rabbit(const Vecti& new_v):
			cs(new_v){
	}
	
	~Rabbit(){}

	explicit Rabbit(const Rabbit& r):
			cs(r.cs){
	}

	Rabbit& operator=(const Rabbit& r){
		cs = r.cs;
		is_dead = r.is_dead;

		return *this;
	}

	Vecti cs;
	bool is_dead = false;
};

struct Snake{
	enum Dir{
		UP = 0,
		RIGHT = 1,
		DOWN = 2,
		LEFT = 3,
	};

	explicit Snake(const Vecti& v, Dir d, short int c = 0):
			segments(std::list<Vecti>(1, v)), 
			dir(d),
			is_dead(false){
	}

	~Snake(){
		fprintf(Game::file, "snake[%p] is destroyed\n", this);
		fflush(Game::file);
	}

	explicit Snake(const Snake& s):
			segments(s.segments),
			dir(s.dir),
			is_dead(s.is_dead){		
	}

	void SetDirection(Dir d){ dir = d;}
	
	void SetColor(short int c){ color = c;}

	Snake& operator=(const Snake& ) = delete;

	std::list<Vecti> segments;
	Dir dir;
	bool is_dead = false;
	short int color;
;
};
