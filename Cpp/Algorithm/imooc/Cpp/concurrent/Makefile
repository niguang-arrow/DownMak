CC = clang++
CFLAGS = -Wall -std=c++0x
CLINKS = -lpthread

NAME = threading
SRC = $(NAME).cpp
TAR = $(NAME).out

$(TAR) : $(SRC)
	$(CC) $(CFLAGS) -o $@ $< $(CLINKS)

.PHONY : clean run

clean :
	rm -rf *.out

run :
	./$(TAR)
