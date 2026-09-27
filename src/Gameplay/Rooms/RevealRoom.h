#ifndef REVEAL_ROOM_H_INCLUDED
#define REVEAL_ROOM_H_INCLUDED

#include "pch.h"
#include "Room.h"

class MapLoader;

class RevealRoom : public Room
{
public:
    void OnInit(Device* _pDevice, Scene& _scene) override;
    void OnUpdate() override { } ;
    void ActiveRoom(XMFLOAT3 newCenter) override;
    void DisabledRoom(float _disableEntity = true) override;
    void SetPlayerAnswer(bool _answer)      { m_pTrapStateMachine->SetAnswer( _answer ); }
    void ChangeColorLight(bool _toGreen);
    void ResetColorLight();
    
private:
    TrapStateMachine* m_pTrapStateMachine = nullptr;
};

#endif