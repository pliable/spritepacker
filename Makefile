CC = gcc
CFLAGS = -g -Wall -Wextra -std=c99
LFLAGS = -lfreeimage -lm

all: spacker

spacker: main.c helper.c
	$(CC) $(CFLAGS) -o spacker main.c helper.c $(LFLAGS)

clean:
	rm -f ./*.o spacker
