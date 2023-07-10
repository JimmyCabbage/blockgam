#include "g_board.h"

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

struct board_s
{
    uint8_t* grid;
};

#define GRID_SIZE (GRID_WIDTH * GRID_HEIGHT * sizeof(uint8_t))

board_t* G_CreateBoard()
{
    board_t* board = malloc(sizeof(board_t));
    
    board->grid = malloc(GRID_SIZE);
    memset(board->grid, 0, GRID_SIZE);
    
    return board;
}

void G_DestroyBoard(board_t* board)
{
    free(board->grid);
    
    free(board);
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

void G_CheckBoardClear(board_t* board)
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
    }
}
