CC = gcc
CFLAGS = -Wall -Wextra -std=c99
LFLAGS = -lfreeimage -lm

all: spacker

spacker: main.c
	$(CC) $(CFLAGS) -o spacker main.c $(LFLAGS)

clean:
	rm -f ./*.o spacker
