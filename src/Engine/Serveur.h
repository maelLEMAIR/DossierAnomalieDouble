#pragma once
#include "Socket.h"

class Serveur : public Socket
{
public:
	Serveur();
	void InitADDR(PCSTR path, int port) override;
};

