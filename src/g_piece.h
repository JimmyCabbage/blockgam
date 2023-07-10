#ifndef TEBRIS_G_PIECE_H
#define TEBRIS_G_PIECE_H

#include <stdbool.h>
#include <stdint.h>

struct board_s;

#define PIECE_WIDTH (5)
#define PIECE_HEIGHT (5)

typedef struct piece_s piece_t;

typedef enum piecetype_e
{
    PIECETYPE_T,
    PIECETYPE_L,
    PIECETYPE_J,
    PIECETYPE_BLOCK,
    PIECETYPE_S,
    PIECETYPE_Z,
    PIECETYPE_LONG,
    PIECETYPE_END
} piecetype_t;

piece_t* G_CreatePiece(piecetype_t type, int x, int y);

void G_DestroyPiece(piece_t* piece);

void G_TryPieceLeft(piece_t* piece, struct board_s* board);

void G_TryPieceRight(piece_t* piece, struct board_s* board);

bool G_TryPieceDrop(piece_t* piece, struct board_s* board);

void G_TryPieceRotate(piece_t* piece, struct board_s* board);

void G_InsertPiece(piece_t* piece, struct board_s* board);

uint8_t G_GetPieceSpace(piece_t* piece, int x, int y);

int G_GetPieceX(piece_t* piece);

int G_GetPieceY(piece_t* piece);

#endif //TEBRIS_G_PIECE_H
