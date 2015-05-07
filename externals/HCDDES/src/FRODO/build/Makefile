#
FRODO_ROOT = ..
FRODO_SOURCE = $(FRODO_ROOT)/src/
FRODO_INCLUDE = $(FRODO_ROOT)/include/
INCLUDED_DIRS = -I$(FRODO_INCLUDE)
TARGET = libfrodo.a

PLATFORM = $(shell uname)
ifeq "$(PLATFORM)" "Darwin"
SERVICES = -framework CoreServices
ARCH_PREFIX = arch/osx/
endif
ifeq "$(PLATFORM)" "Linux"
SERVICES = -lrt
ARCH_PREFIX = arch/linux64/
endif


#
FRODO_OBJS = events.o error_handler.o logger.o peripheral.o ports.o scheduler.o udp.o
PLATFORM_OBJS = highres_timing.o platform.o platform_udp.o
OBJS = $(FRODO_OBJS) $(PLATFORM_OBJS)

CC = gcc
AR = ar
# Optimization must be O0 because higher values cause back evaluation
#   of complex if statements, like the one in scheduler.c:221.
CFLAGS = $(INCLUDED_DIRS) -g -O0

#
all frodo: $(OBJS)
	$(AR) rcs $(TARGET) $(OBJS)
	mv $(TARGET) $(FRODO_ROOT)/lib

events.o: $(FRODO_SOURCE)events.c $(FRODO_INCLUDE)events.h
	$(CC) $(CFLAGS) -o events.o -c $(FRODO_SOURCE)events.c
error_handler.o: $(FRODO_SOURCE)error_handler.c $(FRODO_INCLUDE)error_handler.h
	$(CC) $(CFLAGS) -o error_handler.o -c $(FRODO_SOURCE)error_handler.c
logger.o: $(FRODO_SOURCE)logger.c $(FRODO_INCLUDE)logger.h
	$(CC) $(CFLAGS) -o logger.o -c $(FRODO_SOURCE)logger.c
peripheral.o: $(FRODO_SOURCE)peripheral.c $(FRODO_INCLUDE)peripheral.h
	$(CC) $(CFLAGS) -o peripheral.o -c $(FRODO_SOURCE)peripheral.c
ports.o: $(FRODO_SOURCE)ports.c $(FRODO_INCLUDE)ports.h
	$(CC) $(CFLAGS) -o ports.o -c $(FRODO_SOURCE)ports.c
scheduler.o: $(FRODO_SOURCE)scheduler.c $(FRODO_INCLUDE)scheduler.h
	$(CC) $(CFLAGS) -o scheduler.o -c $(FRODO_SOURCE)scheduler.c
udp.o: $(FRODO_SOURCE)udp.c $(FRODO_INCLUDE)udp.h
	$(CC) $(CFLAGS) -o udp.o -c $(FRODO_SOURCE)udp.c

highres_timing.o: $(FRODO_SOURCE)$(ARCH_PREFIX)highres_timing.c $(FRODO_INCLUDE)highres_timing.h
	$(CC) $(CFLAGS) -o highres_timing.o -c $(FRODO_SOURCE)$(ARCH_PREFIX)highres_timing.c
platform.o: $(FRODO_SOURCE)$(ARCH_PREFIX)platform.c $(FRODO_INCLUDE)$(ARCH_PREFIX)platform.h
	$(CC) $(CFLAGS) -o platform.o -c $(FRODO_SOURCE)$(ARCH_PREFIX)platform.c
platform_udp.o: $(FRODO_SOURCE)$(ARCH_PREFIX)platform_udp.c $(FRODO_INCLUDE)udp.h
	$(CC) $(CFLAGS) -o platform_udp.o -c $(FRODO_SOURCE)$(ARCH_PREFIX)platform_udp.c

Platform: $(PLATFORM_OBJECTS)

Frodo: $(FRODO_OBJECTS)

clean:
	rm -f *.o
	rm -f $(FRODO_ROOT)/lib/$(TARGET)

