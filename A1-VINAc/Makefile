CFLAGS = -Wall -Wextra -Werror -g -std=c99

CC = gcc

OBJS = main.o directory.o dir_member.o archiver.o utils.o logger.o content.o lz.o

# Default target when you just run `make`
all: VINAc

VINAc: $(OBJS)
	$(CC) -o VINAc $(OBJS)

lz.o: lz.c lz.h
	$(CC) -c $(CFLAGS) lz.c

content.o: content.c content.h
	$(CC) -c $(CFLAGS) content.c

directory.o: directory.c directory.h
	$(CC) -c $(CFLAGS) directory.c

dir_member.o: dir_member.c dir_member.h
	$(CC) -c $(CFLAGS) dir_member.c

archiver.o: archiver.c archiver.h dir_member.h directory.h content.h
	$(CC) -c $(CFLAGS) archiver.c

utils.o: utils.c utils.h
	$(CC) -c $(CFLAGS) utils.c

logger.o: logger.c logger.h
	$(CC) -c $(CFLAGS) logger.c

main.o: main.c archiver.h
	$(CC) -c $(CFLAGS) main.c

clean:
	rm -f *.o *~ VINAc

