# Make sure to define some variables
FRODO_ROOT = ../../..
FRODO_SRC = $(FRODO_ROOT)/src/
INCLUDED_DIRS = -I$(FRODO_ROOT)/include
TARGET = UDPSimpleListen

PLATFORM = $(shell uname)
ifeq "$(PLATFORM)" "Darwin"
SERVICES = -framework CoreServices
endif
ifeq "$(PLATFORM)" "Linux"
SERVICES = -lrt
endif

CC = gcc
CFLAGS = $(INCLUDED_DIRS) -g -O3

# Targets
all SingleTaskTiming: main.o
	$(CC) main.o -L$(FRODO_ROOT)/lib -lpthread $(SERVICES) -lfrodo -o $(TARGET)

main.o: main.c
	$(CC) $(CFLAGS) -o main.o -c main.c

# And, allow stuff to be cleaned up
clean:
	rm -f *.o
	rm -f $(TARGET)

