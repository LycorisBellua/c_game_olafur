#ifndef __GAME_H__
#define __GAME_H__

/* Called from the "game" interface object */
void draw_game(void);
void reset_global_coordinates(void);

/* Called from the main loop */
void update_global_coordinates(void);

#endif

