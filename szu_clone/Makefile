CXX = g++
CXXFLAGS = -g -std=c++11
OBJS = test.o board/boardPos.o board/boardPosList.o board/boardUtils.o board/rectSize.o

all: clean program

program: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS)

clean:
	\rm -f *.o
