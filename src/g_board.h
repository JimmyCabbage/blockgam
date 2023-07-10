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
