CC = gcc
CFLAGS = -O2 -Iinclude -Wall -Wextra -pedantic -flto=auto -fno-math-errno
AL_FLAGS = -Llib -lopenal -Wl,-rpath,'$$ORIGIN'
GL_FLAGS = -lGL -Llib -lglfw3
LDFLAGS = -lm $(AL_FLAGS) $(GL_FLAGS)
DIR_BUILD = build
DIR_OBJ = $(DIR_BUILD)/linux/objects
BIN_NAME = olafur
BIN_NAME_WIN = Olafur.exe
BIN = $(DIR_BUILD)/linux/$(BIN_NAME)
SRC = $(shell find lib -name '*.c') $(shell find src -name '*.c')
OBJ = $(patsubst %.c, $(DIR_OBJ)/%.o, $(SRC))

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)
	cp lib/libopenal.so.1 $(DIR_BUILD)/linux/

$(DIR_OBJ)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: all win64 win32
.PHONY: clean clean-linux clean-win64 clean-win32
.PHONY: fclean fclean-linux fclean-win64 fclean-win32
.PHONY: re re-win64 re-win32

# Package: gcc-mingw-w64
win64:
	@$(MAKE) --no-print-directory \
	CC=x86_64-w64-mingw32-gcc \
	DIR_OBJ=$(DIR_BUILD)/win64/objects \
	BIN=$(DIR_BUILD)/win64/$(BIN_NAME_WIN) \
	AL_FLAGS="-mwindows -Wl,-Bstatic -lpthread -Wl,-Bdynamic -Llib/win64 -lOpenAL32" \
	GL_FLAGS="-Llib/win64 -lglfw3 -lopengl32 -lgdi32"
	cp lib/win64/OpenAL32.dll $(DIR_BUILD)/win64/
win32:
	@$(MAKE) --no-print-directory \
	CC=i686-w64-mingw32-gcc \
	DIR_OBJ=$(DIR_BUILD)/win32/objects \
	BIN=$(DIR_BUILD)/win32/$(BIN_NAME_WIN) \
	CFLAGS="-msse2 -mfpmath=sse $(CFLAGS)" \
	AL_FLAGS="-mwindows -Wl,-Bstatic -lpthread -Wl,-Bdynamic -Llib/win32 -lOpenAL32" \
	GL_FLAGS="-Llib/win32 -lglfw3 -lopengl32 -lgdi32"
	cp lib/win32/OpenAL32.dll $(DIR_BUILD)/win32/

clean:
	@$(MAKE) --no-print-directory clean-linux
	@$(MAKE) --no-print-directory clean-win64
	@$(MAKE) --no-print-directory clean-win32
clean-linux:
	rm -rf $(DIR_BUILD)/linux/objects
clean-win64:
	rm -rf $(DIR_BUILD)/win64/objects
clean-win32:
	rm -rf $(DIR_BUILD)/win32/objects

fclean:
	@$(MAKE) --no-print-directory fclean-linux
	@$(MAKE) --no-print-directory fclean-win64
	@$(MAKE) --no-print-directory fclean-win32
fclean-linux: clean-linux
	rm -rf $(DIR_BUILD)/linux/$(BIN_NAME)
fclean-win64: clean-win64
	rm -rf $(DIR_BUILD)/win64/$(BIN_NAME_WIN)
fclean-win32: clean-win32
	rm -rf $(DIR_BUILD)/win32/$(BIN_NAME_WIN)

re: fclean-linux all

re-win64: fclean-win64 win64

re-win32: fclean-win32 win32
