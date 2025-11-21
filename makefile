CC = gcc
CFLAGS = -Wall
SRC = abr_type.c ligne.c liste.c truc.c aqrtopo.c
OBJ = $(SRC:.c=.o)
BIN = maker

all: $(OBJ)
	$(CC) $(CFLAGS) test.c $^ -o test
%.o: %.c
	$(CC) -c $<

clean :
	rm *.o test



