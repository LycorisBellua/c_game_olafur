CC = gcc
SUA_DIR = sua
SUA_LIB = $(SUA_DIR)/libsua.a
SUA_FLAGS = -L$(SUA_DIR) -lsua -lXext -lX11
GL_FLAGS = -lGL -Llib -lglfw34
CFLAGS = -Iinclude -I$(SUA_DIR) -Wall -Wextra -pedantic -O2# -g -fsanitize=address
AL_FLAGS = -Llib -lopenal -Wl,-rpath,'$$ORIGIN/lib'
LDFLAGS = -lm $(AL_FLAGS)
NAME = cimmerian

SRC_COMMON = $(shell find lib -path "$(SUA_DIR)" -prune -o -name '*.c' -print) \
	$(shell find src -path "src/windowing" -prune -o -name '*.c' -print)
WINDOWING ?= SUA
ifeq ($(WINDOWING), GL)
	WINDOWING_SRC = $(shell find src/windowing/gl -name '*.c')
	WINDOWING_FLAGS = $(GL_FLAGS)
else
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
	rm -rf $(NAME)

re: fclean all

.PHONY: all sua gl clean fclean re
