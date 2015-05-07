# Make sure to define some variables
FRODO_ROOT = ../..
FRODO_SRC = $(FRODO_ROOT)/src/
INCLUDED_DIRS = -I $(FRODO_ROOT)/include
TARGET = QuadRotorPlant

PLATFORM = $(shell uname)
ifeq "$(PLATFORM)" "Darwin"
SERVICES = -framework CoreServices
endif
ifeq "$(PLATFORM)" "Linux"
SERVICES = -lrt
endif

CC = gcc
OBJS = main.o QuadRotorPlant.o QuadRotorPlant_data.o rt_rand.o
CFLAGS = $(INCLUDED_DIRS) -g -O3

# Targets
all QuadRotorPlant: main.o QuadRotorPlant.o QuadRotorPlant_data.o rt_rand.o
	$(CC) $(OBJS) -L$(FRODO_ROOT)/lib -lm -lpthread $(SERVICES) -lfrodo -o $(FRODO_ROOT)/bin/$(TARGET)

QuadRotorPlant.o: QuadRotorPlant.c
	$(CC) $(CFLAGS) -o QuadRotorPlant.o -c QuadRotorPlant.c

QuadRotorPlant_data.o: QuadRotorPlant_data.c
	$(CC) $(CFLAGS) -o QuadRotorPlant_data.o -c QuadRotorPlant_data.c

rt_rand.o: rt_rand.c
	$(CC) $(CFLAGS) -o rt_rand.o -c rt_rand.c

main.o: main.c
	$(CC) $(CFLAGS) -o main.o -c main.c

# And, allow stuff to be cleaned up
clean:
	rm -f *.o
	rm -f $(TARGET)

