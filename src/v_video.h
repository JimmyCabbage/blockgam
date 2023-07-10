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

#ifndef TEBRIS_V_VIDEO_H
#define TEBRIS_V_VIDEO_H

#include <stdint.h>
#include <stdbool.h>

struct menu_s;
struct board_s;
struct piece_s;

typedef struct video_s video_t;

video_t* V_Init(int width, int height);

void V_Clear(video_t* video);

void V_SetVisibility(video_t* video, bool visible);

void V_DrawMenu(video_t* video, struct menu_s* menu);

void V_DrawBoard(video_t* video, struct board_s* board);

void V_DrawPiece(video_t* video, struct piece_s* piece);

void V_DrawLevel(video_t* video, int level);

void V_DrawFailure(video_t* video);

void V_Present(video_t* video);

void V_Quit(video_t* video);

void V_WindowResized(video_t* video, int w, int h);

#endif //TEBRIS_V_VIDEO_H
