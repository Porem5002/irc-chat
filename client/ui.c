#include <stdlib.h>
#include <string.h>

#include "ui.h"

void chat_view_push(ChatView* chat, Color username_color, const char* username, Color content_color, const char* content)
{
    size_t old_count = chat->item_count;
    size_t new_count = ++chat->item_count;

    chat->items = realloc(chat->items, sizeof(ChatItem) * new_count);
    memmove(chat->items + 1, chat->items, sizeof(ChatItem) * old_count);

    chat->items[0] = (ChatItem) {
        .username_color = username_color,
        .username = strdup(username),

        .content_color = content_color,
        .content = strdup(content)
    };
}

void chat_view_draw(const ChatView* chat, TextStyle style, Vector2 start_pos)
{
    const char* separator = ": ";
    Vector2 offset = start_pos;

    for (size_t i = 0; i < chat->item_count; i++)
    {
        ChatItem item = chat->items[i];
        Vector2 username_size = measure_styled_text(style, item.username);
        Vector2 separator_size = measure_styled_text(style, separator);
        Vector2 content_size = measure_styled_text(style, item.content);

        Vector2 pos = offset;
        pos.y -= content_size.y / 2;
        
        draw_styled_text(style, item.username, pos, item.username_color);
        pos.x += username_size.x + style.spacing;

        draw_styled_text(style, separator, pos, item.username_color);
        pos.x += separator_size.x + style.spacing;

        draw_styled_text(style, item.content, pos, item.content_color);

        offset.y -= content_size.y;
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