CC = clang++
CFLAGS = -Wall -std=c++0x
CLINKS =

NAME = sort
SRC = $(NAME).cpp
TAR = $(NAME).out 

$(TAR) : $(SRC) sort_function.h
	$(CC) $(CFLAGS) -o $@ $< $(CLINK)

.PHONY : clean run

clean :
	rm -rf *.out

run :
	./$(TAR)
