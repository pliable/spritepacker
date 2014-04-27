CC = gcc
CFLAGS = -Wall -Wextra

all: spacker

spacker: main.c
	$(CC) $(CFLAGS) -o spacker main.c

clean:
	rm -f ./*.o spacker
