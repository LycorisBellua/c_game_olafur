CC = gcc
CFLAGS = -Wall -Wextra -ansi -pedantic
LDFLAGS = -lglfw -lGL

NAME = Cimmerian
DIR_BUILD = builds
DIR_OBJ = $(DIR_BUILD)/unix/objects

EXE = $(DIR_BUILD)/unix/$(NAME)

SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c, $(DIR_OBJ)/%.o, $(SRC))

all: $(EXE)

$(EXE): $(OBJ)
	@$(CC) $^ -o $@ $(LDFLAGS)

$(DIR_OBJ)/%.o: src/%.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c $< -o $@

# Package: gcc-mingw-w64
.PHONY: win64
.PHONY: win32
win64:
	@$(MAKE) -s all \
	CC=x86_64-w64-mingw32-gcc \
	DIR_OBJ=$(DIR_BUILD)/win64/objects \
	EXE=$(DIR_BUILD)/win64/$(NAME)-64bit.exe
win32:
	@$(MAKE) -s all \
	CC=i686-w64-mingw32-gcc \
	DIR_OBJ=$(DIR_BUILD)/win32/objects \
	EXE=$(DIR_BUILD)/win32/$(NAME)-32bit.exe

.PHONY: clean
.PHONY: clean-unix
.PHONY: clean-win64
.PHONY: clean-win32
clean:
	@$(MAKE) -s clean-unix
	@$(MAKE) -s clean-win64
	@$(MAKE) -s clean-win32
clean-unix:
	@rm -rf builds/unix/
clean-win64:
	@rm -rf builds/win64/
clean-win32:
	@rm -rf builds/win32/

