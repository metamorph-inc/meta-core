#
FRODO_ROOT = ../../../
ARCH_PREFIX = arch/linux32/
ARCH_ROOT = $(FRODO_ROOT)$(ARCH_PREFIX)
LINUX_ROOT = /usr/src/linux-headers-2.6.31-17/
HRT_PATH = $(LINUX_ROOT)include
LINUX_ARCH_PATH = $(LINUX_ROOT)arch/x86/include
INCLUDED_DIRS = -I. -I$(FRODO_ROOT)
TARGET = SimpleExample

#
FRODO_OBJS = error_handler.o logger.o scheduler.o ports.o serialio.o
PLATFORM_OBJS = highres_timing.o linux32_platform.o linux32_serial.o
OBJS = $(FRODO_OBJS) $(PLATFORM_OBJS) main.o

CC = gcc
CFLAGS = $(INCLUDED_DIRS)

#
all SimpleExample: $(OBJS)
	$(CC) $(OBJS) -lpthread -lrt -o $(TARGET)

error_handler.o: $(FRODO_ROOT)error_handler.c $(FRODO_ROOT)error_handler.h
	$(CC) $(CFLAGS) -o error_handler.o -c $(FRODO_ROOT)error_handler.c
logger.o: $(FRODO_ROOT)logger.c $(FRODO_ROOT)logger.h
	$(CC) $(CFLAGS) -o logger.o -c $(FRODO_ROOT)logger.c
scheduler.o: $(FRODO_ROOT)scheduler.c $(FRODO_ROOT)scheduler.h
	$(CC) $(CFLAGS) -o scheduler.o -c $(FRODO_ROOT)scheduler.c
ports.o: $(FRODO_ROOT)ports.c $(FRODO_ROOT)ports.h
	$(CC) $(CFLAGS) -o ports.o -c $(FRODO_ROOT)ports.c
serialio.o: $(FRODO_ROOT)serialio.c $(FRODO_ROOT)serialio.h
	$(CC) $(CFLAGS) -o serialio.o -c $(FRODO_ROOT)serialio.c

highres_timing.o: $(ARCH_ROOT)highres_timing.c $(FRODO_ROOT)arch/highres_timing.h
	$(CC) $(CFLAGS) -o highres_timing.o -c $(ARCH_ROOT)highres_timing.c
linux32_platform.o: $(ARCH_ROOT)linux32_platform.c $(ARCH_ROOT)linux32_platform.h
	$(CC) $(CFLAGS) -o linux32_platform.o -c $(ARCH_ROOT)linux32_platform.c
linux32_serial.o: $(ARCH_ROOT)linux32_serial.c
	$(CC) $(CFLAGS) -o linux32_serial.o -c $(ARCH_ROOT)linux32_serial.c

main.o: ../main.c main.h
	$(CC) $(CFLAGS) -o main.o -c ../main.c

Platform: $(PLATFORM_OBJECTS)

Frodo: $(FRODO_OBJECTS)

clean:
	rm *.o

