#ifndef LOOK_AT_THE_PLAYER_H_INCLUDED
#define LOOK_AT_THE_PLAYER_H_INCLUDED

#include "AnomalyBase.h"

class LookAtThePlayer : public AnomalyBase
{
public:
    LookAtThePlayer(AnomalyConfig cfg, Entity* _owner) : AnomalyBase(cfg, _owner) {}
    ~LookAtThePlayer() override = default;

    AnomalyType GetType() const override { return AnomalyType::LookAtThePlayer; }
    void OnInit() override;
    void OnStart() override;
    void OnUpdate(float dt) override;
    void OnStartSeqLoop() override { };
    void OnEndSeqLoop() override { };
    void OnEnd() override;
    void Reset() override;

private:
    XMFLOAT4 m_firstRotation = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
    TransformComponent* m_pTransform = nullptr;
    TransformComponent* m_pPlayerTransform = nullptr;
};

#endif