#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "network/network.h"

typedef struct ServerConfig ServerConfig;
struct ServerConfig
{
    NetworkPort port;
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

            config.port = (NetworkPort)atoi(argv[i+1]);
        }
        else
        {
            fprintf(stderr, "ERROR: \'%s\' is not a valid flag\n", argv[i]);
            exit(EXIT_FAILURE);
        }
    }

    return config;
}

int main(int argc, char** argv)
{
    ServerConfig config = server_load_config(argc, argv);

    network_start();

    NetworkAddress address = {
        .ip = NETWORK_IP_ANY,
        .port = config.port,
    };

    if(!network_bind(address))
    {
        fprintf(stderr, "ERROR: Could not bind server to address\n");
        return 1;
    }

    NetworkMessage msg = {0};

    size_t client_count = 0;
    NetworkAddress* clients = NULL;

    char pbuffer [100];

    while(1)
    {
        NetworkAddress source;

        if(network_receive(&source, &msg))
        {
            switch(msg.kind)
            {
                case NETWORK_MSG_KIND_CONNECT:
                    // Prevent multiple connections from the same client
                    network_address_to_str(source, pbuffer, sizeof(pbuffer));
                    printf("%s connected\n", pbuffer);

                    clients = realloc(clients, sizeof(*clients) * (client_count + 1));
                    clients[client_count++] = source;
                    break;
                case NETWORK_MSG_KIND_DISCONNECT:
                    // Remove clients
                    network_address_to_str(source, pbuffer, sizeof(pbuffer));
                    printf("%s disconnected\n", pbuffer);
                    break;
                case NETWORK_MSG_KIND_TEXT:
                    for(int i = 0; i < client_count; i++)
                    {
                        NetworkAddress c = clients[i];
                        if(!network_address_equals(c, source))
                            network_send(c, &msg);
                    }
                    break;
            }
        }
    }

    network_finish();
    return 0;
}