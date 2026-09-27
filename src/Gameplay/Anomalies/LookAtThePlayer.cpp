#ifndef LOOK_AT_THE_PLAYER_CPP_INCLUDED
#define LOOK_AT_THE_PLAYER_CPP_INCLUDED

#include "pch.h"  
#include "LookAtThePlayer.h"
#include "MainScene.h"

void LookAtThePlayer::OnInit()
{
    Scene* pScene = SceneManager::GetSceneWithName("MainScene");
    MainScene* pMainScene = dynamic_cast<MainScene*>(pScene);
    
    m_pPlayerTransform = pScene->GetComponentType<TransformComponent>(pMainScene->m_pPlayer);
    m_pTransform = pScene->GetComponentType<TransformComponent>(m_pOwner);
    m_firstRotation = m_pTransform->transform.GetWorldRotation();
    
}

void LookAtThePlayer::OnStart()
{
    std::cout << "LookAtThePlayer::OnStart" << '\n';
    m_firstRotation = m_pTransform->transform.GetWorldRotation();
}

void LookAtThePlayer::OnUpdate(float _dt)
{
    BaseTick(_dt);

    if ( m_inCooldown )
        return;
    
    XMFLOAT3 playerPos = m_pPlayerTransform->transform.GetWorldPosition();
    XMFLOAT3 objPos    = m_pTransform->transform.GetWorldPosition();

    float dx = playerPos.x - objPos.x;
    float dz = playerPos.z - objPos.z;
    float yawToPlayer = atan2f(dx, dz);
    float yaw = yawToPlayer - XM_PIDIV2;

    m_pTransform->transform.SetLocalRotation({yaw, 0.0f, 0.f});
}

void LookAtThePlayer::OnEnd()
{
    AnomalyBase::OnEnd();
    std::cout << "LookAtThePlayer::OnEnd" << '\n';
    Reset();
}

void LookAtThePlayer::Reset()
{
    std::cout << "LookAtThePlayer::Reset" << '\n';
    m_pTransform->transform.SetWorldRotationQuaternion(m_firstRotation);
}

#endif
