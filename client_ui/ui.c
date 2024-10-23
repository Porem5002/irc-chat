#include <stdlib.h>
#include <string.h>

#include "ui.h"

void chat_view_push(ChatView* chat, char* text)
{
    size_t old_count = chat->item_count;
    size_t new_count = ++chat->item_count;

    chat->items = realloc(chat->items, sizeof(ChatItem) * new_count);
    memmove(chat->items + 1, chat->items, sizeof(ChatItem) * old_count);
    chat->items[0] = (ChatItem){ strdup(text) };
}

void chat_view_draw(const ChatView* chat, TextStyle style, Vector2 start_pos, Color content_color)
{
    Vector2 curr_pos = start_pos;

    for (size_t i = 0; i < chat->item_count; i++)
    {
        ChatItem item = chat->items[i];
        Vector2 content_size = measure_styled_text(style, item.text);

        curr_pos.y -= content_size.y / 2;
        draw_styled_text(style, item.text, curr_pos, content_color);
        curr_pos.y -= content_size.y / 2;
    }
}

bool text_input_is_full(const TextInput* input)
{
    return input->char_count == input->char_cap - 1;
}

bool text_input_is_empty(const TextInput* input)
{
    return input->char_count == 0;
}

void text_input_push_char(TextInput* input, char c)
{
    if(text_input_is_full(input)) return;

    input->text[input->char_count] = c;
    input->text[++input->char_count] = '\0';
}

void text_input_pop_char(TextInput* input)
{
    if(text_input_is_empty(input)) return;

    input->text[--input->char_count] = '\0';
}

void text_input_clear(TextInput* input)
{
    input->text[0] = '\0';
    input->char_count = 0;
}

Vector2 measure_styled_text(TextStyle style, const char* text)
{
    return MeasureTextEx(style.font, text, style.font_size, style.spacing);
}

void draw_styled_text(TextStyle style, const char* text, Vector2 position, Color color)
{
    DrawTextEx(style.font, text, position, style.font_size, style.spacing, color);
}