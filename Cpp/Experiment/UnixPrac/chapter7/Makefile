CC = clang
CFLAGS = -Wall -ansi -lcurses

NAME = sigdemo3
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
