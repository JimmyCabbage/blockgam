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

#ifndef TEBRIS_G_MAIN_H
#define TEBRIS_G_MAIN_H

struct alloc_s;

typedef enum gamestate_e
{
    GAMESTATE_MENU,
    GAMESTATE_PLAY,
    GAMESTATE_FAIL
} gamestate_t;

typedef struct game_s game_t;

game_t* G_Init(struct alloc_s* alloc);

void G_RunGame(game_t* game);

void G_Quit(game_t* game);

#endif //TEBRIS_G_MAIN_H
