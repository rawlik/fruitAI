CXX = g++
CXXFLAGS = -ansi -pedantic -Wall -O2 -g -lncurses

PROGRAMY = game
OBIEKTY = bin/game_classes.o bin/graph/graph.o bin/graph/matrix_graph.o bin/graph/lists_graph.o

all: $(PROGRAMY)

game: $(OBIEKTY)
	$(CXX) $(CXXFLAGS) -MMD -MF src/deps/game.d src/game.cpp $(OBIEKTY) $(LDLIBS) -o $@

bin/%.o: src/%.cpp
	$(CXX) $(CXXFLAGS) -c -MMD -MF src/deps/$*.d $< -o $@

clean:
	rm -f $(PROGRAMY) *.o *.d graph/*.o graph/*.d
	rm -f bin/*.o bin/graph/*.o src/deps/graph/*.d src/deps/*.d

.PHONY: all clean

include $(wildcard src/deps/*.d)