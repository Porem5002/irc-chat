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
    char* text;
};

typedef struct ChatView ChatView;
struct ChatView
{
    size_t item_count;
    ChatItem* items;
};

void chat_view_push(ChatView* chat, char* text);
void chat_view_draw(const ChatView* chat, TextStyle style, Vector2 start_pos, Color content_color);

bool text_input_is_full(const TextInput* input);
bool text_input_is_empty(const TextInput* input);
void text_input_push_char(TextInput* input, char c);
void text_input_pop_char(TextInput* input);
void text_input_clear(TextInput* input);

Vector2 measure_styled_text(TextStyle style, const char* text);
void draw_styled_text(TextStyle style, const char* text, Vector2 position, Color color);

#endif