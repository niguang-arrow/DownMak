CC = clang
CFLAGS = -Wall -ansi

NAME = spwd
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
