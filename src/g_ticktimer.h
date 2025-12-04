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

#ifndef TEBRIS_G_TICKTIMER_H
#define TEBRIS_G_TICKTIMER_H

#include <stdint.h>

struct alloc_s;

typedef struct ticktimer_s ticktimer_t;

#define TICK_RATE (64)

ticktimer_t* G_CreateTimer(struct alloc_s* alloc);

void G_DestroyTimer(ticktimer_t* timer);

uint64_t G_GetTimerTicks(ticktimer_t* timer);

#endif  // TEBRIS_G_TICKTIMER_H
