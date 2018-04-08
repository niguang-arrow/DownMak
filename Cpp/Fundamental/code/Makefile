CC = clang++
CFLAGS = -std=c++0x -Wall
CLINKS =

NAME = main
SRC = $(NAME).cpp
TAR = $(NAME).out

$(TAR) : $(SRC)
	$(CC)  $(CFLAGS) -o $@ $^ $(CLINKS)


.PHONY : run clean


run :
	./$(TAR)


clean :
	rm -rf *.out
