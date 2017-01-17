CC = gcc

CFLAGS 	= -Wall -Werror -pedantic
CFLAGS += -D_XOPEN_SOURCE=500
CFLAGS += -g
CFLAGS += -m32

OBJ = sem.o hw.o 

all: $(OBJ) sem.h hw.h
	$(CC) $(CFLAGS) -o $@ $(OBJ)
	
.PHONY: clean realclean
clean:
	$(RM) $(OBJ)
realclean : clean
	$(RM) prodcons

