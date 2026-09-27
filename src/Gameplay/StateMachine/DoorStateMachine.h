#ifndef DOOR_STATE_MACHINE_H_INCLUDED
#define DOOR_STATE_MACHINE_H_INCLUDED

#include "pch.h"

class Room;

class DoorStateGlobal : public StateGlobal
{
public:
    void OnStart() override;
    void OnUpdate(float _dt) override;
    void OnCollisionEnter(Entity* other) override;
    void OnCollisionExit(Entity* other) override;

    Room* m_pRoom;
    TransformComponent* pTransformDoor;
    XMFLOAT3 positionStart = { 0.0f, 0.0f, 0.0f};
    XMFLOAT3 positionEnd = { 0.0f, 3.0f, 0.0f};
private:
    void OpenDoor();
    void CloseDoor();

    Sound* m_pOpenSound = nullptr;
    Sound* m_pCloseSound = nullptr;
    
    Tween* m_pOpenTween = nullptr;
    bool m_isOpen = false;
};

#endif