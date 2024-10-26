#include <string.h>

#include "user_color.h"

static const char* color_names [USER_COLOR_ID_COUNT] = { 
    [USER_COLOR_ID_RED] = "red",
    [USER_COLOR_ID_ORANGE] = "orange",
    [USER_COLOR_ID_YELLOW] = "yellow",
    [USER_COLOR_ID_GREEN] = "green",
    [USER_COLOR_ID_BLUE] = "blue",
    [USER_COLOR_ID_PURPLE] = "purple",
    [USER_COLOR_ID_PINK] = "pink"
};

bool user_color_str_to_id(const char* s, UserColorId* out_id)
{
    for(UserColorId i = 0; i < USER_COLOR_ID_COUNT; i++)
    {
        if(strcmp(s, color_names[i]) == 0)
        {
            *out_id = i;
            return true;
        }
    }
    
    *out_id = 0;
    return false;
}

Color user_color_id_to_color(UserColorId id)
{
    switch(id)
    {
        case USER_COLOR_ID_RED: return RED;
        case USER_COLOR_ID_ORANGE: return ORANGE;
        case USER_COLOR_ID_YELLOW: return YELLOW;
        case USER_COLOR_ID_GREEN: return GREEN;
        case USER_COLOR_ID_BLUE: return BLUE; 
        case USER_COLOR_ID_PURPLE: return PURPLE;
        case USER_COLOR_ID_PINK: return PINK;
        default: return WHITE;
    }
}