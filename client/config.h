#ifndef CLIENT_CONFIG_HEADER
#define CLIENT_CONFIG_HEADER

#include "..\network\network.h"

typedef struct ClientConfig ClientConfig;
struct ClientConfig
{
    const char* name;
    NetworkIP ip;
    NetworkPort port;
};

ClientConfig client_load_config(int argc, char** argv);

#endif