/*
    Copyright (C) 2023-2024 Ryan Rhee

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

#include "g_ticktimer.h"

#include "SDL.h"

#include "s_alloc.h"

struct ticktimer_s
{
    alloc_t* alloc;
    uint64_t startTime;
};

ticktimer_t* G_CreateTimer(struct alloc_s* alloc)
{
    SDL_InitSubSystem(SDL_INIT_TIMER);

    ticktimer_t* timer = S_Allocate(alloc, sizeof(ticktimer_t));

    timer->alloc = alloc;

    timer->startTime = SDL_GetTicks64();

    return timer;
}

void G_DestroyTimer(ticktimer_t* timer)
{
    S_Free(timer->alloc, timer);

    SDL_QuitSubSystem(SDL_INIT_TIMER);
}

uint64_t G_GetTimerTicks(ticktimer_t* timer)
{
    const uint64_t ticks = SDL_GetTicks64() - timer->startTime;
    return (ticks * TICK_RATE) / 1000;
}
