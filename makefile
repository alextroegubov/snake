CC = g++
PROG = main
FLAGS = -Wall

TEST_OBJ = test.o text_ui.o game.o ui.o control.o 

PROG_OBJ = main.o text_ui.o game.o ui.o control.o
PROG_LIB = game.h text_ui.h ui.h units.h control.h

all: $(PROG) clear

$(PROG): $(PROG_OBJ) $(PROG_LIB)
	$(CC) -o $@ $(PROG_OBJ) $(FLAGS)

test: $(TEST_OBJ) $(PROG_LIB)
	$(CC) -o $@ $(TEST_OBJ) $(FLAGS)

clear:
	rm -rf *.o
