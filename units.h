#pragma once

#include <algorithm>
#include <vector>
#include <list>

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
		return (x + y < v.x + v.y); 
	}

	bool operator==(const Vect<T>& v) const{
		return (x == v.x && y == v.y); 
	}

	T x;
	T y;
};

using Vecti = Vect<int>;
using Vectf = Vect<float>;


struct Rabbit{
	explicit Rabbit(const Vecti& new_v):
			cs(new_v){
	}
	
	~Rabbit(){}

	explicit Rabbit(const Rabbit& r):
			cs(r.cs){
	}

	Rabbit& operator=(const Rabbit& r) = delete;

	Vecti cs;
};

struct Snake{
	enum Dir{
		UP = 0,
		RIGHT,
		DOWN,
		LEFT,
	};

	explicit Snake(const Vecti& v, Dir d):
			segments(std::list<Vecti>(1, v)), 
			dir(d),
			is_dead(false){
	}

	~Snake(){
	}

	explicit Snake(const Snake& s):
			dir(s.dir),
			segments(s.segments),
			is_dead(s.is_dead){		
	}


	Snake& operator=(const Snake& ) = delete;

/*	//connect with move
	void Grow(){
		int dx = 0;
		int dy = 0;
		int size = segments.size();
		if(size == 1){

			switch(dir){
				case Dir::UP:
					dx = +1;
					break;
				case Dir::DOWN:
					dx = -1;
					break;
				case Dir::LEFT:
					dy = +1;
					break;
				case Dir::RIGHT:
					dy = -1;
			}
		}
		else{
			//the last two segments
			Vecti end(segments.back());
			segments.pop_back();
			Vecti pre_end(segments.back());
			segments.push_back(end);

			int xt = end.x; //tail
			int yt = end.y; //tail
			int x = pre_end.x;
			int y = pre_end.y;

			if(yt == y){
				dx = (xt > x)? +1 : -1;
			}
			else{
				dy = (yt > y)? +1 : -1;
			}
		}
		
		int new_x = segments.back().x + dx;
		int new_y = segments.back().y + dy;
		segments.push_back(Vecti(new_x, new_y));
		
	}
*/
	bool is_dead = false;
	Dir dir;
	std::list<Vecti> segments;
};
