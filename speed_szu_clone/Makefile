BCXX = g++
CXXFLAGS = -g -std=c++11
INCLUDE = ./rl
SRCS :=
REL := board/
include $(REL)Makefile
REL := game2048/
include $(REL)Makefile
REL := util/
include $(REL)Makefile
REL := NTuple/
include $(REL)Makefile
SRCS+=NTuplePlayer2048.cpp
SRCS+=CIG2048ExperimentTDL.cpp
TARGET = a.out
OBJS = $(SRCS:.cpp=.o)
TILDE = $(SRCS:.cpp=.cpp~)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@ -I$(INCLUDE)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $+

default:clean program

program:$(OBJS) $(TARGET)

clean:
	\rm -f $(OBJS) $(TILDE)
