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

#include "s_alloc.h"

#include <stdio.h>
#include <stdlib.h>

struct alloc_s
{
    int totalAlloc;
    //empty for now
};

alloc_t* S_CreateAlloc()
{
    alloc_t* alloc = malloc(sizeof(alloc_t));
    alloc->totalAlloc = 0;

    return alloc;
}

void S_DestroyAlloc(alloc_t* alloc)
{
    if (!alloc)
    {
        return;
    }

    printf("Bytes allocated: %d\n", alloc->totalAlloc);
    free(alloc);
}

void* S_Allocate(alloc_t* alloc, size_t size)
{
    if (!alloc)
    {
        fputs("Tried to alloc with null alloc", stderr);
        return NULL;
    }

    void* ptr = malloc(size);
    memset(ptr, 0, size);

    alloc->totalAlloc += size;

    return ptr;
}

void* S_Reallocate(alloc_t* alloc, void* ptr, size_t size)
{
    return realloc(ptr, size);
}

void S_Free(alloc_t* alloc, void* ptr)
{
    free(ptr);
}

