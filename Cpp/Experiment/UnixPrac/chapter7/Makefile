CC = clang
CFLAGS = -Wall -std=gnu99 -lcurses

NAME = bounce1d
SRC = $(NAME).c
TAR = $(NAME).out

$(TAR) : $(SRC)
	$(CC) $(CFLAGS) -o $@ $^


.PHONY : clean

clean :
	rm -rf *.out

.PHONY : run

run :
	./$(TAR)
