#ifndef MENU_SCENE_H_INCLUDED
#define MENU_SCENE_H_INCLUDED

#include "pch.h"

#include "Scene.h"

class MainScene;

class MenuScene : public Scene
{
public:

    void Reset();
protected:
    void OnInit() override;
    void OnStart() override;
    void OnUpdate(float _dt) override;

private:
    Entity* m_pCamera       = nullptr;
    LightComponent* m_pLight = nullptr;
};


#endif