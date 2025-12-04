/*
    Copyright (C) 2023-2024 Ryan Rhee

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

#ifndef TEBRIS_M_MENU_H
#define TEBRIS_M_MENU_H

#include <stdint.h>
#include <string.h>

struct menuitem_s;
struct alloc_s;

typedef void (*menucallback_t)(struct menuitem_s*);

typedef struct menuitem_s
{
    menucallback_t callback;
    const char* label;
} menuitem_t;

typedef struct menulist_s
{
    menuitem_t** items;
    size_t numItems;
} menulist_t;

typedef struct menu_s menu_t;

menu_t* M_Init(struct alloc_s* alloc);

int32_t M_AddList(menu_t* menu, menulist_t* list);

menulist_t* M_GetList(menu_t* menu);

void M_SetList(menu_t* menu, int32_t list);

size_t M_GetCurrentItem(menu_t* menu);

void M_NextCurrentItem(menu_t* menu);

void M_BackCurrentItem(menu_t* menu);

void M_UseCurrentItem(menu_t* menu);

void M_Quit(menu_t* menu);

#endif  // TEBRIS_M_MENU_H
