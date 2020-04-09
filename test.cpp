
#include <cassert>
#include <cstdio>
#include <iostream>

#include "game.h"
#include "text_ui.h"

#define ASSERT(smth) printf("[%3d] <%s>:\n %s \n", __LINE__, #smth, (smth) == true? "passed" : "ASSERTION FAILED!");
#define $BEGIN(smth) printf("Testing %s\n", #smth);

void TestVect(){
	$BEGIN(struct Vect);
	{
		Vecti c1(1, 1);
		ASSERT(c1.x == 1 && c1.y == 1);
	}

	{
		Vecti c1(1, 1);
		Vecti c2(c1);
		ASSERT(c2.x == c1.x && c2.y == c1.y);
	}
	{
		Vecti c1(1, 1);
		Vecti c2(1, 1);
		ASSERT(c1 == c2);
	}
	{
		Vecti c1(1, 1);
		Vecti c2(2, 2);
		ASSERT(c1.ComputeDistance(c2) == 2);
	}
	{
		Vecti c1(1, 2);
		Vecti c2(1, 1);
		c2 = c1;
		ASSERT(c2.x == c1.x && c2.y == c1.y);
	}
}

void TestRabbit(){
	$BEGIN(struct Rabbit);
	{
		Rabbit r1(Vecti(1, 2));
		ASSERT(r1.cs.x == 1 && r1.cs.y == 2);
	}
	{
		Rabbit r1(Vecti(1,1));
		Rabbit r2(r1);
		ASSERT(r2.cs.x == r1.cs.x && r2.cs.y == r1.cs.y);
	}
}

void TestSnake(){
	$BEGIN(struct Snake);
	{
		Snake s1(Vecti(1,1), Snake::Dir::UP);
		ASSERT(s1.segments.front() == Vecti(1,1));
		ASSERT(s1.dir == Snake::Dir::UP);
	}
	{
		Snake s1(Vecti(1,1), Snake::Dir::UP);
		Snake s2(s1);
		ASSERT(s2.segments.front() == Vecti(1,1));
		ASSERT(s2.dir == Snake::Dir::UP);
	}
	{
		Snake s1(Vecti(5,5), Snake::Dir::UP);
		s1.Grow();
		s1.Grow();

		ASSERT(s1.segments.back() == Vecti(7,5));
		s1.segments.pop_back();
		ASSERT(s1.segments.back() == Vecti(6,5));
	}
}


void TestGame(){
	$BEGIN(class Game);
	{	$BEGIN(AddSnake);

		Game g;
		g.AddSnake(Snake(Vecti(1,1), Snake::Dir::UP));
		g.AddSnake(Snake(Vecti(2,2), Snake::Dir::DOWN));


		ASSERT(g.snakes[0].segments.front() == Vecti(1,1) && g.snakes[0].dir == Snake::Dir::UP);
		ASSERT(g.snakes[1].segments.front() == Vecti(2,2) && g.snakes[1].dir == Snake::Dir::DOWN);
		fflush(stdout);
		
	}
	{	$BEGIN(AddRabbit);

		Game g;
		g.SetSize({10, 10});
		for(int i = 0; i < 20; i++){
			g.AddRabbit();
		}

		ASSERT(g.rabbits.size() <= Game::Settings::MAX_N_RABBIT);
		std::cout << g.busy_cells.size() << " " << g.rabbits.size() << std::endl;
		ASSERT(g.busy_cells.size() == Game::Settings::MAX_N_RABBIT);

		for(const auto& r: g.rabbits){
			if(g.busy_cells.count(r.cs) == false){
				ASSERT(!"wrong set of busy cells");
			}
		}

	}
/*	{ $BEGIN(RandomInit);

		Game g;
		g.SetSize(Vecti(10, 10));

		g.RandomInit(4, 10);
		ASSERT(g.GetNSnakes() == 4 && g.GetNRabbits() == 10);
	 
		bool coords_range_is_ok = true;

		std::vector<Rabbit> rs(g.GetRabbit());

		for(const auto& r: rs){
			printf("(%d, %d)\n", r.cs.x, r.cs.y);

			if(!(1 < r.cs.x && r.cs.x < 10 && 1 < r.cs.y && r.cs.y < 10))
				coords_range_is_ok = false;
		}
		ASSERT(coords_range_is_ok);
	}*/
}

void TestUi(){
	{
		
	}
}
int main(){
	TestVect();
	TestRabbit();
	TestSnake();

	TestGame();

	return 0;
}