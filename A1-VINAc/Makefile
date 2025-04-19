CFLAGS = -Wall -Wextra -g -std=c99
CC = gcc

OBJS = main.o directory.o dir_member.o archiver.o utils.o logger.o

# Default target when you just run `make`
all: VINAc

VINAc: $(OBJS)
	$(CC) -o VINAc $(OBJS)

directory.o: directory.c directory.h
	$(CC) -c $(CFLAGS) directory.c

dir_member.o: dir_member.c dir_member.h
	$(CC) -c $(CFLAGS) dir_member.c

archiver.o: archiver.c archiver.h dir_member.h directory.h
	$(CC) -c $(CFLAGS) archiver.c

utils.o: utils.c utils.h
	$(CC) -c $(CFLAGS) utils.c

logger.o: logger.c logger.h
	$(CC) -c $(CFLAGS) logger.c

main.o: main.c archiver.h
	$(CC) -c $(CFLAGS) main.c

clean:
	rm -f *.o *~ VINAc

