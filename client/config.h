#ifndef CLIENT_CONFIG_HEADER
#define CLIENT_CONFIG_HEADER

#include "..\network\network.h"
#include "user_color.h"

typedef struct ClientConfig ClientConfig;
struct ClientConfig
{
    const char* name;
    UserColorId color;
    NetworkIP ip;
    NetworkPort port;
};

ClientConfig client_load_config(int argc, char** argv);

#endif