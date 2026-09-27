#ifndef FOLLOW_PLAYER_CPP_INCLUDED
#define FOLLOW_PLAYER_CPP_INCLUDED

#include "pch.h"
#include "MainScene.h"
#include "FollowPlayer.h"

void FollowPlayer::OnInit()
{
    Scene* pScene = SceneManager::GetSceneWithName("MainScene");
    MainScene* pMainScene = dynamic_cast<MainScene*>(pScene);
    
    m_pPlayerTransform = pScene->GetComponentType<TransformComponent>(pMainScene->m_pPlayer);
    m_pTransform = pScene->GetComponentType<TransformComponent>(m_pOwner);
    
}

void FollowPlayer::OnStart()
{
    std::cout << "FollowPlayer::OnStart" << '\n';
    m_firstPos = m_pTransform->transform.GetWorldPosition();
}

void FollowPlayer::OnUpdate(float _dt)
{
    BaseTick(_dt);

    if (!m_isActivated) return;
    
    if ( m_inCooldown ) return;
    
    XMFLOAT3 playerPos = m_pPlayerTransform->transform.GetWorldPosition();

    m_pTransform->transform.SetWorldPosition({m_firstPos.x, m_firstPos.y, playerPos.z});
}

void FollowPlayer::OnEndSeqLoop()
{
    std::cout << "FollowPlayer::OnEndSeqLoop" << '\n';
}

void FollowPlayer::OnEnd()
{
    AnomalyBase::OnEnd();
    std::cout << "FollowPlayer::OnEnd" << '\n';
    Reset();
}

void FollowPlayer::Reset()
{
    std::cout << "FollowPlayer::Reset" << '\n';
    m_pTransform->transform.SetWorldPosition(m_firstPos);
}


#endif
