CC = clang++
CFLAGS = -Wall -std=c++0x
CLINKS =

NAME = vtable
SRC = $(NAME).cpp
TAR = $(NAME).out

$(TAR) : $(SRC)
	$(CC) $(CFLAGS) -g -o $@ $^ $(CLINKS)


.PHONY : run clean


run :
	./$(TAR)

clean :
	rm -rf *.out
