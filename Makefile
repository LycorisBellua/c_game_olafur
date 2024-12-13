CC = gcc
CFLAGS = -Iinclude -Wall -Wextra -g#-ansi -pedantic
LDFLAGS = -lGL -Llib -lglfw34 -lm

BIN = cimmerian
SRC = $(wildcard */*.c) $(wildcard */*/*.c)
OBJ = $(SRC:.c=.o)

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -o $@ -c $<

clean:
	rm -rf $(OBJ)

fclean: clean
	rm -rf $(BIN)

re: fclean all

.PHONY: all clean fclean re
