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

#include "g_piece.h"

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "s_alloc.h"
#include "g_board.h"

struct piece_s
{
    alloc_t* alloc;
    piecetype_t type;
    uint8_t* data;
    uint8_t* oldData;
    int x;
    int y;
};

#define PIECE_SIZE (sizeof(uint8_t) * PIECE_WIDTH * PIECE_HEIGHT)
#define PIECE_HALF_WIDTH (PIECE_WIDTH / 2)
#define PIECE_HALF_HEIGHT (PIECE_HEIGHT / 2)

inline static uint8_t* GetLoc(uint8_t* data, int x, int y)
{
    //normalize because (0, 0) is the center of our piece data thing
    const int normX = x + PIECE_HALF_WIDTH;
    const int normY = y + PIECE_HALF_HEIGHT;
    
    return data + (PIECE_WIDTH * normY + normX);
}

inline static void SetPieceSpace(piece_t* piece, int x, int y, uint8_t val)
{
    uint8_t* loc = GetLoc(piece->data, x, y);
    *loc = val;
}

piece_t* G_CreatePiece(struct alloc_s* alloc, piecetype_t type, int x, int y)
{
    piece_t* piece = S_Allocate(alloc, sizeof(piece_t));
   
    piece->alloc = alloc;

    piece->type = type;
    
    piece->data = S_Allocate(alloc, PIECE_SIZE);
    
    piece->oldData = S_Allocate(alloc, PIECE_SIZE);
    
    piece->x = x;
    piece->y = y;
    
    switch (type)
    {
    case PIECETYPE_T:
        SetPieceSpace(piece, 0, 1, 1);
        SetPieceSpace(piece, 0, -1, 1);
        SetPieceSpace(piece, 0, 0, 1);
        SetPieceSpace(piece, -1, 0, 1);
        break;
    case PIECETYPE_L:
        SetPieceSpace(piece, 0, 0, 2);
        SetPieceSpace(piece, 0, 1, 2);
        SetPieceSpace(piece, 0, -1, 2);
        SetPieceSpace(piece, 1, -1, 2);
        break;
    case PIECETYPE_J:
        SetPieceSpace(piece, 0, 0, 3);
        SetPieceSpace(piece, 0, 1, 3);
        SetPieceSpace(piece, 0, -1, 3);
        SetPieceSpace(piece, 1, 1, 3);
        break;
    case PIECETYPE_BLOCK:
        SetPieceSpace(piece, 0, 0, 4);
        SetPieceSpace(piece, 0, 1, 4);
        SetPieceSpace(piece, 1, 0, 4);
        SetPieceSpace(piece, 1, 1, 4);
        break;
    case PIECETYPE_S:
        SetPieceSpace(piece, 0, 0, 5);
        SetPieceSpace(piece, 0, 1, 5);
        SetPieceSpace(piece, -1, 0, 5);
        SetPieceSpace(piece, 1, 1, 5);
        break;
    case PIECETYPE_Z:
        SetPieceSpace(piece, 0, 0, 6);
        SetPieceSpace(piece, 0, 1, 6);
        SetPieceSpace(piece, 1, 0, 6);
        SetPieceSpace(piece, -1, 1, 6);
        break;
    case PIECETYPE_LONG:
        SetPieceSpace(piece, 0, -1, 7);
        SetPieceSpace(piece, 0, 0, 7);
        SetPieceSpace(piece, 0, 1, 7);
        SetPieceSpace(piece, 0, 2, 7);
        break;
    default:
        fprintf(stderr, "Tried to create unavailable piece\n");
        abort();
    }
    
    return piece;
}

void G_DestroyPiece(piece_t* piece)
{
    if (piece)
    {
        return;
    }

    S_Free(piece->alloc, piece->oldData);
    
    S_Free(piece->alloc, piece->data);
    
    S_Free(piece->alloc, piece);
}

inline static bool IsRotatableType(piecetype_t type)
{
    switch (type)
    {
    case PIECETYPE_BLOCK:
        return false;
    default:
        return true;
    }
}

inline static bool CanPiece(piece_t* piece, int xoff, int yoff, board_t* board)
{
    for (int i = -PIECE_HALF_WIDTH; i <= PIECE_HALF_WIDTH; i++)
    {
        for (int j = -PIECE_HALF_HEIGHT; j <= PIECE_HALF_HEIGHT; j++)
        {
            uint8_t c = *GetLoc(piece->data, i, j);
            if (!c)
            {
                continue;
            }
            
            //case for hitting bottom of screen
            if ((piece->y + j + yoff) == -1)
            {
                return false;
            }
            
            //case for hitting right side of screen
            if ((piece->x + i + xoff) >= GRID_WIDTH)
            {
                return false;
            }
            
            //case for hitting left side of screen
            if ((piece->x + i + xoff) < 0)
            {
                return false;
            }
            
            uint8_t t = G_GetBoardSpace(board, piece->x + i + xoff, piece->y + j + yoff);
            if (t > 0)
            {
                return false;
            }
        }
    }
    
    return true;
}

void G_TryPieceLeft(piece_t* piece, board_t* board)
{
    if (CanPiece(piece, -1, 0, board))
    {
        piece->x -= 1;
    }
}

void G_TryPieceRight(piece_t* piece, board_t* board)
{
    if (CanPiece(piece, 1, 0, board))
    {
        piece->x += 1;
    }
}

bool G_TryPieceDrop(piece_t* piece, board_t* board)
{
    if (CanPiece(piece, 0, -1, board))
    {
        piece->y -= 1;
        return true;
    }
    
    return false;
}

void G_TryPieceRotate(piece_t* piece, board_t* board)
{
    if (!IsRotatableType(piece->type))
    {
        return;
    }
    
    memcpy(piece->oldData, piece->data, PIECE_SIZE);
    
    for (int x = -PIECE_HALF_WIDTH; x <= PIECE_HALF_WIDTH; x++)
    {
        for (int y = -PIECE_HALF_HEIGHT; y <= PIECE_HALF_HEIGHT; y++)
        {
            *GetLoc(piece->data, -y, x) = *GetLoc(piece->oldData, x, y);
        }
    }
    
    if (!CanPiece(piece, 0, 0, board))
    {
        memcpy(piece->data, piece->oldData, PIECE_SIZE);
    }
}

void G_InsertPiece(piece_t* piece, board_t* board)
{
    for (int i = -PIECE_HALF_WIDTH; i <= PIECE_HALF_WIDTH; i++)
    {
        for (int j = -PIECE_HALF_HEIGHT; j <= PIECE_HALF_HEIGHT; j++)
        {
            uint8_t c = *GetLoc(piece->data, i, j);
            if (!c)
            {
                continue;
            }
            
            G_SetBoardSpace(board, piece->x + i, piece->y + j, c);
        }
    }
}

uint8_t G_GetPieceSpace(piece_t* piece, int x, int y)
{
    return *GetLoc(piece->data, x, y);
}

int G_GetPieceX(piece_t* piece)
{
    return piece->x;
}

int G_GetPieceY(piece_t* piece)
{
    return piece->y;
}
