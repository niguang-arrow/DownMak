CC = clang
CFLAGS = -Wall -std=gnu99 -I../include -L../include

NAME = pthread_mutex_timedlock
SRC = $(NAME).c
TAR = $(NAME).out

$(TAR) : $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ -lapue -lpthread


.PHONY : clean

clean :
	rm -rf *.out

.PHONY : run

run :
	./$(TAR)
