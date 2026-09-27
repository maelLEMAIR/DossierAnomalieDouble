#ifndef MAIN_SCENE_SERVEUR_CPP_INCLUDED
#define MAIN_SCENE_SERVEUR_CPP_INCLUDED
#include "MainScene_Serveur.h"

#include "Rooms/Room.h"
#include "GameManager.h"

void MainScene_Serveur::NetworkStart(UnorderedMap<int, Room*> _pRoom)
{
    std::uniform_int_distribution dist(_pRoom.size() / 2, _pRoom.size() - 1);
    int roomRand = dist(EngineManager::GetRand());

    m_pRoomManager->SetNextRoom(roomRand);

    float centerFirstRoomZ = _pRoom[roomRand]->centerPos.z - _pRoom[roomRand]->size.z / 2.0f - 10.0f / 2.0f;
    std::cout << "center first room : " << centerFirstRoomZ << std::endl;
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

    Data startRoom;
    startRoom.Init(Cmd::START_ROOM, { &roomRand, &centerFirstRoomZ }, { Type::TYPE_INT1, Type::TYPE_FLOAT });

    EngineManager::GetSocket()->SendSecurTo(startRoom, GameManager::GetOtherPlayerId());

    EngineManager::GetSocket()->LinkCmdFunc(Cmd::STATE_LEVER, [this](const char* _data, std::string _id, sockaddr_in* _from) {this->ReceiveStateLever(_data); });
}

void MainScene_Serveur::NetworkUpdate(float _dt)
{
    if (m_inGameData.receiveStateLever && m_inGameData.inRevealeRoom)
    {
        bool isWin = true;

        if (m_inGameData.ownStateLever != m_inGameData.stateLever)
        {
            //loose
            //open trap
            m_inGameData.vTrapeSM[0]->openTrap = true;
            isWin = false;
            std::cout << "condition one" << std::endl;
        }
        else if (m_inGameData.ownStateLever != m_inGameData.hasAnomaly)
        {
            m_inGameData.vTrapeSM[0]->openTrap = true;
            isWin = false;
            std::cout << "condition two" << std::endl;
        }
        else
        {
            m_inGameData.vTrapeSM[0]->openTrap = false;
            std::cout << "condition three" << std::endl;
        }

        m_inGameData.receiveStateLever = false;
        m_inGameData.vTrapeSM[0]->StartCooldown();
        m_inGameData.vTrapeSM.erase(m_inGameData.vTrapeSM.begin());

        Data data;
        data.Init(Cmd::START_REVEALE_ROOM, {&isWin}, {Type::TYPE_BOOL});

        EngineManager::GetSocket()->SendSecurTo(data, GameManager::GetOtherPlayerId());

        RoomManager::GetInstance()->loadNextRoom = true;
    }

    if (m_pPlayer != nullptr)
    {
        TransformComponent* pPlayerTransform = GetComponentType<TransformComponent>(m_pPlayer);
        if (pPlayerTransform != nullptr)
        {
            XMFLOAT3 pos = pPlayerTransform->transform.GetWorldPosition();

            if (pos.y < -10.0f)
            {
                Restart();
            }
        }
    }
}

void MainScene_Serveur::Restart()
{
    //Reset();

    m_inGameData.receiveStateLever = false;
    m_inGameData.stateLever = false;
    m_inGameData.ownStateLever = false;
    m_inGameData.hasAnomaly = false;
    m_inGameData.inRevealeRoom = false;
    m_inGameData.vTrapeSM.clear();

    m_pRoomManager->Reset();

    std::uniform_int_distribution dist(m_mRooms.size() / 2, m_mRooms.size() - 1);
    int roomRand = dist(EngineManager::GetRand());

    m_pRoomManager->SetNextRoom(roomRand);

    float centerFirstRoomZ = m_mRooms[roomRand]->centerPos.z - m_mRooms[roomRand]->size.z / 2.0f - 10.0f / 2.0f;
    std::cout << "center first room : " << centerFirstRoomZ << std::endl;

    TransformComponent* pFloorTransform = GetComponentType<TransformComponent>(m_pFloor);
    pFloorTransform->transform.SetWorldPosition({ 0.0f, 0.0f, centerFirstRoomZ });
    MeshRenderer* pFloorMesh = GetComponentType<MeshRenderer>(m_pFloor);
    pFloorMesh->pGeometry = RessourceManager::GetGeometry("Cube");
    Collider* pFloorCollider = GetComponentType<Collider>(m_pFloor);

    TransformComponent* pPlayerTransform = GetComponentType<TransformComponent>(m_pPlayer);
    pPlayerTransform->transform.SetWorldPosition({ 0.0f, 2.0f, centerFirstRoomZ });

    Data startRoom;
    startRoom.Init(Cmd::RESTART, { &roomRand, &centerFirstRoomZ }, { Type::TYPE_INT1, Type::TYPE_FLOAT });

    EngineManager::GetSocket()->SendSecurTo(startRoom, GameManager::GetOtherPlayerId());
}

void MainScene_Serveur::ReceiveStateLever(const char* data)
{
    bool stateLever = false;

    memcpy(&stateLever, data, 1);

    m_inGameData.receiveStateLever = true;
    m_inGameData.stateLever = stateLever;
}

#endif