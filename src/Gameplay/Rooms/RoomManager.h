#ifndef ROOM_MANAGER_H_INCLUDED
#define ROOM_MANAGER_H_INCLUDED

#include "pch.h"
#include "Anomalies/AnomaliesManager.h"

class Room;
class RevealRoom;

class RoomManager
{
public:
    RoomManager() { s_pInstance = this; m_pAnomaliesManager = new AnomaliesManager; }
    static RoomManager* GetInstance() { return s_pInstance; }
    
    void SetRoomMap(UnorderedMap<int, Room*> const& _mRooms) { m_mRooms = _mRooms; }
    void SetNextRoom(int _roomNumber) { ActiveRoom(_roomNumber); m_currentRoom = _roomNumber; }
    Vector<int> AvailableRoom();
    UnorderedMap<int, Room*> GetMapRoom() { return m_mRooms; }
    
    void Update(float _dt);
    void Reset();

    void LoadNextRoom(int _idRoom, bool _hasAnomaly = false);
    bool loadNextRoom = false;
    
    Room* GetRoom(int _id) { return m_mRooms[_id]; }

    Scene* pCurrentScene = nullptr;
    TransformComponent* pPlayerTransform = nullptr;
    
private:
    void ActiveRoom(int _roomNumber);
    
    inline static RoomManager* s_pInstance = nullptr;

    AnomaliesManager* m_pAnomaliesManager = nullptr;
    
    int m_currentRoom = 0;
    Vector<Pair<int, Room*>> m_vRooms;
    UnorderedMap<int, Room*> m_mRooms;
};

#endif