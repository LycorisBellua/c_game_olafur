#ifndef MACROS_H
# define MACROS_H

# define TITLE "Olafur"
# define WINDOW_WIDTH  (640 * 1)
# define WINDOW_HEIGHT (360 * 1)
# define ASPECT_RATIO (16.0 / 9)
# define ECHOLOCATION 0
# define DEFAULT_DOF 20
# define DEFAULT_MOVE_SPEED 2.0
# define DEFAULT_ROTATE_SPEED 1.0
# define DEFAULT_PLAYER_RADIUS 0.25
# define FONT_PAD    4
# define FONT_SIZE_X 7
# define FONT_SIZE_Y 12
# define FONT_MOD_X  4
# define FONT_MOD_Y  4
# define SPRITE_RADIUS 0.5
# define PORTAL_IS_CORRIDOR 0
# define IMG_JSON_DEFAULT "assets/img/common/index.json"
# define ID_FONT_DEFAULT "font_default"
# define MAP_DEFAULT "assets/maps/olafur/mus.map"
# define MAP_EASTER_EGG "assets/maps/olafur/elv3.map"

# define NBR_KEYS 15
# ifdef GL
#  define KEY_A         GLFW_KEY_A
#  define KEY_B         GLFW_KEY_B
#  define KEY_D         GLFW_KEY_D
#  define KEY_E         GLFW_KEY_E
#  define KEY_Q         GLFW_KEY_Q
#  define KEY_S         GLFW_KEY_S
#  define KEY_W         GLFW_KEY_W
#  define KEY_BACKSPACE GLFW_KEY_BACKSPACE
#  define KEY_ENTER     GLFW_KEY_ENTER
#  define KEY_ESCAPE    GLFW_KEY_ESCAPE
#  define KEY_LEFT      GLFW_KEY_LEFT
#  define KEY_UP        GLFW_KEY_UP
#  define KEY_RIGHT     GLFW_KEY_RIGHT
#  define KEY_DOWN      GLFW_KEY_DOWN
#  define KEY_F11       GLFW_KEY_F11
#  define KEY_F12       GLFW_KEY_F12
#  define KEY_SHIFT     -1
# else
#  define BTN_CLICK_LEFT  1
#  define BTN_CLICK_WHEEL 2
#  define BTN_CLICK_RIGHT 3
#  define BTN_SCROLL_UP   4
#  define BTN_SCROLL_DOWN 5
#  define KEY_A         97
#  define KEY_B         98
#  define KEY_D         100
#  define KEY_E         101
#  define KEY_Q         113
#  define KEY_S         115
#  define KEY_W         119
#  define KEY_BACKSPACE 65288
#  define KEY_ENTER     65293
#  define KEY_ESCAPE    65307
#  define KEY_LEFT      65361
#  define KEY_UP        65362
#  define KEY_RIGHT     65363
#  define KEY_DOWN      65364
#  define KEY_F11       65480
#  define KEY_F12       65481
#  define KEY_SHIFT     65505
# endif

# define E_NO_ARG "Provide a *.map file as argument"
# define E_TOO_MANY_ARGS "Provide a single *.map file as argument"
# define E_FAIL_MEM "Memory cannot be allocated"
# define E_FAIL_SUA "The SUA lib cannot be initialized"
# define E_FAIL_GLFW "The GLFW lib cannot be initialized"
# define E_FAIL_WINDOW "The window cannot be created"
# define E_FAIL_WINDOW_GL "The window cannot be created. The issue may be " \
	"with the GLFW library, but comes more likely from the OpenGL library. " \
	"Check that your drivers are properly installed"
# define E_FAIL_GL_FUNC "The OpenGL functions cannot be loaded"
# define E_FAIL_SUA_IMG "An SUA image cannot be created"
# define E_FAIL_SHADER "The vertex and/or fragment shaders cannot be compiled"
# define E_FAIL_SHADER_PROG "The shader program cannot be compiled"
# define E_FAIL_UNIFORM_FRAME_TEXTURE "The \"frame_texture\" uniform is " \
	"either not found or unused in the shader program"
# define E_FAIL_MP3_OPEN "Failed to open MP3 file"
# define E_FAIL_MP3_READ "Failed to read MP3 data"

/* Parsing ------------------------------------------------------------------ */

# define MAP_CHARS "NSWEDG"

# define E_MAP_READ "The map file cannot be read"
# define E_MAP_TYPE "The map file doesn't have the *.map extension"
# define E_MAP_NAME "The map file doesn't have a name"
# define E_VARS_AFTER_MAP "Variable found after map content"
# define E_NO_MAP "No wall map was found"
# define E_TOO_MANY_MAPS "There are more than three maps"
# define E_NO_VAR "No var was found"
# define E_BAD_JSON "Not a JSON file"
# define E_FAIL_JSON "The JSON file for images cannot be loaded"
# define E_TYPE_0 "Invalid cell type - Valid is [1 - 9]"
# define E_CARD_NOT_FOUR "At least one cardinal is missing for a wall type"
# define E_DUP_VAR "A single-use variable was found more than once"
# define E_VAR_VAL "A value is missing for a variable"
# define E_VAR_VALS "Too many values found for a variable"
# define E_NO_IMG "PNG image not found"
# define E_NO_TRACK "MP3 sound track not found"
# define E_BAD_COLOR "Invalid RGB or hex color"
# define E_BAD_CARD "Invalid cardinal - Valid is \"NO\", \"SO\", \"WE\" or " \
	"\"EA\""
# define E_BAD_OFFSET "Invalid offset for doors - Valid is \"ANY\", " \
	"\"HMID\", \"VMID\", \"NO\", \"SO\", \"WE\" or \"EA\""
# define E_BAD_TOCOLLECT "Invalid number of sprites to collect - Valid is " \
	"[0 - 2147483647]"
# define E_BAD_COORD "Invalid coordinate - Valid is [0 - 15000]"
# define E_BAD_COORD_F "Invalid coordinate - Valid is [0.0 - 15000.99]"
# define E_FLT_EXTRA_DIGIT "Too many digits after the point - Limit is two"
# define E_BAD_SP_TAG "Invalid sprite tag - Valid is \"collectible\" and " \
	"\"collision\""
# define E_BAD_VAR "Invalid variable"
# define E_BAD_SYM_W "A wall map symbol is missing its texture variable"
# define E_BAD_SYM_F "A floor map symbol is missing its texture variable"
# define E_BAD_SYM_C "A ceiling map symbol is missing its texture variable"
# define E_DUP_PLAYER "The player was found more than once in the wall map"
# define E_NO_PLAYER "No player was found in the wall map"

#endif
