CC=clang
CFLAGS=-Wall -Wextra -pedantic
LDFLAGS=-Wall

uniq = $(if $1,$(firstword $1) $(call uniq,$(filter-out $(firstword $1),$1)))

SRC=$(call uniq, src/pcc.c $(wildcard src/*.c))
OBJ=$(patsubst src/%.c, bin/%.o, $(SRC))

$(info $(shell mkdir -p bin))
$(info $(shell mkdir -p lib))

all: $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) -o prog

bin/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

pcc:
	packcc -a -o pcc src/pcc/grammar.peg
	mv --update pcc.h pcc.c --target-directory=bin
	$(CC) $(CFLAGS) -fPIC -c src/pcc/pcc.c -o bin/pcc.o
	$(CC) $(CFLAGS) -fPIC -c src/pcc/ast.c -o bin/ast.o
	$(CC) -fPIC -shared bin/ast.o bin/pcc.o -o lib/libpcc.so

clean:
	rm -rf bin/*
