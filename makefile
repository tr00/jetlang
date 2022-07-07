CC=clang
CFLAGS=-Wall -Wextra -pedantic

SRC=$(wildcard src/*.c)
OBJ=$(patsubst src/%.c, bin/%.o, $(SRC))

$(info $(shell mkdir -p bin))

pcc:
	packcc -a -o pcc res/grammar.peg
	mv -t src/ pcc.*

clean:
	rm -rf bin/*