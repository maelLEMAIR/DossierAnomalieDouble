#ifndef FLOATING_OBJECT_H_INCLUDED
#define FLOATING_OBJECT_H_INCLUDED

#include "AnomalyBase.h"

class FloatingObject : public AnomalyBase
{
public:
    FloatingObject(AnomalyConfig cfg, Entity* _owner) : AnomalyBase(cfg, _owner) {}
    ~FloatingObject() override = default;
    
    AnomalyType GetType() const override { return AnomalyType::FloatingObject; }
    void OnInit() override;
    void OnStart() override;
    void OnUpdate(float dt) override;
    void OnStartSeqLoop() override;
    void OnEndSeqLoop() override;
    void OnEnd() override;
    void Reset() override;

private:
    TransformComponent* m_pTransform = nullptr;
    Tween* m_pTween = nullptr;
    XMFLOAT3 m_firstPos = XMFLOAT3(0.0f, 0.0f, 0.0f);
};

#endif