CC = clang++
CFLAGS = -Wall -std=c++0x
CLINK =

INCLUDE_FILE = FileOps.h SequenceST.h

NAME = bstree
SRC = $(NAME).cpp
TAR = $(NAME).out


$(TAR) : $(SRC) $(INCLUDE_FILE)
	$(CC) $(CFLAGS) -o $@ $< $(CLINK)


.PHONY : clean run


clean :
	rm -rf *.out

run :
	./$(TAR)

