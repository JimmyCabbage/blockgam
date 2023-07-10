#ifndef TEBRIS_M_MENU_H
#define TEBRIS_M_MENU_H

#include <stdint.h>
#include <string.h>

struct menuitem_s;

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

menu_t* M_Init();

int32_t M_AddList(menu_t* menu, menulist_t* list);

menulist_t* M_GetList(menu_t* menu);

void M_SetList(menu_t* menu, int32_t list);

size_t M_GetCurrentItem(menu_t* menu);

void M_NextCurrentItem(menu_t* menu);

void M_BackCurrentItem(menu_t* menu);

void M_UseCurrentItem(menu_t* menu);

void M_Quit(menu_t* menu);

#endif //TEBRIS_M_MENU_H
