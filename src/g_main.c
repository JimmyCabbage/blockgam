/*
    Copyright (C) 2023 Ryan Rhee

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "g_main.h"

#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#include "SDL.h"

#include "s_alloc.h"
#include "v_video.h"
#include "m_menu.h"
#include "g_board.h"
#include "g_piece.h"
#include "g_ticktimer.h"

struct game_s
{
    alloc_t* alloc;

    bool run;
    
    gamestate_t state;
    
    video_t* video;
    
    menu_t* menu;
    
    board_t* board;
    
    int level;
    uint64_t pieceDropSpeed;
    
	bool pieceExists;
    piece_t* currPiece;
    uint64_t currPieceDrop;
    
    ticktimer_t* timer;
    
    uint64_t lastTick;
    
    uint64_t failTimer;
};

typedef struct gameitem_s
{
    menuitem_t item;
    game_t* game;
} gameitem_t;

static void StartGame(menuitem_t* item)
{
    gameitem_t* start = (gameitem_t*)item;
    
    G_ClearBoard(start->game->board);
    start->game->state = GAMESTATE_PLAY;
    
    start->game->level = 0;
    start->game->pieceDropSpeed = 24;
    
    start->game->pieceExists = false;
    start->game->currPieceDrop = 0;
    
    start->game->lastTick = G_GetTimerTicks(start->game->timer);
}

static void Settings(menuitem_t* item)
{
    gameitem_t* settings = (gameitem_t*)item;
}

static void Quit(menuitem_t* item)
{
    gameitem_t* quit = (gameitem_t*)item;
    quit->game->run = false;
}

inline static bool CreateMenus(game_t* game)
{
    menulist_t* mainMenuList = S_Allocate(game->alloc, sizeof(menulist_t));
    if (!mainMenuList)
    {
        fputs("Failed to allocate memory for main menu list\n", stderr);
        return false;
    }

    int32_t mainMenuId = M_AddList(game->menu, mainMenuList);

    mainMenuList->items = S_Allocate(game->alloc, 3 * sizeof(menuitem_t*));
    mainMenuList->numItems = 3;
    
    gameitem_t* start = S_Allocate(game->alloc, sizeof(gameitem_t));
    mainMenuList->items[0] = &start->item;
    start->item.callback = StartGame;
    start->item.label = "Start Game";
    start->game = game;
    
    gameitem_t* settings = S_Allocate(game->alloc, sizeof(gameitem_t));
    mainMenuList->items[1] = &settings->item;
    settings->item.callback = Settings;
    settings->item.label = "Settings";
    settings->game = game;
    
    gameitem_t* quit = S_Allocate(game->alloc, sizeof(gameitem_t));
    mainMenuList->items[2] = &quit->item;
    quit->item.callback = Quit;
    quit->item.label = "Quit";
    quit->game = game;
    
    M_SetList(game->menu, mainMenuId);

    return true;
}

game_t* G_Init(alloc_t* alloc)
{
    SDL_SetMainReady();

    srand(time(NULL));
    
    game_t* game = S_Allocate(alloc, sizeof(game_t));
    if (!game)
    {
        fputs("Failed to allocate memory for game\n", stderr);
        goto fail;
    }

    game->alloc = alloc;

    game->run = false;
    
    game->state = GAMESTATE_MENU;
    
    if (!(game->video = V_Init(alloc, 1024, 724)))
    {
        fputs("Failed to initialize video\n", stderr);
        goto fail;
    }
    
    if (!(game->menu = M_Init(alloc)))
    {
        fputs("Failed to initialize menu\n", stderr);
        goto fail;
    }
    
    if (!(game->board = G_CreateBoard(alloc)))
    {
        fputs("Failed to initialize board\n", stderr);
        goto fail;
    }
    
	game->pieceExists = false;
    game->currPiece = G_AllocatePiece(game->alloc);
    game->currPieceDrop = 0;
    
    game->timer = G_CreateTimer(alloc);
    
    game->lastTick = 0;
    
    if (!CreateMenus(game))
    {
        fputs("Failed to create sub-menus\n", stderr);
        goto fail;
    }
    
    return game;

fail:
	if (game)
		G_Quit(game);

    return NULL;
}

inline static bool TryDropPiece(game_t* game)
{
    if (G_TryPieceDrop(game->currPiece, game->board))
    {
        game->currPieceDrop = game->pieceDropSpeed;
        
        return true;
    }
    
    return false;
}

inline static void ProcessEvents(game_t* game)
{
    SDL_Event ev;
    while (SDL_PollEvent(&ev))
    {
        switch (ev.type)
        {
        case SDL_QUIT:
            game->run = false;
            break;
        case SDL_KEYDOWN:
            if (ev.key.keysym.sym == SDLK_ESCAPE)
            {
                game->run = false;
                break;
            }
            switch (game->state)
            {
            case GAMESTATE_MENU:
                switch (ev.key.keysym.sym)
                {
                case SDLK_UP:
                    M_BackCurrentItem(game->menu);
                    break;
                case SDLK_DOWN:
                    M_NextCurrentItem(game->menu);
                    break;
                case SDLK_RETURN:
                    M_UseCurrentItem(game->menu);
                    break;
                }
                break;
            case GAMESTATE_PLAY:
                if (game->pieceExists)
                {
                    switch (ev.key.keysym.sym)
                    {
                    case SDLK_SPACE:
                    case SDLK_UP:
                        G_TryPieceRotate(game->currPiece, game->board);
                        break;
                    case SDLK_d:
                    case SDLK_RIGHT:
                        G_TryPieceRight(game->currPiece, game->board);
                        break;
                    case SDLK_a:
                    case SDLK_LEFT:
                        G_TryPieceLeft(game->currPiece, game->board);
                        break;
                    case SDLK_s:
                    case SDLK_DOWN:
                        TryDropPiece(game);
                        break;
                    }
                }
                break;
            }
            break;
        case SDL_WINDOWEVENT:
            switch (ev.window.event)
            {
            case SDL_WINDOWEVENT_RESIZED:
                V_WindowResized(game->video, ev.window.data1, ev.window.data2);
                break;
            }
            break;
        }
    }
}

inline static void DrawScreen(game_t* game)
{
    V_Clear(game->video);
    
    switch (game->state)
    {
    case GAMESTATE_MENU:
        V_DrawMenu(game->video, game->menu);
        break;
    case GAMESTATE_PLAY:
        V_DrawBoard(game->video, game->board);
        if (game->currPiece)
        {
            V_DrawPiece(game->video, game->currPiece);
        }
        V_DrawLevel(game->video, game->level);
        break;
    case GAMESTATE_FAIL:
        V_DrawFailure(game->video, game->level);
        break;
    }
    
    V_Present(game->video);
}

static bool ChooseRandomPiece(game_t* game)
{
    const int spawnX = GRID_WIDTH / 2;
    const int spawnY = 24;
    
    if (G_GetBoardSpace(game->board, spawnX, spawnY) != 0)
    {
        return false;
    }
    
    const int type = rand() % PIECETYPE_END;
    game->currPieceDrop = game->pieceDropSpeed;
    if (game->level++ % 5 == 0 && game->pieceDropSpeed > 4)
    {
        game->pieceDropSpeed--;
    }
    
    G_CreatePiece(game->currPiece, type, spawnX, spawnY);
	game->pieceExists = true;

	return true;
}

inline static void TryRunTicks(game_t* game)
{
    if (game->state == GAMESTATE_MENU)
    {
        return;
    }
    
    const uint64_t currTicks = G_GetTimerTicks(game->timer);
    const uint64_t ticks = currTicks - game->lastTick;
    game->lastTick = currTicks;
    
    if (ticks == 0)
    {
        return;
    }
    
    for (uint64_t i = 0; i < ticks; i++)
    {
        switch (game->state)
        {
        case GAMESTATE_PLAY:
            if (!game->pieceExists)
            {
                if (!ChooseRandomPiece(game))
                {
                    game->state = GAMESTATE_FAIL;
                    game->failTimer = TICK_RATE * 4;
                    continue;
                }
            }
            
            if (game->pieceExists)
            {
                if (game->currPieceDrop-- == 0)
                {
                    if (!TryDropPiece(game))
                    {
                        G_InsertPiece(game->currPiece, game->board);
                        game->pieceExists = false;
                    }
                }
            }
            
            if (G_TryBoardClear(game->board) && game->pieceDropSpeed > 4)
            {
                game->pieceDropSpeed--;
            }
            break;
        case GAMESTATE_FAIL:
            if (--game->failTimer == 0)
            {
                game->state = GAMESTATE_MENU;
            }
            break;
        }
    }
}

void G_RunGame(game_t* game)
{
    game->run = true;
    
    while (game->run)
    {
        TryRunTicks(game);
        ProcessEvents(game);
        DrawScreen(game);
    }
    
    game->run = false;
}

void G_Quit(game_t* game)
{
    if (game->currPiece)
		G_DestroyPiece(game->currPiece);
    
	if (game->timer)
		G_DestroyTimer(game->timer);
    
	if (game->board)
		G_DestroyBoard(game->board);
    
	if (game->menu)
		M_Quit(game->menu);
    
	if (game->video)
		V_Quit(game->video);
    
    S_Free(game->alloc, game);
}
