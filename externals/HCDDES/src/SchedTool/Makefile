# Makefile for SchedTool


GECODE_PATH=/home/jporter/svn/HCDDES/3rdparty/gecode/3.2.2
ANTLR3_PATH=/home/jporter/svn/HCDDES/3rdparty/antlr/3.1.3

CXXFLAGS=-c -g -I. -I$(GECODE_PATH)/include -I$(ANTLR3_PATH)/include
LDFLAGS=-L$(GECODE_PATH)/lib -L$(ANTLR3_PATH)/lib -lstdc++ -lantlr3c \
	-lboost_program_options \
	-lgecodekernel -lgecodesupport -lgecodeminimodel -lgecodeint \
	-lgecodesearch -lgecodescheduling -lgecodegist -lgecodeset

C_SOURCES=schedParser.c schedLexer.c
CXX_SOURCES=ConfigKeeper.cpp CPEngine.cpp cpSolver.cpp \
	defaultSchedModel.cpp InstanceKeeper.cpp schedAdapter.cpp \
	schedModel.cpp SchedTool.cpp solverAdapter.cpp

C_OBJECTS=$(C_SOURCES:.c=.o)
CXX_OBJECTS=$(CXX_SOURCES:.cpp=.o)

EXECUTABLE=SchedTool

all: $(parser) $(C_SOURCES) $(CXX_SOURCES) $(EXECUTABLE)

# PRODUCES schedParser.[ch] schedLexer.[ch]
$(C_SOURCES): sched.g
	antlr3 sched.g

$(EXECUTABLE): $(C_OBJECTS) $(CXX_OBJECTS)
	echo $(C_OBJECTS)
	echo $(CXX_OBJECTS)
	$(CXX) $(LDFLAGS) $(C_OBJECTS) $(CXX_OBJECTS) -o $@

.c.o:
	$(CC) $(CXXFLAGS) $< -o $@

.cpp.o:
	$(CXX) $(CXXFLAGS) $< -o $@

cleanall: clean

clean:
	rm -f $(C_OBJECTS) $(CXX_OBJECTS) $(EXECUTABLE)

