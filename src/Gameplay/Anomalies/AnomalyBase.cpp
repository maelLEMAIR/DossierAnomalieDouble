#ifndef ANOMALY_BASE_CPP_INCLUDED
#define ANOMALY_BASE_CPP_INCLUDED

#include "AnomalyBase.h"

void AnomalyBase::OnEnd()
{
    m_elapsed       = 0.0f;
    m_phaseTimer    = 0.0f;
    m_isActivated   = false;
    m_phaseTimer    = 0.0f;
    m_inCooldown    = false;
    m_isFinished    = false;
}

void AnomalyBase::BaseTick(float dt)
{
    if ( m_isFinished )
        return;

    if ( !m_isActivated )
    {
        if ( m_elapsed > m_config.startDelay )
        {
            m_elapsed       = 0.0f;
            m_phaseTimer    = 0.0f;
            m_inCooldown    = false;
            m_isActivated   = true;
            OnStart();
            return;
        }
        m_elapsed += dt;
        return;
    }
    
    if ( !m_inCooldown )
    {
        if ( m_phaseTimer > m_config.activeTime )
        {
            m_inCooldown = true;
            m_phaseTimer = 0.0f;
            if ( m_config.loops == false )
            {
                m_isFinished = true;
                OnEnd();
                return;
            }
            OnEndSeqLoop();
            return;
        }
        m_phaseTimer += dt;
    }
    else
    {
        if ( m_phaseTimer > m_config.cooldownTime )
        {
            m_phaseTimer = 0.0f;
            m_inCooldown = false;
            OnStartSeqLoop();
            return;
        }
        m_phaseTimer += dt;
    }
}

#endif
