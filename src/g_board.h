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

#ifndef TEBRIS_G_BOARD_H
#define TEBRIS_G_BOARD_H

#include <stdbool.h>
#include <stdint.h>

#define GRID_WIDTH (10)
#define GRID_HEIGHT (30)

typedef struct board_s board_t;

board_t* G_CreateBoard();

void G_DestroyBoard(board_t* board);

void G_ClearBoard(board_t* board);

uint8_t G_GetBoardSpace(board_t* board, int x, int y);

void G_SetBoardSpace(board_t* board, int x, int y, uint8_t val);

void G_CheckBoardClear(board_t* board);

#endif //TEBRIS_G_BOARD_H
