#ifndef DOOR_STATE_MACHINE_CPP_INCLUDED
#define DOOR_STATE_MACHINE_CPP_INCLUDED

#include "DoorStateMachine.h"
#include "Rooms/Room.h"

void DoorStateGlobal::OnStart()
{
    Scene* current = SceneManager::GetSceneWithName("MainScene");
    
    XMFLOAT3 posDoor = pTransformDoor->transform.GetWorldPosition();
    XMFLOAT3 scaleDoor = pTransformDoor->transform.GetWorldScale();
    XMFLOAT4 rotationDoor = pTransformDoor->transform.GetWorldRotation();
    
    TransformComponent* pTransformOwner = current->GetComponentType<TransformComponent>(m_pOwner);
    XMFLOAT3 newScale = scaleDoor * 2.0f + XMFLOAT3(0.0f, 0.0f, 2.0f);
    pTransformOwner->transform.SetWorldScale(newScale);

    XMFLOAT3 newPos = posDoor + XMFLOAT3(0.0f, 0.0f, -1.0f) * newScale * 0.5f;
    
    pTransformOwner->transform.SetWorldPosition(newPos);
    pTransformOwner->transform.OrbitAroundAxis(XMLoadFloat3(&posDoor), rotationDoor);

    m_pOpenSound = AudioEngine::LoadWav(L"../../res/Audio/doorOpen.wav");
    m_pCloseSound = AudioEngine::LoadWav(L"../../res/Audio/doorClose.wav");

    
    m_pOpenTween = TweenSystem::Create(
            positionStart,
            positionEnd,
            Interpolation::easingOut_cubic
        );
    
    if ( m_pRoom ) m_pRoom->AddTween(m_pOpenTween);
}

void DoorStateGlobal::OnUpdate(float _dt)
{
}

void DoorStateGlobal::OnCollisionEnter(Entity* other)
{
    if (other->name == "Player")
        OpenDoor();
}

void DoorStateGlobal::OnCollisionExit(Entity* other)
{
    if (other->name == "Player")
        CloseDoor();
}

void DoorStateGlobal::OpenDoor()
{
    if (m_isOpen == false)
    {
        XMFLOAT3 pos = pTransformDoor->transform.GetWorldPosition();
        AudioEngine::Play3D(m_pOpenSound, pos.x, pos.y, pos.z, 1.0);
        m_pOpenTween->StartDuration(0.5f, Function::Position, &pTransformDoor->transform, false);
        m_isOpen = true;
    }
}

void DoorStateGlobal::CloseDoor()
{
    if (m_isOpen)
    {
        XMFLOAT3 pos = pTransformDoor->transform.GetWorldPosition();
        AudioEngine::Play3D(m_pCloseSound, pos.x, pos.y, pos.z, 1.0);
        m_pOpenTween->StartDuration(1.5f, Function::Position, &pTransformDoor->transform, true);
        m_isOpen = false;
    }
}

#endif