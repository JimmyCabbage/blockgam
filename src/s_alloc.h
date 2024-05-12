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

#ifndef TEBRIS_S_ALLOC_H
#define TEBRIS_S_ALLOC_H

#include <string.h>

typedef struct alloc_s alloc_t;

alloc_t* S_CreateAlloc();

void S_DestroyAlloc(alloc_t* alloc);

void* S_Allocate(alloc_t* alloc, size_t size);

void* S_Reallocate(alloc_t* alloc, void* ptr, size_t size);

void S_Free(alloc_t* alloc, void* ptr);

#endif
