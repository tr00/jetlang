CC=clang
CFLAGS=-Wall -Wextra -pedantic

SRC=$(wildcard src/*.c)
OBJ=$(patsubst src/%.c, bin/%.o, $(SRC))

clean:
	rm -rf bin/*