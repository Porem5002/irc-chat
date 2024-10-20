#include <stdlib.h>

#define MAX_MSG_SIZE 1024

enum 
{
    MSG_KIND_CONNECT,
    MSG_KIND_DISCONNECT,
    MSG_KIND_DATA,
};

typedef struct MsgPacketText MsgPacketText;
struct __attribute__((__packed__)) MsgPacketText
{
    unsigned short kind;
    char text [];
};

typedef union MsgPacket MsgPacket;
union MsgPacket
{
    unsigned short kind;
    MsgPacketText as_text;
};

unsigned short msg_get_kind(const MsgPacket* msg);
void msg_set_kind(MsgPacket* msg, unsigned short kind);

size_t msg_get_size(const MsgPacket* msg);