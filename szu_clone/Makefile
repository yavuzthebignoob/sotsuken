CC=g++
CFLAG=-g
OBJGROUP=NTuplePlayer2048.o action2048.o game2048.o randomUtils.o state2048.o

all: clean program

program: $(OBJGROUP)
	(CC) -o program $(OBJGROUP)

clean:
	\rm *.o
