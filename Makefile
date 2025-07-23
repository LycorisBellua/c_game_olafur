CC = gcc
SUA_DIR = sua
SUA_LIB = $(SUA_DIR)/libsua.a
SUA_FLAGS = -L$(SUA_DIR) -lsua -lXext -lX11
GL_FLAGS = -lGL -Llib -lglfw3
CFLAGS = -Iinclude -Wall -Wextra -pedantic -O2
AL_FLAGS = -Llib -lopenal -Wl,-rpath,'$$ORIGIN/lib'
LDFLAGS = -lm $(AL_FLAGS)
NAME = olafur
NAME_W64 = Olafur-64bit.exe
NAME_W32 = Olafur-32bit.exe

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
OBJ = $(patsubst %.c, build/%.o, $(SRC))
LDFLAGS += $(WINDOWING_FLAGS)

all: $(SUA_LIB) $(NAME)

sua: $(SUA_LIB)
	@$(MAKE) WINDOWING=SUA

gl:
	@$(MAKE) WINDOWING=GL

# Package: gcc-mingw-w64
# After compilation, place lib/win64/OpenAL32.dll next to the executable
win64:
	$(MAKE) fclean
	$(MAKE) gl CC=x86_64-w64-mingw32-gcc NAME=$(NAME_W64) \
		AL_FLAGS="-mwindows -Wl,-Bstatic -lpthread -Wl,-Bdynamic -Llib/win64 -lOpenAL32" \
		GL_FLAGS="-Llib/win64 -lglfw3 -lopengl32 -lgdi32"

# Package: gcc-mingw-w64
# After compilation, place lib/win32/OpenAL32.dll next to the executable
win32:
	$(MAKE) fclean
	$(MAKE) gl CC=i686-w64-mingw32-gcc NAME=$(NAME_W32) \
		AL_FLAGS="-mwindows -Wl,-Bstatic -lpthread -Wl,-Bdynamic -Llib/win32 -lOpenAL32" \
		GL_FLAGS="-Llib/win32 -lglfw3 -lopengl32 -lgdi32"

$(SUA_LIB):
	@if [ "$(WINDOWING)" = "SUA" ]; then \
		make -s -C $(SUA_DIR); \
	fi

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

build/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -D$(WINDOWING) -o $@ -c $<

clean:
	rm -rf $(shell find . -name '*.o')

fclean: clean
	rm -rf $(NAME) $(NAME_W64) $(NAME_W32)

re: fclean all

.PHONY: all sua gl win64 win32 clean fclean re
