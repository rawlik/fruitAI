CXX = g++
CXXFLAGS = -MMD -ansi -pedantic -Wall -O2 -g

PROGRAMY = 

all: $(PROGRAMY)

test: graph.o matrix_graph.o lists_graph.o

%: %.o
	$(CXX) $(LDFLAGS) $^ $(LDLIBS) -o $@

clean:
	rm -f $(PROGRAMY) *.o *.d

.PHONY: all clean

include $(wildcard *.d)
