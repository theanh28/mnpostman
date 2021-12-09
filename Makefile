# Check out https://www.cs.colby.edu/maxwell/courses/tutorials/maketutor/ for C Makefile
CC = gcc
IDIR = include
ODIR = src/obj
SDIR = src
LDIR = /usr/lib # remember, dir name with or without / before them are different
# so for safety, you can use ./include instead of include at IDIR above

_LIBS = ssl crypto
LIBS = $(patsubst %,-l %,$(_LIBS))

CFLAGS = -I $(IDIR) -L $(LDIR) $(LIBS)

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