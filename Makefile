CC = gcc
CFLAGS = -Wall -Wextra -std=c99

all: spacker

spacker: main.c
	$(CC) $(CFLAGS) -o spacker main.c

clean:
	rm -f ./*.o spacker
