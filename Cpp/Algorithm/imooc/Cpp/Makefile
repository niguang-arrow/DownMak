CC = clang++
CFLAGS = -Wall -std=c++0x
CLINKS =

NAME = inherit
SRC = $(NAME).cpp
TAR = $(NAME).out

$(TAR) : $(SRC)
	$(CC) $(CFLAGS) -o $@ $< $(CLINK)

.PHONY : clean run

clean :
	rm -rf *.out

run :
	./$(TAR)
