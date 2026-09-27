#ifndef LOBBY_SCENE_CPP_INCLUDED
#define LOBBY_SCENE_CPP_INCLUDED

#include "GameManager.h"
#include "LobbyScene.h"
#include "MapLoader.h"
#include <Components/UIButtonComponent.h>
#include <Systems/UIButtonSystem.h>

class StartButton : public StateGlobal
{
public:
    void OnButtonUp() override
    { 
        Data data;
        data.Init(Cmd::START);

        EngineManager::GetSocket()->SendSecurTo(data, GameManager::GetOtherPlayerId());
        SceneManager::ChangeCurrentScene("MainScene");
    }

};


void LobbyScene::Reset()
{
    m_pEntityText->isActive = true;
    m_pStartButton->isActive = false;
}

void LobbyScene::OnInit()
{
    m_pEntityText = CreateEntity();
    TransformComponent* pTransform = GetComponentType<TransformComponent>(m_pEntityText);
    pTransform->transform.SetWorldScale(2.0f);
    TextComponent* pText = AddComponent<TextComponent>(m_pEntityText);
    pText->SetText("Waiting for second player...");
    pText->offsetCenter.x = -0.2f;

    m_pStartButton = CreateEntity();
    m_pStartButton->isActive = false;
    TransformComponent* pTransformButton = GetComponentType<TransformComponent>(m_pStartButton);
    UIButtonComponent* pButton = AddComponent<UIButtonComponent>(m_pStartButton);

    Device* pDevice = EngineManager::GetDevice();
    Texture* pNormal = pDevice->CreateTexture(L"../../res/Textures/Button/Start Button/Button-classique.dds");
    Texture* pHover = pDevice->CreateTexture(L"../../res/Textures/Button/Start Button/Button-hovered.dds");

    pButton->textureNormal = pNormal;
    pButton->textureHover = pHover;
    SpriteComponent* pSprite = AddComponent<SpriteComponent>(m_pStartButton);
    pSprite->width = 150;
    pSprite->height = 75;
    pSprite->pRect = SpriteFactory::BuildRectangle(pDevice, pSprite->width, pSprite->height);
    pSprite->SetTexture(pNormal);

    StateMachineComponent* pSM = AddComponent<StateMachineComponent>(m_pStartButton);
    pSM->SetStateGlobal(new StartButton);

    m_isInit = true;
}

void LobbyScene::OnStart()
{
    Reset();

    EngineManager::GetSocket()->LinkCmdFunc(Cmd::CONNECT, [this](const char* data, std::string id, sockaddr_in* from) {this->Connect(id, from); });
    EngineManager::GetSocket()->LinkCmdFunc(Cmd::START, [this](const char* data, std::string id, sockaddr_in* from) {this->Start(); });
}

void LobbyScene::OnUpdate(float _dt)
{

}

void LobbyScene::Connect(std::string id, sockaddr_in* from)
{
    std::cout << "Other player connect " << std::endl;

    EngineManager::GetSocket()->CreateProfils(id, *from);

    GameManager::SetOtherPlayerId(id);

    m_pEntityText->isActive = false;
    m_pStartButton->isActive = true;
}

void LobbyScene::Start()
{
    SceneManager::ChangeCurrentScene("MainScene");
}

#endif
