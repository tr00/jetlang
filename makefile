CC=clang
CFLAGS=-Wall -Wextra -pedantic
LDFLAGS=-Wall

uniq = $(if $1,$(firstword $1) $(call uniq,$(filter-out $(firstword $1),$1)))

SRC=$(call uniq, src/pcc.c $(wildcard src/*.c))
OBJ=$(patsubst src/%.c, bin/%.o, $(SRC))

$(info $(shell mkdir -p bin))

all: $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) -o prog

bin/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

src/pcc.c: res/grammar.peg
	packcc -a -o pcc res/grammar.peg ; mv -t src/ pcc.*

clean:
	rm -rf bin/*
