cmake_minimum_required(VERSION 3.21)

project(irc-chat)

add_executable(irc-chat-client "client/main.c" "client/config.c" "client/ui.c" "client/user_color.c" "network/network.c")
target_include_directories(irc-chat-client PRIVATE "vendor/raylib/include")
target_link_directories(irc-chat-client PRIVATE "vendor/raylib/lib")
target_link_libraries(irc-chat-client PRIVATE Ws2_32.lib raylibdll.lib)

add_executable(irc-chat-server "server/main.c" "server/config.c" "network/network.c")
target_link_libraries(irc-chat-server PRIVATE Ws2_32.lib)