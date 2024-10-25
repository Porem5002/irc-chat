#ifndef UI_HEADER
#define UI_HEADER

#include <raylib.h>

typedef struct TextStyle TextStyle;
struct TextStyle
{
    Font font;
    float font_size;
    float spacing;
};

typedef struct TextInput TextInput;
struct TextInput
{
    size_t char_cap;
    size_t char_count;
    char* text;
};

typedef struct ChatItem ChatItem;
struct ChatItem
{
    Color username_color;
    char* username;

    Color content_color;
    char* content;
};

typedef struct ChatView ChatView;
struct ChatView
{
    size_t item_count;
    ChatItem* items;
};

void chat_view_push(ChatView* chat, Color username_color, const char* username, Color content_color, const char* content);
void chat_view_draw(const ChatView* chat, TextStyle style, Vector2 start_pos);

bool text_input_is_full(const TextInput* input);
bool text_input_is_empty(const TextInput* input);
void text_input_push_char(TextInput* input, char c);
void text_input_pop_char(TextInput* input);
void text_input_clear(TextInput* input);

Vector2 measure_styled_text(TextStyle style, const char* text);
void draw_styled_text(TextStyle style, const char* text, Vector2 position, Color color);

#endif