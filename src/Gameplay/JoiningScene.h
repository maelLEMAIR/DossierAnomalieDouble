#ifndef JOINING_SCENE_H_INCLUDED
#define JOINING_SCENE_H_INCLUDED

#include "pch.h"

#include "Scene.h"

class JoiningScene : public Scene
{
public:

    void Reset();
protected:
    void OnInit() override;
    void OnStart() override;
    void OnUpdate(float _dt) override;

private:
    Entity* m_pCamera       = nullptr;
};


#endif