#ifndef LEVER_STATE_MACHINE_CPP_INCLUDED
#define LEVER_STATE_MACHINE_CPP_INCLUDED

#include "LeverStateMachine.h"
#include "Rooms/RevealRoom.h"
#include "Rooms/Room.h"
#include "MainScene.h"

void LeverStateMachine::OnStart()
{
    m_pLeverAnimation = TweenSystem::Create({0.0f, 0.0f, 0.0f}, {0.0f, -XM_PI, 0.0f}, Interpolation::easingInAndOut_linear);
    m_firstRotation = pTransformLever->transform.GetWorldRotation();
    pMainScene = reinterpret_cast<MainScene*>(SceneManager::GetSceneWithName("MainScene"));
}

void LeverStateMachine::OnCollisionStay(Entity* other)
{
    if (other->name != "Player")
        return;

    pMainScene->m_pInfo->isActive = true;
    if (InputSystem::IsKeyDown(E))
    {
        if (m_isActivate) RaiseTheLever();
        else LowerTheLever();
    }
}

void LeverStateMachine::OnCollisionExit(Entity* other)
{
    if (other->name != "Player")
        return;

    pMainScene->m_pInfo->isActive = false;
}
void LeverStateMachine::Reset()
{
    m_isActivate = false;
    pMeshRendererLeverIndicator->pMaterial = RessourceManager::GetMaterial("Red");
    pTransformLever->transform.SetWorldRotationQuaternion(m_firstRotation);
    pRoom->pRevealRoom->SetPlayerAnswer(m_isActivate);
}

void LeverStateMachine::LowerTheLever()
{
    m_isActivate = true;
    pMeshRendererLeverIndicator->pMaterial = RessourceManager::GetMaterial("Green");
    pRoom->pRevealRoom->SetPlayerAnswer(m_isActivate);
    m_pLeverAnimation->StartDuration(0.5f, Function::Rotation, &pTransformLever->transform, false);
}

void LeverStateMachine::RaiseTheLever()
{
    m_isActivate = false;
    pMeshRendererLeverIndicator->pMaterial = RessourceManager::GetMaterial("Red");
    pRoom->pRevealRoom->SetPlayerAnswer(m_isActivate);
    m_pLeverAnimation->StartDuration(0.5f, Function::Rotation, &pTransformLever->transform, true);
}

#endif
