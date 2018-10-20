CC = clang++
CFLAGS = -Wall -std=c++0x
CLINKS = 

NAME = meituan
SRC = $(NAME).cpp
TAR = $(NAME).out


$(TAR) : $(SRC) function.h
	$(CC) $(CFLAGS) -o $@ $< $(CLINKS)


.PHONY : run clean

run :
	./$(TAR)

clean :
	rm -rf *.out
