#ifndef SPLASH_SCREEN_SCENE_CPP_INCLUDED
#define SPLASH_SCREEN_SCENE_CPP_INCLUDED

#include "SplashScreenScene.h"

#include "LobbyScene.h"
#include "MenuCreditScene.h"
#include "MenuScene.h"
#include "Render/Generic/Factories/SpriteFactory.hpp"

void SplashScreen::OnInit()
{    
}

void SplashScreen::Initialization(String _name, WString _backgroundPath)
{
    Device* pDevice = EngineManager::GetDevice();

    Texture* splashText = pDevice->CreateTexture(_backgroundPath);

    Sprite* rect = SpriteFactory::BuildRectangle(pDevice, EngineManager::GetWindow()->GetWidth(),
        EngineManager::GetWindow()->GetHeight());

    Entity* splashEntity = CreateEntity();
    SpriteComponent* sprite = AddComponent<SpriteComponent>(splashEntity);
    sprite->pRect = rect;
    sprite->SetTexture(splashText);

    m_NextSceneName = _name;
    
    m_isInit = true;
}

void SplashScreen::OnUpdate(float _dt)
{
    if (m_pNextScene == nullptr)
    {
        switch (typeScene)
        {
        case MENU:
            m_pNextScene = SceneManager::CreateSceneType<MenuScene>(m_NextSceneName);
            break;
        case CREDITS_MENU:
            m_pNextScene = SceneManager::CreateSceneType<MenuCreditScene>(m_NextSceneName);
            break;
        case OPTIONS_MENU:
            //m_pNextScene = SceneManager::CreateSceneType<MenuOptionsScene>(m_NextSceneName);
            break;
        case JOIN_MENU:
            m_pNextScene = SceneManager::CreateSceneType<LobbyScene>(m_NextSceneName);
            break;
        case CREATE_MENU:
            m_pNextScene = SceneManager::CreateSceneType<LobbyScene>(m_NextSceneName);
            break;
        case GAME:
            m_pNextScene = SceneManager::CreateSceneType<MainScene>(m_NextSceneName);
            break;
        }
    }

    if (m_pNextScene->IsInit())
        SceneManager::ChangeCurrentScene(m_NextSceneName);
}

void SplashScreen::OnStart()
{
    
}

#endif
