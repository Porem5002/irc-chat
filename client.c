#include <winsock2.h>
#include <stdio.h>

#include "shared.h"

typedef struct ClientConfig ClientConfig;
struct ClientConfig
{
    const char* ip;
    u_short port;
};

ClientConfig client_load_config(int argc, char** argv)
{
    ClientConfig config = {
        .ip = "127.0.0.1",
        .port = 6000,
    };

    for(size_t i = 1; i < argc; i+=2)
    {
        if(strcmp(argv[i], "-ip") == 0)
        {
            if(i + 1 >= argc)
            {
                fprintf(stderr, "ERROR: Invalid number of arguments for flag '-ip'\n");
                exit(EXIT_FAILURE);
            }

            config.ip = argv[i+1];
        }
        else if(strcmp(argv[i], "-port") == 0)
        {
            if(i + 1 >= argc)
            {
                fprintf(stderr, "ERROR: Invalid number of arguments for flag '-port'\n");
                exit(EXIT_FAILURE);
            }

            config.port = (u_short)atoi(argv[i+1]);
        }
        else
        {
            fprintf(stderr, "ERROR: \'%s\' is not a valid flag\n", argv[i]);
            exit(EXIT_FAILURE);
        }
    }

    return config;
}

int network_main(int argc, char** argv, SOCKET skt)
{
    ClientConfig config = client_load_config(argc, argv);

    SOCKADDR_IN address = {0};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(config.ip);
    address.sin_port = htons(config.port);

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