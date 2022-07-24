CC=gcc
CFLAGS=-I. -g -W -pthread
DEPS = buff.h
OBJ = sthread.o lowlevel.o

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

sthread: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)


