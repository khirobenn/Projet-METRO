CC = gcc
CFLAGS = -Wall
SFMLFLAG = -lcsfml-graphics -lcsfml-window -lcsfml-system -lcsfml-audio
SRC = abr_type.c ligne.c liste.c truc.c aqrtopo.c test.c
OBJ = $(SRC:.c=.o)
BIN = maker

all: $(OBJ)
	$(CC) $(CFLAGS) $^ $(SFMLFLAG) -o test
%.o: %.c
	$(CC) -c $<

clean :
	rm *.o test