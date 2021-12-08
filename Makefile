# Check out https://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/ for C Makefile
CC = gcc
IDIR = include
CFLAGS = -I $(IDIR)
ODIR = src/obj
SDIR = src

_DEPS = connect_mod.h request_mod.h
DEPS = $(patsubst %,$(IDIR)/%,$(_DEPS))

_OBJ = client.o connect_mod.o request_mod.o
OBJ = $(patsubst %,$(ODIR)/%,$(_OBJ));

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)
	@echo $@ in $(ODIR)

mnpostman: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

.PHONY: clean

clean:
	rm -r src/obj
	mkdir src/obj
	rm mnpostman