#ifndef LEVER_STATE_MACHINE_H_INCLUDED
#define LEVER_STATE_MACHINE_H_INCLUDED

#include "pch.h"

class MainScene;
class RevealRoom;

class LeverStateMachine : public StateGlobal
{
public:
    void OnStart() override;
    void OnCollisionStay(Entity* other) override;
    void OnCollisionExit(Entity* other) override;
    void RaiseTheLever();
    void Reset();

    TransformComponent* pTransformLever = nullptr;
    LightComponent* pLightLever = nullptr;
    MeshRenderer* pMeshRendererLeverIndicator = nullptr;

    Room* pRoom = nullptr;
private:
    void LowerTheLever();

    MainScene* pMainScene = nullptr;
    XMFLOAT4 m_firstRotation = { 0.0f, 0.0f, 0.0f, 1.0f };
    bool m_isActivate = false;
    Tween* m_pLeverAnimation = nullptr;
};
#endif
