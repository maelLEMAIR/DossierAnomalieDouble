#ifndef GRAVITY_CPP_INCLUDED
#define GRAVITY_CPP_INCLUDED

#include "pch.h"  
#include "Gravity.h"

void Gravity::OnInit()
{
    Scene* pScene = SceneManager::GetSceneWithName("MainScene");
    m_pForceSystem = pScene->GetForceSystem();
}

void Gravity::OnStart()
{
    std::cout << "Gravity::OnStart" << '\n';
    ChangeGravity();
}

void Gravity::OnUpdate(float _dt)
{
    BaseTick(_dt);
    
    if ( !m_inCooldown ) return;
}

void Gravity::OnStartSeqLoop()
{
    std::cout << "Gravity::OnStartSeqLoop" << '\n';
    ChangeGravity();
}

void Gravity::OnEndSeqLoop()
{
    std::cout << "Gravity::OnEndSeqLoop" << '\n';
    ChangeGravity();
}

void Gravity::OnEnd()
{
    AnomalyBase::OnEnd();
    std::cout << "Gravity::OnEnd" << '\n';
    Reset();
}

void Gravity::Reset()
{
    std::cout << "Gravity::Reset" << '\n';
    m_pForceSystem->m_Gravity  = m_firstGravityScale;
}

void Gravity::ChangeGravity()
{
    m_pForceSystem->m_Gravity *= -1;
}

#endif
