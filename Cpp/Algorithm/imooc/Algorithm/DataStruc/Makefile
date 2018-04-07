CC = clang++
CFLAGS = -Wall -std=c++0x
CLINKS =

NAME = graph
SRC = $(NAME).cpp
TAR = $(NAME).out

$(TAR) : $(SRC) Graph.h ReadGraph.h Component.h Path.h ShortestPath.h
	$(CC) $(CFLAGS) -o $@ $< $(CLINK)

.PHONY : clean run

clean :
	rm -rf *.out

run :
	./$(TAR)
