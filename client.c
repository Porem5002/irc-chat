#include <winsock2.h>
#include <stdio.h>

#include "shared.h"

int network_main(int argc, char** argv, SOCKET skt)
{
    SOCKADDR_IN address = {0};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr("127.0.0.1");
    address.sin_port = htons(6000);

    char buffer [MAX_MSG_SIZE];
    MsgPacket* msg = (MsgPacket*)buffer;
    
    u_long io_mode = 1;
    ioctlsocket(skt, FIONBIO, &io_mode);

    msg_set_kind(msg, MSG_KIND_CONNECT);
    sendto(skt, (char*)msg, msg_get_size(msg), 0, (SOCKADDR*)&address, sizeof(address));

    while(1)
    {
        char cmd [100];
        printf("you: ");
        scanf(" %99[^\n]", cmd);

        if(strcmp(cmd, "!") == 0) break;
        
        msg_set_kind(msg, MSG_KIND_DATA);
        strcpy(msg->as_text.text, cmd);
        sendto(skt, (char*)msg, msg_get_size(msg), 0, (SOCKADDR*)&address, sizeof(address));

        while(recvfrom(skt, (char*)msg, sizeof(buffer), 0, NULL, NULL) != SOCKET_ERROR)
        {
            if(msg_get_kind(msg) == MSG_KIND_DATA)
                printf("someone: %s\n", msg->as_text.text);
        }
    }

    msg_set_kind(msg, MSG_KIND_DISCONNECT);
    sendto(skt, (char*)msg, msg_get_size(msg), 0, (SOCKADDR*)&address, sizeof(address));

    return 0;
}