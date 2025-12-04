/*
    Copyright (C) 2023-2025 Ryan Rhee

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

#include "g_board.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "s_alloc.h"

struct board_s
{
    alloc_t* alloc;
    uint8_t* grid;
};

#define GRID_SIZE (GRID_WIDTH * GRID_HEIGHT * sizeof(uint8_t))

board_t* G_CreateBoard(struct alloc_s* alloc)
{
    board_t* board = S_Allocate(alloc, sizeof(board_t));

    board->alloc = alloc;

    board->grid = S_Allocate(board->alloc, GRID_SIZE);
    memset(board->grid, 0, GRID_SIZE);

    return board;
}

void G_DestroyBoard(board_t* board)
{
    if (!board)
    {
        return;
    }

    S_Free(board->alloc, board->grid);

    S_Free(board->alloc, board);
}

void G_ClearBoard(board_t* board)
{
    memset(board->grid, 0, GRID_SIZE);
}

uint8_t G_GetBoardSpace(board_t* board, int x, int y)
{
    return board->grid[GRID_WIDTH * y + x];
}

void G_SetBoardSpace(board_t* board, int x, int y, uint8_t val)
{
    board->grid[GRID_WIDTH * y + x] = val;
}

bool G_TryBoardClear(board_t* board)
{
    int lineClear = -1;

    for (int j = 0; j < GRID_HEIGHT; j++)
    {
        bool validLine = true;

        for (int i = 0; i < GRID_WIDTH; i++)
        {
            if (G_GetBoardSpace(board, i, j) == 0)
            {
                validLine = false;
                break;
            }
        }

        if (validLine)
        {
            lineClear = j;
            break;
        }
    }

    if (lineClear >= 0)
    {
        if (lineClear == 0)
        {
            memcpy(board->grid, board->grid + GRID_WIDTH, sizeof(uint8_t) * (GRID_HEIGHT - 1) * GRID_WIDTH);
        }
        else if (lineClear == GRID_HEIGHT - 1)
        {
            memset(board->grid + (GRID_WIDTH * (GRID_HEIGHT - 1)), 0, sizeof(uint8_t) * GRID_WIDTH);
        }
        else
        {
            memcpy(board->grid + (GRID_WIDTH * lineClear), board->grid + (GRID_WIDTH * (lineClear + 1)), sizeof(uint8_t) * (GRID_HEIGHT - lineClear - 1) * GRID_WIDTH);
        }

        return true;
    }

    return false;
}
