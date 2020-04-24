#include <memory>
#include <iostream>
#include <chrono>
#include <thread>

#include "text_ui.h"
#include "game.h"
#include "control.h"

int main(){

	std::unique_ptr<ui> my_ui(ui::get("text_ui"));
	//calls delete automatically

	Game my_game;

	Player player1(my_game, TextUi::RED);

	Computer comp1(my_game, 1, TextUi::GREEN);
	Computer comp2(my_game, 2, TextUi::YELLOW);
	Computer comp3(my_game, 3, TextUi::BLUE);

	
	my_ui->Run(my_game);
	
	return 0;
}

/*
==23578== 552 bytes in 1 blocks are still reachable in loss record 1 of 1
==23578==    at 0x4C2FB0F: malloc (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so)
==23578==    by 0x545BE49: __fopen_internal (iofopen.c:65)
==23578==    by 0x545BE49: fopen@@GLIBC_2.2.5 (iofopen.c:89)
==23578==    by 0x4EF8973: std::random_device::_M_init(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&) (in /usr/lib/x86_64-linux-gnu/libstdc++.so.6.0.25)
==23578==    by 0x10F698: std::random_device::random_device(std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > const&) (in /home/alex/4sem/game/snake/main)
==23578==    by 0x10F498: __static_initialization_and_destruction_0(int, int) (in /home/alex/4sem/game/snake/main)
==23578==    by 0x10F542: _GLOBAL__sub_I__ZN4Game4fileE (in /home/alex/4sem/game/snake/main)
==23578==    by 0x1168CC: __libc_csu_init (in /home/alex/4sem/game/snake/main)
==23578==    by 0x53FEB27: (below main) (libc-start.c:266)
*/