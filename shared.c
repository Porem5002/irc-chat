#include <winsock2.h>
#include <stdio.h>

#include "shared.h"

int network_main(int argc, char** argv, SOCKET skt);

int main(int argc, char** argv)
{
    WSADATA wsadata;
    if(WSAStartup(MAKEWORD(2, 2), &wsadata) != 0)
    {
        fprintf(stderr, "ERROR: Could not initialize Winsock\n");
        return 1;
    }

    SOCKET skt = socket(AF_INET, SOCK_DGRAM, 0);
    if(skt == INVALID_SOCKET)
    {
        fprintf(stderr, "ERROR: Could not create socket\n");
        return 1;
    }

    int exit_code = network_main(argc, argv, skt);

    closesocket(skt);
    WSACleanup();
    return exit_code;
}

unsigned short msg_get_kind(const MsgPacket* msg)
{
    return ntohs(msg->kind);
}

void msg_set_kind(MsgPacket* msg, unsigned short kind)
{
    msg->kind = htons(kind);
}

size_t msg_get_size(const MsgPacket* msg)
{
    switch(ntohs(msg->kind))
    {
        case MSG_KIND_DATA:
            return sizeof(msg->kind) + strlen(msg->as_text.text) + 1;
        default:
            return sizeof(msg->kind);
    }
}