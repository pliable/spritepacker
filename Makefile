CC = gcc
CFLAGS = -g -Wall -Wextra -std=c99 -D _BSD_SOURCE
LFLAGS = -lfreeimage -lm

all: spacker

spacker: main.c helper.c
	$(CC) $(CFLAGS) -o spacker main.c helper.c $(LFLAGS)

clean:
	rm -f ./*.o spacker
