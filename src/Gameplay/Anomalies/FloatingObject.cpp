#ifndef FLOATING_OBJECT_CPP_INCLUDED
#define FLOATING_OBJECT_CPP_INCLUDED

#include "pch.h"
#include "FloatingObject.h"

void FloatingObject::OnInit()
{
    Scene* pScene = SceneManager::GetSceneWithName("MainScene");

    m_pTransform = pScene->GetComponentType<TransformComponent>(m_pOwner);
    
    m_pTween = TweenSystem::Create(
    m_pTransform->transform.GetWorldPosition(),
    m_pTransform->transform.GetWorldPosition() + XMFLOAT3(0.0f, m_config.intensity, 0.0f),
        Interpolation::easingInAndOut_linear
    );
    m_pTween->StartLoop(m_config.activeTime / 2, Function::Position, &m_pTransform->transform);
    m_pTween->StopLoop();
}

void FloatingObject::OnStart()
{
    m_pTween->ResumeLoop();
    m_firstPos = m_pTransform->transform.GetWorldPosition();
    m_pTransform->transform.SetWorldPosition(m_firstPos);
    m_pTween->SetStart(m_firstPos);
    m_pTween->SetEnd(m_firstPos + XMFLOAT3(0.0f, m_config.intensity, 0.0f));
    std::cout << "FloatingObject::OnStart" << '\n';
}

void FloatingObject::OnUpdate(float _dt)
{
    BaseTick(_dt);
}

void FloatingObject::OnEnd()
{
    AnomalyBase::OnEnd();
    std::cout << "FloatingObject::OnEnd" << '\n';
    m_pTween->StopLoop();
    m_pTween->Restart();
    Reset();
}

void FloatingObject::OnStartSeqLoop()
{
    std::cout << "FloatingObject::OnStartSeqLoop" << '\n';
    m_pTween->ResumeLoop();
}

void FloatingObject::OnEndSeqLoop()
{
    std::cout << "FloatingObject::OnEndSeqLoop" << '\n';
    m_pTween->StopLoop();
    m_pTween->Restart();
    m_pTransform->transform.SetWorldPosition(m_firstPos);
}

void FloatingObject::Reset()
{
    std::cout << "FloatingObject::Reset" << '\n';
    m_pTransform->transform.SetWorldPosition(m_firstPos);
}

#endif
