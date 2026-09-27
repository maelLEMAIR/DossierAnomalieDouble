#ifndef FOLLOW_PLAYER_H_INCLUDED
#define FOLLOW_PLAYER_H_INCLUDED

#include "AnomalyBase.h"

class MainScene;

class FollowPlayer : public AnomalyBase
{
public:
    FollowPlayer(AnomalyConfig cfg, Entity* _owner) : AnomalyBase(cfg, _owner) {}
    ~FollowPlayer() override = default;
    
    AnomalyType GetType() const override { return AnomalyType::FollowPlayer; }
    void OnInit() override;
    void OnStart() override;
    void OnUpdate(float dt) override;
    void OnStartSeqLoop() override { };
    void OnEndSeqLoop() override;
    void OnEnd() override;
    void Reset() override;

private:
    TransformComponent* m_pTransform = nullptr;
    TransformComponent* m_pPlayerTransform = nullptr;
    XMFLOAT3 m_firstPos = {0.0f, 0.0f, 0.0f};
};

#endif