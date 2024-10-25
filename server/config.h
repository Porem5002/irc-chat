#ifndef SERVER_CONFIG_HEADER
#define SERVER_CONFIG_HEADER

#include "../network/network.h"

typedef struct ServerConfig ServerConfig;
struct ServerConfig
{
    NetworkPort port;
};

ServerConfig server_load_config(int argc, char** argv);

#endif