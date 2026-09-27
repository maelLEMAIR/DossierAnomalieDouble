#ifndef MAIN_SCENE_CLIENT_CPP_INCLUDED
#define MAIN_SCENE_CLIENT_CPP_INCLUDED
#include "MainScene_Client.h"

#include "Rooms/Room.h"

void MainScene_Client::NetworkStart(UnorderedMap<int, Room*> _pRoom)
{
    EngineManager::GetSocket()->LinkCmdFunc(Cmd::START_ROOM, [this](const char* _data, std::string _id, sockaddr_in* _from) {this->StartRoom(_data); });
    EngineManager::GetSocket()->LinkCmdFunc(Cmd::ID_NEXT_ROOM, [this](const char* _data, std::string _id, sockaddr_in* _from) {this->NextRoom(_data); });
    EngineManager::GetSocket()->LinkCmdFunc(Cmd::START_REVEALE_ROOM, [this](const char* _data, std::string _id, sockaddr_in* _from) {this->StartRevealeRoom(_data); });
    EngineManager::GetSocket()->LinkCmdFunc(Cmd::RESTART, [this](const char* _data, std::string _id, sockaddr_in* _from) {this->Reset(_data); });
}

void MainScene_Client::NetworkUpdate(float _dt)
{
    if (m_canLoadNextRoom && m_receiveNextRoom)
    {
        m_canLoadNextRoom = false;
        m_receiveNextRoom = false;

        RoomManager::GetInstance()->LoadNextRoom(m_idNextRoom, m_hasAnomaly);
    }
}

void MainScene_Client::StartRoom(const char* _data)
{
    int roomRand = 0;
    memcpy(&roomRand, _data, 1);

    m_pRoomManager->SetNextRoom(roomRand);

    float centerFirstRoomZ = 0;
    memcpy(&centerFirstRoomZ, _data + 1, 4);

    m_pFloor = CreateEntity();
    TransformComponent* pFloorTransform = GetComponentType<TransformComponent>(m_pFloor);
    pFloorTransform->transform.SetWorldPosition({ 0.0f, 0.0f, centerFirstRoomZ });
    pFloorTransform->transform.SetWorldScale({ 10.0f, 0.125f, 10.0f });
    MeshRenderer* pFloorMesh = AddComponent<MeshRenderer>(m_pFloor);
    pFloorMesh->pGeometry = RessourceManager::GetGeometry("Cube");
    Collider* pFloorCollider = AddComponent<Collider>(m_pFloor);
    pFloorCollider->colliderType = ColliderType::BOX;
    pFloorCollider->isStatic = true;

    TransformComponent* pPlayerTransform = GetComponentType<TransformComponent>(m_pPlayer);
    pPlayerTransform->transform.SetWorldPosition({ 0.0f, 2.0f, centerFirstRoomZ });
}

void MainScene_Client::NextRoom(const char* _data)
{
    int roomId = 0;
    memcpy(&roomId, _data, 1);

    m_idNextRoom = roomId;

    bool hasAnomaly = false;
    memcpy(&hasAnomaly, _data + 1, 1);

    m_hasAnomaly = hasAnomaly;

    m_receiveNextRoom = true;

    //RoomManager::GetInstance()->LoadNextRoom(roomId);
}

void MainScene_Client::StartRevealeRoom(const char* _data)
{
    bool isWin = true;

    memcpy(&isWin, _data, 1);

    m_inGameData.vTrapeSM[0]->openTrap = !isWin;

    m_inGameData.vTrapeSM[0]->StartCooldown();
    m_inGameData.vTrapeSM.erase(m_inGameData.vTrapeSM.begin());
}

void MainScene_Client::Reset(const char* _data)
{
    int roomRand = 0;
    memcpy(&roomRand, _data, 1);

    float centerFirstRoomZ = 0;
    memcpy(&centerFirstRoomZ, _data + 1, 4);

    m_pRoomManager->Reset();

    m_inGameData.receiveStateLever = false;
    m_inGameData.inRevealeRoom = false;

    m_pRoomManager->SetNextRoom(roomRand);

    TransformComponent* pFloorTransform = GetComponentType<TransformComponent>(m_pFloor);
    pFloorTransform->transform.SetWorldPosition({ 0.0f, 0.0f, centerFirstRoomZ });
    MeshRenderer* pFloorMesh = GetComponentType<MeshRenderer>(m_pFloor);
    Collider* pFloorCollider = GetComponentType<Collider>(m_pFloor);

    TransformComponent* pPlayerTransform = GetComponentType<TransformComponent>(m_pPlayer);
    pPlayerTransform->transform.SetWorldPosition({ 0.0f, 2.0f, centerFirstRoomZ });
}

#endif