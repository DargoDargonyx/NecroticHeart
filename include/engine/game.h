/**
 * @file game.h
 * @author DargoDargonyx
 * @date 08/11/2026
 */

#ifndef GAME_H
#define GAME_H

#define GAME_NAME "NecroticHeart"

// Helper functions
int init_game(void);
void quit_game(int);
static inline void exit_game_successfully(void) { quit_game(0); }
static inline void exit_game_unsuccessfully(void) { quit_game(1); }

// Game loop
int run_game_loop(void);

#endif // GAME_H
