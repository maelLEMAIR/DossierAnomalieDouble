#ifndef MAIN_SCENE_H_INCLUDED
#define MAIN_SCENE_H_INCLUDED

#include "pch.h"

#include "Scene.h"

class Hall;
class Bedroom;
class Corridor;
class RevealRoom;
class DiningRoom;
class RoomManager;

class TrapStateMachine;

struct InGameData
{
    bool receiveStateLever = false;
    bool stateLever = false;

    bool ownStateLever = false;

    Vector<TrapStateMachine*> vTrapeSM;

    bool hasAnomaly = false;
        
    bool inRevealeRoom = false;
};

class MainScene : public Scene
{
public:
    void Reset();
    void Restart();
    void EnterNewRoom();

    Entity* m_pPlayer = nullptr;
    bool isClient = false;

    Entity* m_pInfo = nullptr;
    
    InGameData m_inGameData;

protected:
    void OnInit() override;
    void OnStart() override;
    UnorderedMap<int, Room*> GenerateRooms();
    virtual void NetworkStart(UnorderedMap<int, Room*> _pRoom) {};
    void OnUpdate(float _dt) override;
    virtual void NetworkUpdate(float _dt) {};

    void ShowPing(const char* data);

    Entity* m_pFloor = nullptr;
    float m_pCenterFirstRoomZ = 0.0f;

    RoomManager* m_pRoomManager = nullptr;

    Entity* m_pCamera = nullptr;

    UnorderedMap<int, Room*> m_mRooms;
};

#endif