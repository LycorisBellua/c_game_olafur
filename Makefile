CC = gcc
MLX_DIR = lib/mlx
MLX_LIB = $(MLX_DIR)/libmlx.a
MLX_FLAGS = -L$(MLX_DIR) -lmlx -lXext -lX11
GL_FLAGS = -lGL -Llib -lglfw34
CFLAGS = -Iinclude -I$(MLX_DIR) -Wall -Wextra -Werror -pedantic -O2#-g -fsanitize=address
LDFLAGS = -lm
NAME = cimmerian

SRC_COMMON = $(shell find lib -path "$(MLX_DIR)" -prune -o -name '*.c' -print) \
	$(shell find src -path "src/engine" -prune -o -name '*.c' -print)
ENGINE ?= MLX
ifeq ($(ENGINE), GL)
	ENGINE_SRC = $(shell find src/engine/gl -name '*.c')
	ENGINE_FLAGS = $(GL_FLAGS)
else
	ENGINE_SRC = $(shell find src/engine/mlx -name '*.c')
	ENGINE_FLAGS = $(MLX_FLAGS)
endif
SRC = $(SRC_COMMON) $(ENGINE_SRC)
OBJ = $(SRC:.c=.o)
LDFLAGS += $(ENGINE_FLAGS)

all: $(MLX_LIB) $(NAME)

mlx: $(MLX_LIB)
	@$(MAKE) ENGINE=MLX

gl:
	@$(MAKE) ENGINE=GL

$(MLX_LIB):
	@if [ "$(ENGINE)" = "MLX" ]; then \
		make -s -C $(MLX_DIR); \
	fi

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -D$(ENGINE) -o $@ -c $<

clean:
	rm -rf $(shell find . -name '*.o')

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all mlx gl clean fclean re
