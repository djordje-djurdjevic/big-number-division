CC     = gcc
CFLAGS = -std=c2x -Wall -Wextra -Wpedantic
SRC    = bigint.c main.c
HDR    = bigint.h

main: $(SRC) $(HDR)
	$(CC) $(CFLAGS) $(SRC) -o main

.PHONY: clean
clean:
	rm -f main