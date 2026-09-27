#ifndef SOCKETS_CPP_DEFINED
#define SOCKETS_CPP_DEFINED

#include "SOCKETS.h"

#include <cstdlib>

void SOCKETS::Start()
{
    WSADATA data;
    if (WSAStartup(MAKEWORD(2, 2), &data) != 0)
        exit(0);
}

void SOCKETS::Release()
{
    WSACleanup();
}

int SOCKETS::GetError()
{
    return WSAGetLastError();
}

void SOCKETS::CloseSocket(SOCKET socket)
{
    closesocket(socket);
}

#endif