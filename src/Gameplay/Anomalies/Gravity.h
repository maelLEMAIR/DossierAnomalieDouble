#ifndef GRAVITY_H_INCLUDED
#define GRAVITY_H_INCLUDED

#include "AnomalyBase.h"

class Gravity : public AnomalyBase
{
public:
    Gravity(AnomalyConfig cfg, Entity* _owner) : AnomalyBase(cfg, _owner) {}
    ~Gravity() override = default;
    
    AnomalyType GetType() const override { return AnomalyType::GravitySwitch; }
    void OnInit() override;
    void OnStart() override;
    void OnUpdate(float dt) override;
    void OnStartSeqLoop() override;
    void OnEndSeqLoop() override;
    void OnEnd() override;
    void Reset() override;

private:
    void ChangeGravity();

    ForceSystem* m_pForceSystem = nullptr;
    float m_firstGravityScale = -9.81f;
};

#endif