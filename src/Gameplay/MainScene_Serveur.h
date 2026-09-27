#ifndef MAIN_SCENE_SERVEUR_H_INCLUDED
#define MAIN_SCENE_SERVEUR_H_INCLUDED
#include "MainScene.h"

class MainScene_Serveur : public MainScene
{
private:
	void NetworkStart(UnorderedMap<int, Room*> _pRoom) override;
	void NetworkUpdate(float _dt) override;
	void Restart();

	void ReceiveStateLever(const char* data);
};

#endif