#ifndef ANOMALY_BASE_H_INCLUDED
#define ANOMALY_BASE_H_INCLUDED

#include "pch.h"

enum class AnomalyType
{
    LookAtThePlayer,
    FloatingObject,
    PlayerScale,
    GravitySwitch,
    FollowPlayer,
};

struct AnomalyConfig {
    float duration      = 5.0f;
    float intensity     = 1.0f;
    bool  loops         = false;

    // Delay before first activation
    float startDelay    = 0.0f;

    // Time between every seq (if loops = true)
    float activeTime    = 5.0f;   // Time of the active seq
    float cooldownTime  = 0.0f;   // Time before the next seq
};

class AnomalyBase
{
public:
    AnomalyBase(AnomalyConfig cfg, Entity* _owner) : m_pOwner(_owner), m_config(cfg) {}
    virtual ~AnomalyBase() {}
    
    virtual AnomalyType GetType() const = 0;
    virtual void OnInit()               = 0;
    virtual void OnStart()              = 0;
    virtual void OnUpdate(float dt)     = 0;
    virtual void OnStartSeqLoop()       = 0;
    virtual void OnEndSeqLoop()         = 0;
    virtual void OnEnd();
    virtual void Reset()                = 0;

    bool IsFinished() const { return m_elapsed < 0.f; }
    
protected:

    bool m_isFinished = false;
    
    bool  m_isActivated     = false;
    float m_elapsed         = 0.0f;   // Global time
    float m_phaseTimer      = 0.0f;   // Time in the current seq++
    bool  m_inCooldown      = false;
    
    void BaseTick(float dt);
    
    Entity* m_pOwner        = nullptr;
    AnomalyConfig m_config;
};

#endif