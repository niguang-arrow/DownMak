CC = clang
CFLAGS = -Wall -ansi -I../include -L../include

NAME = test_lseek
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
