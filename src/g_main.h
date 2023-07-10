#ifndef TEBRIS_G_MAIN_H
#define TEBRIS_G_MAIN_H

typedef enum gamestate_e
{
    GAMESTATE_MENU,
    GAMESTATE_PLAY,
    GAMESTATE_FAIL
} gamestate_t;

typedef struct game_s game_t;

game_t* G_Init();

void G_RunGame(game_t* game);

void G_Quit(game_t* game);

#endif //TEBRIS_G_MAIN_H
