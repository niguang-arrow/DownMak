CC = clang
CFLAGS = -Wall -std=gnu99 -I../include -L../include

NAME = getcwd
SRC = $(NAME).c
TAR = $(NAME).out

$(TAR) : $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ -lapue


.PHONY : clean

clean :
	rm -rf *.out

.PHONY : run

run :
	./$(TAR)
