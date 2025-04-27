##
## Makefile for MiniLibX in /home/boulon/work/c/raytraceur/minilibx
## 
## Made by Olivier Crouzet
## Login   <ol@epitech.net>
## 
## Started on  Tue Oct  5 15:56:43 2004 Olivier Crouzet
## Last update Tue May 15 15:41:20 2007 Olivier Crouzet
##

## Please use configure script


INC	=%%%%

UNAME = $(shell uname)
CC	= gcc
ifeq ($(UNAME),FreeBSD)
	CC = clang
endif

NAME		= libmlx.a
NAME_UNAME	= libmlx_$(UNAME).a

SRC	= mlx_init.c mlx_window_create.c mlx_window_resize.c mlx_window_clear.c \
	mlx_window_move.c mlx_window_decoration.c mlx_window_fullscreen.c \
	mlx_loop.c mlx_loop_end.c mlx_loop_hook.c \
	mlx_image_create.c mlx_image_to_window.c mlx_window_destroy.c \
	mlx_int_param_event.c mlx_hook.c \
	mlx_image_destroy.c mlx_mouse_hide.c mlx_mouse_show.c \
	mlx_screen_size.c mlx_display_destroy.c

OBJ_DIR = obj
OBJ	= $(addprefix $(OBJ_DIR)/,$(SRC:%.c=%.o))
CFLAGS	= -O3 -I$(INC)

all	: $(NAME)

$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) $(IFLAGS) -c $< -o $@

$(NAME)	: $(OBJ)
	ar -r $(NAME) $(OBJ)
	ranlib $(NAME)
	cp $(NAME) $(NAME_UNAME)

check: all
	@test/run_tests.sh

show:
	@printf "NAME  		: $(NAME)\n"
	@printf "NAME_UNAME	: $(NAME_UNAME)\n"
	@printf "CC		: $(CC)\n"
	@printf "CFLAGS		: $(CFLAGS)\n"
	@printf "SRC		:\n	$(SRC)\n"
	@printf "OBJ		:\n	$(OBJ)\n"

clean	:
	rm -rf $(OBJ_DIR)/ $(NAME) $(NAME_UNAME) *~ core *.core

.PHONY: all check show clean
