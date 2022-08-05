CC=clang
CFLAGS=-Wall -Wextra -pedantic
LDFLAGS=-fPIC -shared -Wall

SRCDIR=src
OBJDIR=obj
LIBDIR=lib

DIRS=$(wildcard $(SRCDIR)/*/)

DEPS=$(wildcard *.h $(foreach fd, $(DIRS), $(fd)*.h))
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
	@ echo "compiled '$<' to '$@'"

# utils/

alloc:
	$(CC) $(CFLAGS) -c src/utils/alloc.c -o bin/alloc.o


utils: $(OBJ_UTILS)
#	$(CC) $(CFLAGS) -c $(wildcard src/utils/*.c) -o

# pcc/

$(SRCDIR)/pcc/pcc.c: $(SRCDIR)/pcc/pcc2.peg
	@ packcc -a -l -o pcc src/pcc/pcc2.peg
	@ mv pcc.c pcc.h -t src/pcc
	@ echo "generated parser from '$<'"

$(LIBDIR)/libpcc.so: $(OBJDIR)/pcc/pcc.o
	$(CC) -fPIC -shared $< -o $@

PHONY += pcc
pcc: $(LIBDIR)/libpcc.so

# sym/

sym:
	$(CC) $(CFLAGS) -fPIC -c src/sym/sym2.c -o bin/sym.o
	$(CC) -fPIC -shared bin/sym.o -o lib/libsym.so

$(LIBDIR)/libsym.so: $(OBJDIR)/sym/sym.o
	@ $(CC) $(LDFLAGS) $< -o $@

PHONY += clean
clean:
	@ rm -rfv $(OBJDIR)/* $(LIBDIR)/*

.PHONY = $(PHONY)
