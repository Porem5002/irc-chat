#include <winsock2.h>
#include <stdio.h>

#include "shared.h"

typedef struct ServerConfig ServerConfig;
struct ServerConfig
{
    u_short port;
};

ServerConfig server_load_config(int argc, char** argv)
{
    ServerConfig config = {
        .port = 6000,
    };

    for(size_t i = 1; i < argc; i+=2)
    {
        if(strcmp(argv[i], "-port") == 0)
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
    ServerConfig config = server_load_config(argc, argv);

    SOCKADDR_IN address = {0};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(config.port);

    if(bind(skt, (SOCKADDR*)&address, sizeof(address)))
    {
        fprintf(stderr, "ERROR: Could not bind server to address\n");
        return 1;
    }

    char buffer [MAX_MSG_SIZE];
    MsgPacket* msg = (MsgPacket*)buffer;

    size_t client_count = 0;
    SOCKADDR_IN* clients = NULL;

    while(1)
    {
        SOCKADDR_IN source;
        int source_len = sizeof(source);
        int msg_size = recvfrom(skt, (char*)buffer, sizeof(buffer), 0, (SOCKADDR*)&source, &source_len);

        if(msg_size != SOCKET_ERROR && msg_size >= sizeof(u_short))
        {
            switch(msg_get_kind(msg))
            {
                case MSG_KIND_CONNECT:
                    // Prevent multiple connections from the same client
                    clients = realloc(clients, sizeof(*clients) * (client_count + 1));
                    clients[client_count++] = source;
                    break;
                case MSG_KIND_DISCONNECT:
                    // Remove clients
                    break;
                case MSG_KIND_DATA:
                    for(int i = 0; i < client_count; i++)
                    {
                        SOCKADDR_IN c = clients[i];

                        if(c.sin_addr.s_addr != source.sin_addr.s_addr || c.sin_port != source.sin_port)
                            sendto(skt, (char*)msg, msg_get_size(msg), 0, (SOCKADDR*)&c, sizeof(c));
                    }
                    break;
            }
        }
    }

    return 0;
}