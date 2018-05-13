CC = clang++
CFLAGS = -Wall -std=c++0x
CLINKS =

NAME = toutiao2
SRC = $(NAME).cpp
TAR = $(NAME).out

$(TAR) : $(SRC) function.h
	$(CC) $(CFLAGS) -o $@ $< $(CLINK)

.PHONY : clean run

clean :
	rm -rf *.out

run :
	./$(TAR)
