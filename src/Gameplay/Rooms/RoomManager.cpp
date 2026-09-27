#ifndef ROOM_MANAGER_CPP_INCLUDED
#define ROOM_MANAGER_CPP_INCLUDED

#include "Room.h"
#include "RoomManager.h"
#include "RevealRoom.h"
#include "../GameManager.h"
#include "MainScene.h"

Vector<int> RoomManager::AvailableRoom()
{
    Vector<int> room;
    for (int i = (int)m_mRooms.size() / 2; i < (int)m_mRooms.size(); i++)
    {
        if (i != m_currentRoom)
            room.push_back(i);
    }
    return room;
}

void RoomManager::Update(float _dt)
{
    if (m_vRooms.empty())
        return;

    m_pAnomaliesManager->Update(_dt);
    if (loadNextRoom == false)
        return;
    
    XMFLOAT3 posPlayer = pPlayerTransform->transform.GetWorldPosition();
    Room* pLastRoom = m_vRooms.back().second;
    
    if (posPlayer.z > pLastRoom->centerPos.z - pLastRoom->size.z / 2.0f &&
        posPlayer.z < pLastRoom->centerPos.z + pLastRoom->size.z / 2.0f)
    {
        Vector<int> availableRoom = AvailableRoom();
    
        if (availableRoom.empty()) return;

        loadNextRoom = false;
    
        std::uniform_int_distribution<int> dist(0, (int)availableRoom.size() - 1);
        int roomRand = dist(EngineManager::GetRand());

        std::uniform_int_distribution<int> distProbaAnomalyExist(0, 10);
        int anomalyExistRand = distProbaAnomalyExist(EngineManager::GetRand());

        bool hasAnomaly = false;
        bool hasAnomalySend = false;

        MainScene* pMainScene = reinterpret_cast<MainScene*>(SceneManager::GetSceneWithName("MainScene"));

        if (anomalyExistRand <= 5)
        {
            std::uniform_int_distribution<int> proba(0, 1);
            hasAnomaly = proba(EngineManager::GetRand());
            hasAnomalySend = !hasAnomaly;

            pMainScene->m_inGameData.hasAnomaly = true;
        }
        else
        {
            pMainScene->m_inGameData.hasAnomaly = false;
        }
   
        Data rdm;
        rdm.Init(Cmd::ID_NEXT_ROOM, { &roomRand, &hasAnomalySend }, { Type::TYPE_INT1, Type::TYPE_BOOL });

        EngineManager::GetSocket()->SendSecurTo(rdm, GameManager::GetOtherPlayerId());

        LoadNextRoom(roomRand, hasAnomaly);
    }
}

void RoomManager::Reset()
{
    for (int i = 0; i < m_vRooms.size(); i++)
    {
        if (m_vRooms[i].second != nullptr)
        {
            m_vRooms[i].second->DisabledRoom();
        }
    }

    m_vRooms.clear();
    m_currentRoom = -1;
    loadNextRoom = false;
}

void RoomManager::LoadNextRoom(int _idRoom, bool _hasAnomaly)
{
    Room* roomAdd = nullptr;
    Vector<int> availableRoom = AvailableRoom();

    roomAdd = m_mRooms[availableRoom[_idRoom]];

    m_pAnomaliesManager->DisableAnomaly();
    if (_hasAnomaly)
    {
        m_pAnomaliesManager->ChooseAnAnomaly(roomAdd);
    }

    if (m_vRooms.size() == 4)
    {
        if (roomAdd != nullptr)
        {
            if (m_vRooms.front().second != roomAdd)
            {
                m_mRooms[m_vRooms.front().first]->DisabledRoom();
                m_vRooms.erase(m_vRooms.begin());
                m_mRooms[m_vRooms.front().first]->pRevealRoom->DisabledRoom();
                m_vRooms.erase(m_vRooms.begin());
            }
            else
            {
                m_mRooms[m_vRooms.front().first]->DisabledRoom(false);
                m_vRooms.erase(m_vRooms.begin());
                m_mRooms[m_vRooms.front().first]->pRevealRoom->DisabledRoom(false);
                m_vRooms.erase(m_vRooms.begin());
            }
        }
        else
        {
            m_mRooms[m_vRooms.front().first]->DisabledRoom();
            m_vRooms.erase(m_vRooms.begin());
            m_mRooms[m_vRooms.front().first]->pRevealRoom->DisabledRoom();
            m_vRooms.erase(m_vRooms.begin());
        }
    }

    SetNextRoom(availableRoom[_idRoom]);
}

void RoomManager::ActiveRoom(int _roomNumber)
{
    Room* pRoom = m_mRooms[_roomNumber];

    if (m_vRooms.empty())
    {
        pRoom->ActiveRoom(XMFLOAT3(0.0f, 0.0f, 0.0f));
        m_vRooms.push_back({_roomNumber, pRoom});
    }
    else
    {
        Room* pLastRoom = m_vRooms.back().second;
        XMFLOAT3 nextCenter = pLastRoom->centerPos;
        nextCenter.z += pLastRoom->size.z / 2.0f + pRoom->size.z / 2.0f;
        pRoom->ActiveRoom(nextCenter);
        m_vRooms.push_back({_roomNumber, pRoom});
    }

    RevealRoom* pRevealRoom = pRoom->pRevealRoom;
    XMFLOAT3 centerReveal = pRoom->centerPos;
    centerReveal.z += pRoom->size.z / 2.0f + pRevealRoom->size.z / 2.0f;
    pRevealRoom->ActiveRoom(centerReveal);
    m_vRooms.push_back({_roomNumber, pRevealRoom});
}

#endif
