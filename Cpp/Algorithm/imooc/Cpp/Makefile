CC = clang++
CFLAGS = -Wall -std=c++0x
CLINKS =

NAME = main
SRC = $(NAME).cpp
TAR = $(NAME).out

$(TAR) : $(SRC) a.cpp
	$(CC) $(CFLAGS) -o $@ $^ $(CLINK)

.PHONY : clean run

clean :
	rm -rf *.out

run :
	./$(TAR)
