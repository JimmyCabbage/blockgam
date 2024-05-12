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

#include "m_menu.h"

#include <stdlib.h>

#include "s_alloc.h"

struct menu_s
{
    alloc_t* alloc;
    
    menulist_t** lists;
    int32_t numLists;
    
    int32_t currentList;
    size_t currentItem;
};

menu_t* M_Init(struct alloc_s* alloc)
{
    menu_t* menu = S_Allocate(alloc, sizeof(menu_t));
    
    menu->alloc = alloc;

    menu->lists = NULL;
    menu->numLists = 0;
    
    menu->currentList = -1;
    menu->currentItem = 0;
    
    return menu;
}

int32_t M_AddList(menu_t* menu, menulist_t* list)
{
    const int32_t currentList = menu->numLists++;
    
    menu->lists = S_Reallocate(menu->alloc, menu->lists, menu->numLists * sizeof(menulist_t*));
    
    menu->lists[currentList] = list;
    
    return currentList;
}

menulist_t* M_GetList(menu_t* menu)
{
    return menu->lists[menu->currentList];
}

void M_SetList(menu_t* menu, int32_t list)
{
    menu->currentList = list;
}

size_t M_GetCurrentItem(menu_t* menu)
{
    return menu->currentItem;
}

void M_NextCurrentItem(menu_t* menu)
{
    if (menu->lists[menu->currentList]->numItems - 1 > menu->currentItem)
    {
        menu->currentItem += 1;
    }
}

void M_BackCurrentItem(menu_t* menu)
{
    if (0 < menu->currentItem)
    {
        menu->currentItem -= 1;
    }
}

void M_UseCurrentItem(menu_t* menu)
{
    menuitem_t* currItem = menu->lists[menu->currentList]->items[menu->currentItem];
    currItem->callback(currItem);
}

void M_Quit(menu_t *menu)
{
    if (!menu)
    {
        return;
    }

    for (size_t i = 0; i < (size_t)menu->numLists; i++)
    {
        for (size_t j = 0; j < menu->lists[i]->numItems; j++)
        {
            S_Free(menu->alloc, menu->lists[i]->items[j]);
        }
        
        S_Free(menu->alloc, menu->lists[i]->items);
        
        S_Free(menu->alloc, menu->lists[i]);
    }
    
    S_Free(menu->alloc, menu->lists);
    
    S_Free(menu->alloc, menu);
}
