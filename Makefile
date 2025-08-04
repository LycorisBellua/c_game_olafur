CC = gcc
SUA_DIR = sua
SUA_LIB = $(SUA_DIR)/libsua.a
SUA_FLAGS = -L$(SUA_DIR) -lsua -lXext -lX11
GL_FLAGS = -lGL -Llib -lglfw3
CFLAGS = -O2 -Iinclude -Wall -Wextra -pedantic -g
AL_FLAGS = -Llib -lopenal -Wl,-rpath,'$$ORIGIN'
LDFLAGS = -lm $(AL_FLAGS)
DIR_BUILD = build
DIR_OBJ = $(DIR_BUILD)/linux/objects
BIN_NAME = olafur
BIN_NAME_WIN = Olafur.exe
BIN = $(DIR_BUILD)/linux/$(BIN_NAME)

SRC_COMMON = $(shell find lib -path "$(SUA_DIR)" -prune -o -name '*.c' -print) \
	$(shell find src -path "src/windowing" -prune -o -name '*.c' -print)
WINDOWING ?= SUA
ifeq ($(WINDOWING), GL)
	WINDOWING_SRC = $(shell find src/windowing/gl -name '*.c')
	WINDOWING_FLAGS = $(GL_FLAGS)
else
	CFLAGS += -I$(SUA_DIR)
	WINDOWING_SRC = $(shell find src/windowing/sua -name '*.c')
	WINDOWING_FLAGS = $(SUA_FLAGS)
endif
SRC = $(SRC_COMMON) $(WINDOWING_SRC)
OBJ = $(patsubst %.c, $(DIR_OBJ)/%.o, $(SRC))
LDFLAGS += $(WINDOWING_FLAGS)

# Linux: After compilation, place lib/libopenal.so.1 next to the exe
# Win: After compilation, place lib/win{64-32}/OpenAL32.dll next to the exe

all: $(SUA_LIB) $(BIN)

sua: $(SUA_LIB)
	@$(MAKE) --no-print-directory WINDOWING=SUA

gl:
	@$(MAKE) --no-print-directory WINDOWING=GL

$(SUA_LIB):
	@if [ "$(WINDOWING)" = "SUA" ]; then \
		make -s -C $(SUA_DIR); \
	fi

$(BIN): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

$(DIR_OBJ)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -D$(WINDOWING) -c $< -o $@

.PHONY: all win64 win32 clean clean-linux clean-win64 clean-win32 fclean 
.PHONY: fclean-linux fclean-win64 fclean-win32 re re-win64 re-win32

# Package: gcc-mingw-w64
win64:
	@$(MAKE) --no-print-directory gl \
	CC=x86_64-w64-mingw32-gcc \
	DIR_OBJ=$(DIR_BUILD)/win64/objects \
	BIN=$(DIR_BUILD)/win64/$(BIN_NAME_WIN) \
	AL_FLAGS="-mwindows -Wl,-Bstatic -lpthread -Wl,-Bdynamic -Llib/win64 -lOpenAL32" \
	GL_FLAGS="-Llib/win64 -lglfw3 -lopengl32 -lgdi32"
win32:
	@$(MAKE) --no-print-directory gl \
	CC=i686-w64-mingw32-gcc \
	DIR_OBJ=$(DIR_BUILD)/win32/objects \
	BIN=$(DIR_BUILD)/win32/$(BIN_NAME_WIN) \
	AL_FLAGS="-mwindows -Wl,-Bstatic -lpthread -Wl,-Bdynamic -Llib/win32 -lOpenAL32" \
	GL_FLAGS="-Llib/win32 -lglfw3 -lopengl32 -lgdi32"

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
