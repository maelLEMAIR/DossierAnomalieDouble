#ifndef SPLASH_SCREEN_SCENE_H_INCLUDED
#define SPLASH_SCREEN_SCENE_H_INCLUDED

#include "pch.h"
#include "Scene.h"
#include "MainScene.h"

enum TypeScene
{
    MENU,
    CREDITS_MENU,
    OPTIONS_MENU,
    JOIN_MENU,
    CREATE_MENU,
    GAME,
    ANY_SCENE
};

class SplashScreen : public Scene
{
public:
    void Initialization(String _name, WString _backgroundPath);

    TypeScene typeScene = ANY_SCENE;
protected:
    void OnInit() override;
    void OnUpdate(float _dt) override;
    void OnStart() override;

private:
    Scene* m_pNextScene = nullptr;
    String m_NextSceneName = "NextScene";
};

#endif