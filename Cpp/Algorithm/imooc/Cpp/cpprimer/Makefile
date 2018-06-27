CC = clang++
CFLAGS = -Wall -std=c++0x
CLINKS =

NAME = inherit
SRC = $(NAME).cpp
TAR = $(NAME).out

$(TAR) : $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(CLINKS)


.PHONY : run clean


run :
	./$(TAR)

clean :
	rm -rf *.out
