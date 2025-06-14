INC	=%%%%
## Please use configure script

UNAME = $(shell uname)
CC	= gcc
ifeq ($(UNAME),FreeBSD)
	CC = clang
endif

NAME = libsua.a
NAME_UNAME = libsua_$(UNAME).a

SRC	= sua_init.c sua_window_create.c sua_window_resize.c sua_window_clear.c \
	sua_window_move.c sua_window_decoration.c sua_window_fullscreen.c \
	sua_loop.c sua_loop_end.c sua_loop_hook.c sua_image_create.c \
	sua_image_to_window.c sua_window_destroy.c sua_int_param_event.c \
	sua_hook.c sua_image_destroy.c sua_mouse_hide.c sua_mouse_show.c \
	sua_screen_size.c sua_display_destroy.c

OBJ_DIR = obj
OBJ = $(addprefix $(OBJ_DIR)/,$(SRC:%.c=%.o))
CFLAGS = -O3 -I$(INC)

all: $(NAME)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

$(NAME): $(OBJ)
	ar -r $(NAME) $(OBJ)
	ranlib $(NAME)
	cp $(NAME) $(NAME_UNAME)

clean:
	rm -rf $(OBJ_DIR)/ $(NAME) $(NAME_UNAME) *~ core *.core

.PHONY: all clean
