#ifndef NETWORK_HEADER
#define NETWORK_HEADER

#include <stdint.h>
#include <stdbool.h>

#define NETWORK_MESSAGE_MAX_CONTENT 400

#define NETWORK_IP_ANY ((NetworkIP){0})
#define NETWORK_IP_LOCALHOST ((NetworkIP){ 127, 0, 0, 1 })

enum
{
    NETWORK_MSG_KIND_CONNECT,
    NETWORK_MSG_KIND_TEXT,
    NETWORK_MSG_KIND_DISCONNECT,
};

typedef struct NetworkMessage NetworkMessage;
struct NetworkMessage
{
    uint8_t kind;
    char content [NETWORK_MESSAGE_MAX_CONTENT];
};

typedef struct NetworkIP NetworkIP;
struct NetworkIP
{
    uint8_t bytes [4];
};

typedef uint16_t NetworkPort;

typedef struct NetworkAddress NetworkAddress;
struct NetworkAddress
{
    NetworkIP ip;
    NetworkPort port;
};

void network_start();
void network_finish();

bool network_bind(NetworkAddress address);
bool network_receive(NetworkAddress* source, NetworkMessage* msg);
bool network_send(NetworkAddress target, const NetworkMessage* msg);

bool network_str_to_ip(const char* s, NetworkIP* ip);
void network_ip_to_str(NetworkIP ip, char* s, size_t n);
void network_address_to_str(NetworkAddress address, char* s, size_t n);

bool network_ip_equals(NetworkIP a, NetworkIP b);
bool network_address_equals(NetworkAddress a, NetworkAddress b);

#endif