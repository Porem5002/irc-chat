#include <stdio.h>
#include <stdlib.h>

#include <winsock2.h>

#include "network.h"

typedef struct NetworkContext NetworkContext;
struct NetworkContext
{
    SOCKET sock;
};

NetworkContext net;

void network_start()
{
    WSADATA wsaData;
    if(WSAStartup(MAKEWORD(2, 2), &wsaData))
    {
        fprintf(stderr, "ERROR: Could not initialize winsock!\n");
        exit(EXIT_FAILURE);
    }

    net.sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if(net.sock == INVALID_SOCKET)
    {
        fprintf(stderr, "ERROR: Could not create socket!\n");
        exit(EXIT_FAILURE);
    }

    u_long io_mode = 1;
    if(ioctlsocket(net.sock, FIONBIO, &io_mode) == SOCKET_ERROR)
    {
        fprintf(stderr, "ERROR: Could not enable non blocking IO!\n");
        exit(EXIT_FAILURE);
    }
}

void network_finish()
{
    closesocket(net.sock);
    WSACleanup();
}

bool network_bind(NetworkAddress address)
{
    SOCKADDR_IN sock_address = {0};
    sock_address.sin_family = AF_INET;
    memcpy(&sock_address.sin_addr.s_addr, &address.ip, sizeof(address.ip));
    sock_address.sin_port = htons(address.port);

    return bind(net.sock, (SOCKADDR*)&sock_address, sizeof(sock_address)) != SOCKET_ERROR;
}

bool network_receive(NetworkAddress* source, NetworkMessage* msg)
{
    SOCKADDR_IN address = {0};
    int address_len = sizeof(address);

    int msg_size = recvfrom(net.sock, (char*)msg, sizeof(*msg), 0, (SOCKADDR*)&address, &address_len);
    if(msg_size == SOCKET_ERROR)
    {
        *source = (NetworkAddress){0};
        *msg = (NetworkMessage){0};
        return false;
    }

    memcpy(&source->ip, &address.sin_addr.s_addr, sizeof(source->ip));
    source->port = ntohs(address.sin_port);
    return true;
}

bool network_send(NetworkAddress target, const NetworkMessage* msg)
{
    SOCKADDR_IN address = {0};
    address.sin_family = AF_INET;
    memcpy(&address.sin_addr.s_addr, &target.ip, sizeof(target.ip));
    address.sin_port = htons(target.port);

    return sendto(net.sock, (char*)msg, sizeof(*msg), 0, (SOCKADDR*)&address, sizeof(address)) != SOCKET_ERROR;
}

void network_ip_to_str(NetworkIP ip, char* s, size_t n)
{
    if(n == 0) return;

    snprintf(s, n, "%u.%u.%u.%u", ip.bytes[0], ip.bytes[1], ip.bytes[2], ip.bytes[3]);
    s[n-1] = '\0';
}

void network_address_to_str(NetworkAddress address, char* s, size_t n)
{
    if(n == 0) return;
    
    NetworkIP ip = address.ip;
    NetworkPort port = address.port;

    snprintf(s, n, "%u.%u.%u.%u:%u", ip.bytes[0], ip.bytes[1], ip.bytes[2], ip.bytes[3], port);
    s[n-1] = '\0';
}

bool network_ip_equals(NetworkIP a, NetworkIP b)
{
    return memcmp(a.bytes, b.bytes, sizeof(a)) == 0;
}

bool network_address_equals(NetworkAddress a, NetworkAddress b)
{
    return a.port == b.port && network_ip_equals(a.ip, b.ip);
}
