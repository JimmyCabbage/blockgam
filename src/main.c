#include "SDL.h"

#include "g_main.h"

int main(int argc, char** argv)
{
    game_t* game = G_Init();
    
    G_RunGame(game);
    
    G_Quit(game);
    
    return 0;
}
