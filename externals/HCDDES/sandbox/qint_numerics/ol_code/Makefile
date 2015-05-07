#
# Makefile for outer loop tester
#

CC=/home/jporter/gumstix/gumstix-oe/tmp/cross/arm-angstrom-linux-gnueabi/bin/gcc


CFLAGS = -c -O2 -march=armv5te -mtune=xscale -Wa,-mcpu=xscale -I.
LDFLAGS = 

C_SOURCES=OuterLoop_sl.c TestOuterLoop.c

C_OBJECTS=$(C_SOURCES:.c=.o)

EXECUTABLE=TestOuterLoop

all: $(parser) $(C_SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(C_OBJECTS)
	echo $(C_OBJECTS)
	$(CC) $(LDFLAGS) $(C_OBJECTS) -o $@

.c.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(C_OBJECTS) $(EXECUTABLE)


