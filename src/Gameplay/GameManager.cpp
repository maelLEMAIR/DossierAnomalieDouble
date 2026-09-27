#ifndef GAME_MANAGER_CPP_DEFINED
#define GAME_MANAGER_CPP_DEFINED

#include "GameManager.h"

#include "MainScene.h"
#include "MenuCreditScene.h"
#include "MenuScene.h"
#include "OptionsMenuScene.h"

GameManager::GameManager()
{
    s_pInstance = this;
}

void GameManager::Run()
{
    EngineManager engineManager;
    engineManager.Initialize(1920 / 2, 1080 / 2, L"DON'T BLINK TO MUCH", true);

    SceneManager::CreateSceneType<MenuScene>("MenuScene");
    SceneManager::CreateSceneType<MenuCreditScene>("MenuCreditScene");
    SceneManager::CreateSceneType<OptionsMenuScene>("OptionsMenuScene");
    /*SceneManager::CreateSceneType<MainScene>("MainScene");*/
    SceneManager::ChangeCurrentScene("MenuScene");

    engineManager.Run();
}

#endif