#ifndef SOCKETS_H_DEFINED
#define SOCKETS_H_DEFINED

#include <WinSock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

namespace SOCKETS
{
    void Start();
    void Release();
    int GetError();
    void CloseSocket(SOCKET socket);
}

#endif