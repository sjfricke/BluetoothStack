#ifndef COMMON_SOCKET_H_
#define COMMON_SOCKET_H_

#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>
#include <sys/socket.h>

#define MAX_MAC_SIZE 18

// Holds info a client needs to handle socket connection
struct ClientSocketInfo {
    sockaddr_rc server;
    socklen_t socket_length = sizeof(sockaddr_rc);
    int socket;
    int connection;
    char mac[MAX_MAC_SIZE];
};

// Holds info a server needs to handle socket connection
struct ServerSocketInfo {
    sockaddr_rc client;
    sockaddr_rc server;
    socklen_t socket_length = sizeof(sockaddr_rc);
    int socket;
    int connection;
    char mac[MAX_MAC_SIZE];
};

#endif  // COMMON_SOCKET_H_