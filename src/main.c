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

#define SDL_MAIN_HANDLED
#include "SDL.h"

#include "s_alloc.h"
#include "g_main.h"

int main(int argc, char** argv)
{
    alloc_t* alloc = S_CreateAlloc();
    if (!alloc)
    {
        fputs("Failed to initialize memory allocator\n", stderr);
		return 1;
    }

    game_t* game = G_Init(alloc);
    if (!game)
    {
        fputs("Failed to initialize game\n", stderr);
        return 1;
    }

    G_RunGame(game);
    
    G_Quit(game);
   
    S_DestroyAlloc(alloc);

    return 0;
}
