#ifndef CLIENT_USER_COLOR_HEADER
#define CLIENT_USER_COLOR_HEADER

#include <stdint.h>
#include <stdbool.h>

#include <raylib.h>

typedef uint8_t UserColorId;

enum 
{
    USER_COLOR_ID_RED,
    USER_COLOR_ID_ORANGE,
    USER_COLOR_ID_YELLOW,
    USER_COLOR_ID_GREEN,
    USER_COLOR_ID_BLUE,
    USER_COLOR_ID_PURPLE,
    USER_COLOR_ID_PINK,

    USER_COLOR_ID_COUNT,
};

bool user_color_str_to_id(const char* s, UserColorId* out_id);
Color user_color_id_to_color(UserColorId id);

#endif