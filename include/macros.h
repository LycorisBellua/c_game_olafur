#ifndef MACROS_H
# define MACROS_H

# define TITLE "Cimmerian"
# define WINDOW_WIDTH  (640 * 1)
# define WINDOW_HEIGHT (360 * 1)
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
# define SPRITE_AMOUNT_TO_COLLECT 5
# define PORTAL_IS_CORRIDOR 0
# define MAP_EASTER_EGG "maps/bonus/olafur/menu3.map"

# ifdef GL
#  define PATH_VERTEX_SHADER "src/engine/gl/shaders/vs.glsl"
#  define PATH_FRAGMENT_SHADER "src/engine/gl/shaders/fs.glsl"
# endif

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
# define E_FAIL_MLX "The MLX lib cannot be initialized"
# define E_FAIL_GLFW "The GLFW lib cannot be initialized"
# define E_FAIL_WINDOW "The window cannot be created"
# define E_FAIL_WINDOW_GL "The window cannot be created. The issue may be " \
	"with the GLFW library, but comes more likely from the OpenGL library. " \
	"Check that your drivers are properly installed"
# define E_FAIL_GL_FUNC "The OpenGL functions cannot be loaded"
# define E_FAIL_MLX_IMG "An MLX image cannot be created"
# define E_FAIL_SHADER "The vertex and/or fragment shaders cannot be compiled"
# define E_FAIL_SHADER_PROG "The shader program cannot be compiled"
# define E_FAIL_UNIFORM_FRAME_TEXTURE "The \"frame_texture\" uniform is " \
	"either not found or unused in the shader program"

/* Parsing ------------------------------------------------------------------ */

# define E_MAP_READ "The map file cannot be read"
# define E_MAP_TYPE "The map file doesn't have the *.map extension"
# define E_MAP_NAME "The map file doesn't have a name"

/* Parsing (Temp) ----------------------------------------------------------- */

//ANNEXE
# define WHITES "\a\b\t\n\v\f\r "
# define VALID_MANDATORY " 01"
# define VALID_MANDA_MAP " 01NSWE\n"
# define VALID_BONUS " GD43210"
# define PLAYER "NSWE"

//ERROR MESSAGES
# define E_NBSPRITES "overreached limit number of sprites"
# define E_OUTTILE "checked tile is too edgy"
# define E_UPTILE "upper tile wasn't a valid map char"
# define E_UNDTILE "under tile wasn't a valid map char"
# define E_LFTTILE "left tile wasn't a valid map char"
# define E_RGTTILE "right tile wasn't a valid map char"
# define E_MISSINFO "missing tex or color"
# define E_MISSPLAYER "missing player"
# define E_EMPTYLINE "find an empty line in map"
# define E_INVALIDCHAR "find an invalid char in map"
# define E_EMPTYMAP "Empty map!"
# define E_DOUBLEMAP "double declaration of a map"
# define E_DOUBLEWALL "double declaration of a wall side"
# define E_DOUBLEPLAYER "double declaration of player"
# define E_DOUBLEFCOL "double declaration of f_color"
# define E_DOUBLECCOL "double declaration of c_color"
# define E_DOUBLESKYBOX "double declaration of skybox"
# define E_DOUBLEMUSIC "double declaration of music"
# define E_WRONGCHAR "parasite char found"
# define E_WRONGMUSIC "not a valid music address"
# define E_INCOMPLETE "unfit declaration"
# define E_INVALIDIDTEX "invalid texture name"

#endif
