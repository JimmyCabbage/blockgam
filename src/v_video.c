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

#include "v_video.h"

#include "SDL.h"
#include "SDL_ttf.h"

#include "m_menu.h"
#include "g_board.h"
#include "g_piece.h"

typedef struct internal_texture_s
{
    char* name;
    SDL_Texture* handle;
    int width;
    int height;
} internal_texture_t;

struct video_s
{
    SDL_Window* window;
    SDL_Renderer* renderer;
    
    TTF_Font* font;
    
    int width;
    int height;
    
    bool visible;
    
    internal_texture_t* textureCache;
    size_t textureCacheLength;
};

video_t* V_Init(int width, int height)
{
    video_t* video = malloc(sizeof(video_t));
    
    //who needs error checking
    SDL_InitSubSystem(SDL_INIT_VIDEO);
    
    video->window = SDL_CreateWindow("blockgam", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    
    video->renderer = SDL_CreateRenderer(video->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    
    TTF_Init();
    
    video->font = TTF_OpenFont("consolas.ttf", 24);
    
    video->width = width;
    video->height = height;
    
    video->visible = true;
    
    video->textureCache = NULL;
    video->textureCacheLength = 0;
    
    return video;
}

void V_Clear(video_t* video)
{
    SDL_SetRenderDrawColor(video->renderer, 0, 0, 0, 255);
    SDL_RenderClear(video->renderer);
}

void V_SetVisibility(video_t* video, bool visible)
{
    video->visible = visible;
}

inline static internal_texture_t* GetCachedTexture(video_t* video, const char* text)
{
    for (size_t i = 0; i < video->textureCacheLength; i++)
    {
        if (strcmp(video->textureCache[i].name, text) == 0)
        {
            return &video->textureCache[i];
        }
    }
    
    const size_t newTextureNum = video->textureCacheLength++;
    video->textureCache = realloc(video->textureCache, sizeof(internal_texture_t) * video->textureCacheLength);
    
    SDL_Color textColor = { 255, 255, 255, 255 };
    SDL_Surface* surface = TTF_RenderText_Solid(video->font, text, textColor);
    
    internal_texture_t* texture = video->textureCache + newTextureNum;
    
    size_t nameLength = strlen(text);
    texture->name = malloc(nameLength + 1);
    memset(texture->name, 0, nameLength + 1);
    memcpy(texture->name, text, nameLength);
    
    texture->handle = SDL_CreateTextureFromSurface(video->renderer, surface);
    
    texture->width = surface->w;
    texture->height = surface->h;
    
    SDL_FreeSurface(surface);
    
    return texture;
}

static void DrawText(video_t* video, int x, int y, const char* text)
{
    internal_texture_t* texture = GetCachedTexture(video, text);
    
    SDL_Rect rect =
    {
        .x = x,
        .y = y,
        .w = texture->width,
        .h = texture->height
    };
    
    SDL_RenderCopy(video->renderer, texture->handle, NULL, &rect);
}

void V_DrawMenu(video_t* video, menu_t* menu)
{
    menulist_t* list = M_GetList(menu);
    if (!list)
    {
        return;
    }
    
    for (size_t i = 0; i < list->numItems; i++)
    {
        SDL_SetRenderDrawColor(video->renderer, 255, 0, 0, 1);
        DrawText(video, 100, (int)i * 75 + 100, list->items[i]->label);
    }
    
    const size_t currItem = M_GetCurrentItem(menu);
    SDL_Rect selectRect =
    {
        .x = 50,
        .y = (int)currItem * 75 + 105,
        .w = 8,
        .h = 8
    };
    SDL_RenderFillRect(video->renderer, &selectRect);
}

inline static int CalculatePixelSize(video_t* video)
{
    return video->height / 40;
}

#define GREY_COLOR (25)

inline static void DrawRectBoardSpace(video_t* video, int x, int y, uint8_t c)
{
    SDL_Color color = { .a = 1 };
    switch (c)
    {
    case 0:
        color.r = 0;
        color.g = 0;
        color.b = 0;
        break;
    case 1:
        color.r = 255;
        color.g = 0;
        color.b = 0;
        break;
    case 2:
        color.r = 0;
        color.g = 255;
        color.b = 0;
        break;
    case 3:
        color.r = 0;
        color.g = 0;
        color.b = 255;
        break;
    case 4:
        color.r = 255;
        color.g = 255;
        color.b = 0;
        break;
    case 5:
        color.r = 255;
        color.g = 128;
        color.b = 0;
        break;
    case 6:
        color.r = 0;
        color.g = 200;
        color.b = 255;
        break;
    case 7:
        color.r = 200;
        color.g = 0;
        color.b = 255;
        break;
    case GREY_COLOR:
        color.r = 200;
        color.g = 200;
        color.b = 200;
        break;
    default:
        color.r = 255;
        color.g = 255;
        color.b = 255;
        break;
    }
    
    SDL_SetRenderDrawColor(video->renderer, color.r, color.g, color.b, color.a);
    
    const int pixelSize = CalculatePixelSize(video);
    
    const int startX = (video->width - (pixelSize * (GRID_WIDTH + 2))) / 2;
    const int startY = (video->height / 8);
    
    SDL_Rect drect =
    {
        .x = x * pixelSize + startX,
        .y = video->height - y * pixelSize - startY,
        .w = pixelSize,
        .h = pixelSize
    };
    SDL_RenderFillRect(video->renderer, &drect);
}

void V_DrawBoard(video_t* video, board_t* board)
{
    //draw play space
    for (int x = 0; x < GRID_WIDTH; x++)
    {
        for (int y = 0; y < GRID_HEIGHT; y++)
        {
            DrawRectBoardSpace(video, x, y, G_GetBoardSpace(board, x, y));
        }
    }
    
    //draw bottom of playspace
    for (int x = -1; x <= GRID_WIDTH; x++)
    {
        DrawRectBoardSpace(video, x, -1, GREY_COLOR);
    }
    
    //draw left side of playspace
    for (int y = 0; y <= GRID_HEIGHT; y++)
    {
        DrawRectBoardSpace(video, -1, y, GREY_COLOR);
    }
    
    //draw right side of playspace
    for (int y = 0; y <= GRID_HEIGHT; y++)
    {
        DrawRectBoardSpace(video, GRID_WIDTH, y, GREY_COLOR);
    }
}

void V_DrawPiece(video_t* video, piece_t* piece)
{
    const int x = G_GetPieceX(piece);
    const int y = G_GetPieceY(piece);
    
    const int halfWidth = PIECE_WIDTH / 2;
    const int halfHeight = PIECE_HEIGHT / 2;
    
    for (int i = -halfWidth; i <= halfWidth; i++)
    {
        for (int j = -halfHeight; j <= halfHeight; j++)
        {
            uint8_t c = G_GetPieceSpace(piece, i, j);
            if (!c)
            {
                continue;
            }
            
            DrawRectBoardSpace(video, x + i , y + j, c);
        }
    }
}

void V_DrawLevel(video_t* video, int level)
{
    static char strBuf[24];
    snprintf(strBuf, sizeof strBuf, "Score: %d", level);
    
    const int pixelSize = CalculatePixelSize(video);
    const int x = (video->width - (pixelSize * (GRID_WIDTH + 2))) / 4;
    
    DrawText(video, x, 100, strBuf);
}

void V_DrawFailure(video_t* video)
{
    DrawText(video, video->width / 3, video->height / 2, "You Are a Failure");
}

void V_Present(video_t* video)
{
    SDL_RenderPresent(video->renderer);
}

void V_Quit(video_t* video)
{
    for (size_t i = 0; i < video->textureCacheLength; i++)
    {
        SDL_DestroyTexture(video->textureCache[i].handle);
        free(video->textureCache[i].name);
    }
    free(video->textureCache);
    
    TTF_CloseFont(video->font);
    
    TTF_Quit();
    
    SDL_DestroyRenderer(video->renderer);
    
    SDL_DestroyWindow(video->window);
    
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
    
    free(video);
}

void V_WindowResized(video_t* video, int w, int h)
{
    video->width = w;
    video->height = h;
}
