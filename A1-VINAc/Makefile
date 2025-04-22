#CFLAGS = -Wall -Wextra -Werror -g -std=c99
CFLAGS = -g -std=c99
#// TOOD TODO TODO

CC = gcc

OBJS_COMMON = directory.o archiver.o utils.o logger.o content.o lz.o files.o
OBJS = main.o dir_member.o $(OBJS_COMMON)

all: VINAc

homolog: VINAc-homologation

VINAc-homologation: main_homolog.o dir_member_homolog.o $(OBJS_COMMON)
	mkdir -p tests
	$(CC) -o tests/VINAc-homologation main_homolog.o dir_member_homolog.o $(OBJS_COMMON)

VINAc: $(OBJS)
	$(CC) -o VINAc $(OBJS)

main.o: main.c
	$(CC) -c $(CFLAGS) main.c -o main.o

main_homolog.o: main.c archiver.h
	$(CC) -c $(CFLAGS) -DHOMOLOG main.c -o main_homolog.o

dir_member_homolog.o: dir_member.c dir_member.h
	$(CC) -c $(CFLAGS) -DHOMOLOG dir_member.c -o dir_member_homolog.o

lz.o: lz.c lz.h
	$(CC) -c $(CFLAGS) lz.c

content.o: content.c content.h
	$(CC) -c $(CFLAGS) content.c

directory.o: directory.c directory.h
	$(CC) -c $(CFLAGS) directory.c

dir_member.o: dir_member.c dir_member.h
	$(CC) -c $(CFLAGS) dir_member.c

archiver.o: archiver.c archiver.h
	$(CC) -c $(CFLAGS) archiver.c

utils.o: utils.c utils.h
	$(CC) -c $(CFLAGS) utils.c

logger.o: logger.c logger.h
	$(CC) -c $(CFLAGS) logger.c

files.o: files.c files.h
	$(CC) -c $(CFLAGS) files.c

clean:
	rm -f *.o *~ VINAc tests/*.o tests/*~ tests/VINAc-homologation
