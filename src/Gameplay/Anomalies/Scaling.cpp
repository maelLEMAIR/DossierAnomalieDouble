#include "pch.h" 
#include "Scaling.h"

void Scaling::OnInit()
{
    Scene* pScene = SceneManager::GetSceneWithName("MainScene");
    m_pTransform  = pScene->GetComponentType<TransformComponent>(m_pOwner);
    m_firstScale  = m_pTransform->transform.GetWorldScale();
}

void Scaling::OnStart()
{
    std::cout << "Scaling::OnStart" << '\n';
    m_firstScale  = m_pTransform->transform.GetWorldScale();
}

void Scaling::OnUpdate(float _dt)
{
    BaseTick(_dt);
    
    if (!m_isActivated) return;
    
    float t = m_elapsed / m_config.duration;
    t = max(0.0f, min(1.0f, t));

    float targetScale = m_config.intensity;
    
    XMFLOAT3 currentScale = {
        m_firstScale.x * (1.0f - t) + (m_firstScale.x * targetScale) * t,
        m_firstScale.y * (1.0f - t) + (m_firstScale.y * targetScale) * t,
        m_firstScale.z * (1.0f - t) + (m_firstScale.z * targetScale) * t
    };

    if ( currentScale.y <= 0.2f)
        return;
    
    m_pTransform->transform.SetWorldScale(currentScale);
}

void Scaling::OnEndSeqLoop()
{
    Reset();
}

void Scaling::OnEnd()
{
    AnomalyBase::OnEnd();
    std::cout << "Scaling::OnEnd" << '\n';
    Reset();
}

void Scaling::Reset()
{
    if (m_pTransform->transform.GetWorldScale() != m_firstScale)
    {
        m_pTransform->transform.SetWorldScale(m_firstScale);
        m_pTransform->transform.SetWorldPosition(m_pTransform->transform.GetWorldPosition() +
            XMFLOAT3(0.0f, m_firstScale.y, 0.0f));
    }
}
