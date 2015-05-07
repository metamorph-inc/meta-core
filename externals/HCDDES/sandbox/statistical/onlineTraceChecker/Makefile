BISON=bison
BISON_ARGS=-d#-vdt
FLEX=flex
CC=gcc
CPP=g++
CFLAGS=
CDEBUG=-g
COPT=-O3
VERBOSE=#-DVERBOSE

EXEC=checker

all: test.o checker.o parser.tab.o
	$(CPP) $(COPT) $(CDEBUG) $(VERBOSE) test.o checker.o parser.tab.o -o $(EXEC)

interface: interface.o checker.o parser.tab.o
	

test.o: test.cpp Makefile
	$(CPP) $(COPT) $(CDEBUG) $(VERBOSE) -c test.cpp

interface.o: interface.cpp Makefile
	$(CPP) $(COPT) $(CDEBUG) $(VERBOSE) -c interface.cpp

checker.o: checker.cpp checker.hpp Makefile
	$(CPP) $(COPT) $(CDEBUG) $(VERBOSE) -c checker.cpp

parser.tab.o: parser.tab.cpp Makefile
	$(CPP) $(COPT) $(CDEBUG) $(VERBOSE) -c parser.tab.cpp

parser.tab.cpp: parser.ypp parser.hpp lex.yy.c
	$(BISON) $(BISON_ARGS) parser.ypp

lex.yy.c: lexer.l
	$(FLEX) lexer.l

clean:
	rm -f *.o
	rm -f lex.yy.c
	rm -f parser.tab.*
	rm -f parser.output
	rm -f $(EXEC)
