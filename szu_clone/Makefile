CXX = g++
CXXFLAGS = -g -std=c++11
OBJS = board/boardPos.o board/boardPosList.o board/boardUtils.o board/rectSize.o game2048/action2048.o game2048/game2048Board.o util/arrayUtils.o util/collectionUtils.o util/randomUtils.o util/summaryStatistics.o game2048/game2048.o NTuplePlayer2048.o game2048/state2048.o NTuple/NTuple.o game2048/player2048.o NTuple/NTuple.o NTuple/NTuples.o NTuple/NTupleUtils.o NTuple/NTuplesBuilder.o NTuple/eval/defaultNTupleEvaluator.o NTuple/expanders/identitySymmetryExpander.o NTuple/expanders/standardSymmetryExpander.o NTuple/expanders/symmetryUtils.o

all: clean program

program: $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS)

clean:
	\rm -f *.o
