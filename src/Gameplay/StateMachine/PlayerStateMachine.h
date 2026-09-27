#ifndef PLAYER_STATE_MACHINE_H_INCLUDED
#define PLAYER_STATE_MACHINE_H_INCLUDED

#include "pch.h"

class PlayerStateGlobal : public StateGlobal
{
public:
    
    void OnStart() override;
    void OnUpdate(float _dt) override;

    void Reset();
    void AddScore(int _score);
    
    float m_sensitive = 0.1f;
    Entity* m_pCam = nullptr;
    
private:
    void HandleInput(float _dt);
    void HandleMouseMovement(float _dt);

    void Ping();
    bool CanJump();

    RayCast* m_pRayJump = nullptr;
    HitPoint* m_pHit = nullptr;
    float m_jumpCooldown = 0.0f;
    bool isJumping = false;
    
    Sound* m_pFootStepSound = nullptr;
    ActiveVoice* m_pFootstepsAV = nullptr;
    Sound* m_pPingSound = nullptr;
    
    bool m_isReady = false;

    float m_footstepTimer = 0.f;
    
    int m_score = 0;
    TextComponent* m_pTextScore = nullptr;
    
    XMFLOAT2 m_yawPitch = XMFLOAT2(0.0f, 0.0f);
    
    TransformComponent* m_pTransform = nullptr;
    TransformComponent* m_pCamTransform = nullptr;

    Collider* m_pHeadCollider = nullptr;
    ForceComponent* m_pHeadForce = nullptr;
};


#endif
