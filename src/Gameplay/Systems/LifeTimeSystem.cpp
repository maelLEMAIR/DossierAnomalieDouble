#ifndef LIFE_TIME_SYSTEM_CPP_INCLUDED
#define LIFE_TIME_SYSTEM_CPP_INCLUDED

#include "LifeTimeSystem.h"
#include "Components/LifeTimeComponent.h"

void LifeTimeSystem::OnInit()
{
    SetMaskLoadComponents<LifeTimeComponent>();
}

void LifeTimeSystem::Update(float dt)
{
    for (auto [id, vComponents] : m_mComponents)
    {
        LifeTimeComponent* pLTC = reinterpret_cast<LifeTimeComponent*>(vComponents[0]);

        if (pLTC == nullptr)
            continue;
        
        pLTC->m_lifeCooldown -= dt;
        if (pLTC->m_lifeCooldown <= 0)
        {
            Scene* pScene = SceneManager::GetCurrentScene();
            pScene->DestroyEntity(pLTC->GetOwner());
        }
    }
}
#endif