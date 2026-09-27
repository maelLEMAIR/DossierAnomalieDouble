
#include "Serveur.h"

Serveur::Serveur() : Socket()
{

    mId = 0;
    mCanRecv = true;
}

void Serveur::InitADDR(PCSTR path, int port)
{
    Socket::InitADDR(path, port);

    mAddr.sin_addr.s_addr = ADDR_ANY;

    if (bind(mSock, (sockaddr*)&mAddr, sizeof(mAddr)) == SOCKET_ERROR)
    {
        std::cout << "Fail to bind" << std::endl;
        return;
    }
    else
    {
        std::cout << "Bind done with success" << std::endl;
    }
}
