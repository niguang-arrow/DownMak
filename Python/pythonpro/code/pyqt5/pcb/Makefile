CC = clang++
CFLAGS = -std=c++0x -Wall
CLINKS = `pkg-config --libs opencv`


NAME = bgfg_segm
SRC = $(NAME).cpp BackgroundSubtractorPAWCS.cpp \
	 BackgroundSubtractorLBSP.cpp LBSP.cpp 
TAR = $(NAME).out

$(TAR) : $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(CLINKS)


.PHONY : clean run


run :
	./$(TAR)


clean :
	rm -rf *.out
