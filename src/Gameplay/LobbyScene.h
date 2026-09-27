#ifndef LOBBY_SCENE_H_INCLUDED
#define LOBBY_SCENE_H_INCLUDED

#include "pch.h"

#include "Scene.h"

class LobbyScene : public Scene
{
public:

    void Reset();
protected:
    void OnInit() override;
    void OnStart() override;
    void OnUpdate(float _dt) override;

private:
    Entity* m_pCamera       = nullptr;

    Entity* m_pEntityText = nullptr;
    Entity* m_pStartButton = nullptr;

    void Connect(std::string id, sockaddr_in* from);
    void Start();
};


#endif