#include "m_menu.h"

#include <stdlib.h>

struct menu_s
{
    menulist_t** lists;
    int32_t numLists;
    
    int32_t currentList;
    size_t currentItem;
};

menu_t* M_Init()
{
    menu_t* menu = malloc(sizeof(menu_t));
    
    menu->lists = NULL;
    menu->numLists = 0;
    
    menu->currentList = -1;
    menu->currentItem = 0;
    
    return menu;
}

int32_t M_AddList(menu_t* menu, menulist_t* list)
{
    const int32_t currentList = menu->numLists++;
    
    menu->lists = realloc(menu->lists, menu->numLists * sizeof(menulist_t*));
    
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
    for (size_t i = 0; i < (size_t)menu->numLists; i++)
    {
        for (size_t j = 0; j < menu->lists[i]->numItems; j++)
        {
            free(menu->lists[i]->items[j]);
        }
        
        free(menu->lists[i]->items);
        
        free(menu->lists[i]);
    }
    
    free(menu->lists);
    
    free(menu);
}
