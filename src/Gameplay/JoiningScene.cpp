#ifndef JOINING_SCENE_CPP_INCLUDED
#define JOINING_SCENE_CPP_INCLUDED

#include "JoiningScene.h"
#include "MapLoader.h"
#include <Components/UIButtonComponent.h>
#include <Systems/UIButtonSystem.h>
#include "../Engine/components.h"
#include "GameManager.h"
#include "MainScene.h"

class StateButton : public StateGlobal
{
public:
    void OnButtonUp() override
    { 
    }

    void OnButtonPressed() override
    { 
    }

    void OnButtonDown() override
    { 
    }

    void OnHoveredEnter() override
    {
        pTarget->transform.SetWorldPosition({ 0.05f, pTarget->transform.GetWorldPosition().y, pTarget->transform.GetWorldPosition().z });
    }

    void OnHoveredExit() override
    {
        pTarget->transform.SetWorldPosition({ 0.0f, pTarget->transform.GetWorldPosition().y, pTarget->transform.GetWorldPosition().z });
    }

    TransformComponent* pTarget;
};


void JoiningScene::Reset()
{
}

class InputTextState : public StateGlobal
{
public:
    void OnButtonPressed() override
    {
        if (m_pOwner != nullptr && m_pOwner->pScene != nullptr)
        {
            UITextInputComponent* pInput = m_pOwner->pScene->GetComponentType<UITextInputComponent>(m_pOwner);
            /*extComponent* pText = m_pOwner->pScene->GetComponentType<TextComponent>(m_pOwner);

            if (pInput != nullptr && pText != nullptr)
            {
                std::cout << "Validation ! Le texte recupere est : " << pInput->currentText << std::endl;
                pText->SetColor(0.0f, 1.0f, 0.0f);
            }*/

            EngineManager::InitClient(pInput->currentText.c_str(), 1888);
            Socket* pSock = EngineManager::GetSocket();
            pSock->CreateProfils((char*)SERVEUR, pSock->mAddr);

            Data data;
            data.Init(Cmd::CONNECT);

            pSock->SendSecurTo(data, (char*)SERVEUR);
            pSock->StartReading();

            GameManager::SetOtherPlayerId((char*)SERVEUR);

            MainScene* p_mainScene = reinterpret_cast<MainScene*>(SceneManager::GetSceneWithName("MainScene"));
            p_mainScene->isClient = true;

            SceneManager::ChangeCurrentScene("Lobby Scene");
        }
    }
};

void JoiningScene::OnInit()
{
    Device* pDevice = EngineManager::GetDevice();

    Entity* pEntityText = CreateEntity();
    TransformComponent* pTransform = GetComponentType<TransformComponent>(pEntityText);
    pTransform->transform.SetWorldScale(2.0f);
    UITextInputComponent* pTextInput = AddComponent<UITextInputComponent>(pEntityText);
    pTextInput->isFocused = true;
    pTextInput->type = UITextInputType::STATE;
    TextComponent* pText = AddComponent<TextComponent>(pEntityText);
    pText->SetText("Ecrire l'adresse du serveur");
    pText->SetColor(1.0f, 1.0f, 1.0f);
    SpriteComponent* pSprite = AddComponent<SpriteComponent>(pEntityText);
    pSprite->width = 400;
    pSprite->height = 50;
    pSprite->pRect = SpriteFactory::BuildRectangle(pDevice, pSprite->width, pSprite->height);
    InputTextState* stateGlobal = new InputTextState();
    StateMachineComponent* pSm = AddComponent<StateMachineComponent>(pEntityText);
    pSm->SetStateGlobal(stateGlobal);

    std::cout << "Start joining scene" << std::endl;
}

void JoiningScene::OnStart()
{
}

void JoiningScene::OnUpdate(float _dt)
{
}

#endif
