#ifndef MAIN_SCENE_CLIENT_H_INCLUDED
#define MAIN_SCENE_CLIENT_H_INCLUDED
#include "MainScene.h"

class MainScene_Client : public MainScene
{
public:
	bool m_canLoadNextRoom = false;
private:
	void NetworkStart(UnorderedMap<int, Room*> _pRoom) override;
	void NetworkUpdate(float _dt) override;

	void StartRoom(const char* _data);
	void NextRoom(const char* _data);
	void StartRevealeRoom(const char* _data);
	void Reset(const char* _data);
	bool m_receiveNextRoom = false;
	int m_idNextRoom = 0;
	bool m_hasAnomaly = false;
};

#endif