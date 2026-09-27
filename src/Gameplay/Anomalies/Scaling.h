#ifndef SCALING_H_INCLUDED
#define SCALING_H_INCLUDED

#include "AnomalyBase.h"

class Scaling : public AnomalyBase
{
public:
    Scaling(AnomalyConfig cfg, Entity* _owner) : AnomalyBase(cfg, _owner) {}
    ~Scaling() override = default;
    
    AnomalyType GetType() const override { return AnomalyType::PlayerScale; }
    void OnInit() override;
    void OnStart() override;
    void OnUpdate(float dt) override;
    void OnStartSeqLoop() override { };
    void OnEndSeqLoop() override;
    void OnEnd() override;
    void Reset() override;

private:
    TransformComponent* m_pTransform = nullptr;
    XMFLOAT3 m_firstScale = { 1.0f, 1.0f, 1.0f};
};

#endif