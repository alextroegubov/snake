CXX = g++
PROG = main
CXXFLAGS += -Wall

PROG_OBJ = main.o text_ui.o game.o ui.o control.o
PROG_LIB = game.h text_ui.h ui.h units.h control.h

all: $(PROG) clean

$(PROG): $(PROG_OBJ) $(PROG_LIB)
	$(CXX) -o $@ $(PROG_OBJ) $(CXXFLAGS)

clean:
	rm -rf *.o
