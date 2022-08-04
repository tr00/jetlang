CC=clang
CFLAGS=-Wall -Wextra -pedantic
LDFLAGS=-Wall

SRCDIR=src
OBJDIR=obj
LIBDIR=lib

DIRS=$(wildcard $(SRCDIR)/*/)

SRCS=$(wildcard *.c $(foreach fd, $(DIRS), $(fd)*.c))
OBJS=$(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o, $(SRCS)) # src/xyz/xyz.c => obj/xyz/xyz.o


# $(info $(shell mkdir -p obj))
# $(info $(shell mkdir -p lib))

PHONY := echo

echo:
	@ echo "dirs: $(DIRS)"
	@ echo "srcs: $(SRCS)"
	@ echo "objs: $(OBJS)"

all: $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) -o prog

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@ mkdir -p $(@D)
	@ $(CC) $(CFLAGS) -c $< -o $@
	@ echo "compiled $< to $@"

# utils/

alloc:
	$(CC) $(CFLAGS) -c src/utils/alloc.c -o bin/alloc.o


utils: $(OBJ_UTILS)
	$(CC) $(CFLAGS) -c $(wildcard src/utils/*c) -o

# pcc/

pcc:
	packcc -a -o pcc src/pcc/grammar.peg
	mv pcc.h pcc.c --target-directory=src/pcc
	$(CC) $(CFLAGS) -fPIC -c src/pcc/pcc.c -o bin/pcc.o
	$(CC) $(CFLAGS) -fPIC -c src/pcc/ast.c -o bin/ast.o
	$(CC) -fPIC -shared bin/ast.o bin/pcc.o -o lib/libpcc.so

# sym/

sym:
	$(CC) $(CFLAGS) -fPIC -c src/sym/sym2.c -o bin/sym.o
	$(CC) -fPIC -shared bin/sym.o -o lib/libsym.so

PHONY += clean
clean:
	@ rm -rfv $(OBJDIR)/* $(LIBDIR)/*

.PHONY = $(PHONY)
