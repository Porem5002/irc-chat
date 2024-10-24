#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "..\network\network.h"
#include "ui.h"

#define BACKGROUND_COLOR ((Color){50, 50, 50, 255})
#define TEXT_COLOR ((Color){ 230, 230, 230, 255 })

#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 600

#define INPUT_CAP 50

int main()
{
    network_start();
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Client");

    TextStyle style = {
        .font = GetFontDefault(),
        .font_size = 30.0f,
        .spacing = 2.0f
    };

    Vector2 input_pos = { 0, SCREEN_HEIGHT - 20 };
    char input_buffer [INPUT_CAP] = {0};
    TextInput input = {
        .char_cap = INPUT_CAP,
        .char_count = 0,
        .text = input_buffer,
    };

    ChatView chat = { 0, NULL };

    NetworkAddress target_address = {
        .ip = { 127, 0, 0, 1 },
        .port = 6000,
    };

    NetworkAddress source_address;
    NetworkMessage msg = {0};

    msg.kind = NETWORK_MSG_KIND_CONNECT;
    network_send(target_address, &msg);

    while(!WindowShouldClose())
    {
        char c;
        while((c = GetCharPressed()) != 0)
            text_input_push_char(&input, c);

        if((IsKeyPressed(KEY_BACKSPACE) || IsKeyPressedRepeat(KEY_BACKSPACE)))
            text_input_pop_char(&input);

        while(network_receive(&source_address, &msg))
        {
            if(msg.kind == NETWORK_MSG_KIND_TEXT)
                chat_view_push(&chat, msg.content);
        }

        if(IsKeyPressed(KEY_ENTER) && !text_input_is_empty(&input))
        {
            msg.kind = NETWORK_MSG_KIND_TEXT;
            strcpy(msg.content, input.text);
            network_send(target_address, &msg);

            chat_view_push(&chat, input.text);
            text_input_clear(&input);
        }
        
        BeginDrawing();
            ClearBackground(BACKGROUND_COLOR);

            Vector2 input_size = measure_styled_text(style, input.text);
            
            // draw chat above text input
            Vector2 chat_pos = input_pos;
            chat_pos.y -= input_size.y;
            chat_view_draw(&chat, style, chat_pos, TEXT_COLOR);

            Vector2 aligned_input_pos = { input_pos.x, input_pos.y - input_size.y/2 };
            draw_styled_text(style, input.text, aligned_input_pos, TEXT_COLOR);
        EndDrawing();
    }

    msg.kind = NETWORK_MSG_KIND_DISCONNECT;
    network_send(target_address, &msg);

    CloseWindow();
    network_finish();
    return 0;
}