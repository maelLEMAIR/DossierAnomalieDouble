#ifndef MAIN_SCENE_CPP_INCLUDED
#define MAIN_SCENE_CPP_INCLUDED

#include "MainScene.h"
#include "MapLoader.h"
#include "GameManager.h"

//ROOMS
#include "Rooms/RoomManager.h"
#include "Rooms/RevealRoom.h"
#include "Rooms/Hall.h"
#include "Rooms/Corridor.h"
#include "Rooms/Bedroom.h"
#include "Rooms/DiningRoom.h"

#include "Anomalies/Gravity.h"
#include "Anomalies/Scaling.h"
#include "PingTriangle.h"

void MainScene::OnInit()
{
    RegisterSystem<LifeTimeSystem>();

    Device* pDevice = EngineManager::GetDevice();

    /*Texture* blurText = pDevice->CreateTexture(L"../../res/Textures/VHS.dds");

    Sprite* rect = SpriteFactory::BuildRectangle(pDevice, EngineManager::GetWindow()->GetWidth(),
        EngineManager::GetWindow()->GetHeight());

    Entity* blurEntity = CreateEntity();
    SpriteComponent* sprite = AddComponent<SpriteComponent>(blurEntity);
    sprite->pRect = rect;
    sprite->SetTexture(blurText);*/

    Geometry* pCubeGeo = GeometryFactory::BuildCube(pDevice);
    Geometry* pSphereGeo = GeometryFactory::BuildIcosphere(pDevice, 1);
    Geometry* pPyramide = GeometryFactory::BuildPyramid(pDevice);
    Geometry* pCylinder = GeometryFactory::BuildCylinder(pDevice, 16);
    Geometry* pTrap = GeometryFactory::LoadJsonGeometryWPath(pDevice, "../../res/Obj/trap.json");
    RessourceManager::AddGeometry("Cube", pCubeGeo);
    RessourceManager::AddGeometry("Sphere", pSphereGeo);
    RessourceManager::AddGeometry("Pyramide", pPyramide);
    RessourceManager::AddGeometry("Cylinder", pCylinder);
    RessourceManager::AddGeometry("Trap", pTrap);

    Material* pBlue = RessourceManager::GetShader("Color")->CreateMaterial();
    pBlue->SetFloat4("DiffuseAlbedo", { 0.0f, 0.0f, 1.0f, 1.0f });
    RessourceManager::AddMaterial("Blue", pBlue);

    Material* pGreen = RessourceManager::GetShader("Color")->CreateMaterial();
    pGreen->SetFloat4("DiffuseAlbedo", { 0.0f, 1.0f, 0.0f, 1.0f });
    RessourceManager::AddMaterial("Green", pGreen);

    Material* pRed = RessourceManager::GetShader("Color")->CreateMaterial();
    pRed->SetFloat4("DiffuseAlbedo", { 1.0f, 0.0f, 0.0f, 1.0f });
    RessourceManager::AddMaterial("Red", pRed);

    Sound* pBgSound = AudioEngine::LoadWav(L"../../res/Audio/bgMusic.wav");
    AudioEngine::PlaySoundW(pBgSound, 0.1f, true);

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    m_pCamera = pCamera->GetOwner();
    m_pCamera->name = "Camera";
    m_pPlayer = CreateEntity();
    m_pPlayer->name = "Player";
    m_pPlayer->tag = PLAYER_TAG;
    m_pInfo = CreateEntity();
    m_pInfo->name = "Info";

    XMINT2 sizeWindow = { EngineManager::GetWindow()->GetWidth(), EngineManager::GetWindow()->GetHeight() };
    TransformComponent* pTransformInfo = this->AddComponent<TransformComponent>(m_pInfo);
    pTransformInfo->transform.SetWorldPosition(XMFLOAT3(-100.0f, (float)sizeWindow.y / 2.0f - 75.f, 0.0f));
    pTransformInfo->transform.SetWorldScale(1.5f);

    TextComponent* pTextInfo = this->AddComponent<TextComponent>(m_pInfo);
    pTextInfo->SetText("press E");
    m_pInfo->isActive = false;

    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    TransformComponent* pPlayerTransform = AddComponent<TransformComponent>(m_pPlayer);
    pPlayerTransform->transform.SetWorldPosition(XMFLOAT3(0.0f, 2.0f, 0.0f));
    Collider* pPlayerColliderComponent = AddComponent<Collider>(m_pPlayer);
    pPlayerColliderComponent->colliderType = ColliderType::BOX;
    pPlayerColliderComponent->scale = XMFLOAT3(0.3f, 1.7f, 0.3f);
    pPlayerColliderComponent->isActive = true;
    pPlayerColliderComponent->canBounce = false;

    ForceComponent* pPlayerForceComponent = AddComponent<ForceComponent>(m_pPlayer);
    pPlayerForceComponent->useGravity = true;

    ParentComponent* pPlayerParentComponent = AddComponent<ParentComponent>(m_pPlayer);
    pPlayerParentComponent->vChildrens.push_back(m_pCamera);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ChildrenComponent* pCamChildComponent = AddComponent<ChildrenComponent>(m_pCamera);
    pCamChildComponent->SetParent(m_pPlayer);
    TransformComponent* pTransformCamera = AddComponent<TransformComponent>(m_pCamera);
    pTransformCamera->transform.SetLocalPosition(XMFLOAT3(0.0f, 0.75f, 0.0f));
    CameraComponent* pCamComponent = GetComponentType<CameraComponent>(m_pCamera);
    pCamComponent->camera.nearPlane = 0.1f;
    EngineManager::GetDevice()->SetMainCamera(&pCamComponent->camera);

    PlayerStateGlobal* statePlayer = new PlayerStateGlobal();
    statePlayer->m_sensitive = 0.30f;
    statePlayer->m_pCam = m_pCamera;
    StateMachineComponent* pPlayerSM = AddComponent<StateMachineComponent>(m_pPlayer);
    pPlayerSM->SetStateGlobal(statePlayer);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    m_pRoomManager = new RoomManager();
    m_pRoomManager->pCurrentScene = this;
    m_pRoomManager->pPlayerTransform = pPlayerTransform;
}

UnorderedMap<int, Room*> MainScene::GenerateRooms()
{
    Device* pDevice = EngineManager::GetDevice();
    UnorderedMap<int, Room*> m_mRooms;

    AnomalyConfig cfgScaling = AnomalyConfig( 50.0f, 0.1f, false, 2.5f);
    Scaling* pScalePlayerAnomaly = new Scaling(cfgScaling, m_pPlayer);
    pScalePlayerAnomaly->OnInit();
    AnomalyConfig cfgGravity = AnomalyConfig(2.0f, 1.0f, true, 5.0f, 2.0f, 5.0f);
    Gravity* pGravityPlayerAnomaly = new Gravity(cfgGravity, m_pPlayer);
    pGravityPlayerAnomaly->OnInit();

    RevealRoom* pRevealRoom = new RevealRoom;
    pRevealRoom->OnInit(pDevice, *this);
    Corridor* pCorridor = new Corridor;
    pCorridor->pRevealRoom = pRevealRoom;
    pCorridor->OnInit(pDevice, *this);
    pCorridor->AddAnomaly(AnomalyType::GravitySwitch, pGravityPlayerAnomaly);

    RevealRoom* pRevealRoom2 = new RevealRoom;
    pRevealRoom2->OnInit(pDevice, *this);
    Hall* pHall = new Hall;
    pHall->pRevealRoom = pRevealRoom2;
    pHall->OnInit(pDevice, *this);
    pHall->AddAnomaly(AnomalyType::GravitySwitch, pGravityPlayerAnomaly);

    RevealRoom* pRevealRoom3 = new RevealRoom;
    pRevealRoom3->OnInit(pDevice, *this);
    Bedroom* pBedroom = new Bedroom;
    pBedroom->pRevealRoom = pRevealRoom3;
    pBedroom->OnInit(pDevice, *this);
    pBedroom->AddAnomaly(AnomalyType::GravitySwitch, pGravityPlayerAnomaly);

    RevealRoom* pRevealRoom4 = new RevealRoom;
    pRevealRoom4->OnInit(pDevice, *this);
    DiningRoom* pDiningRoom = new DiningRoom;
    pDiningRoom->pRevealRoom = pRevealRoom4;
    pDiningRoom->OnInit(pDevice, *this);

    m_mRooms[0] = pRevealRoom;
    m_mRooms[1] = pRevealRoom2;
    m_mRooms[2] = pRevealRoom3;

    m_mRooms[3] = pCorridor;
    m_mRooms[4] = pHall;
    m_mRooms[5] = pBedroom;

    m_pRoomManager->SetRoomMap(m_mRooms);

    return m_mRooms;
}

void MainScene::OnStart()
{
    m_mRooms = GenerateRooms();

    XMINT2 center(EngineManager::GetWindow()->GetWidth() / 2, EngineManager::GetWindow()->GetHeight() / 2);
    InputSystem::LockMouseCursor();
    InputSystem::SetMousePosition(center);
    InputSystem::HideMouseCursor();
 
    EngineManager::GetSocket()->LinkCmdFunc(Cmd::SHOW_PING, [this](const char* _data, std::string _id, sockaddr_in* _from) {this->ShowPing(_data); });

    NetworkStart(m_mRooms);
}

void MainScene::OnUpdate(float _dt)
{
    m_pRoomManager->Update(_dt);
    NetworkUpdate(_dt);

    if (EngineManager::GetSocket()->ProfilIsConnected((char*)GameManager::GetOtherPlayerId().c_str()) == false)
    {
        std::cout << "Connexion lost with other player return to menu" << std::endl;
        EngineManager::CloseSocket();
        SceneManager::ChangeCurrentScene("MenuScene");
    }
}

void MainScene::ShowPing(const char* data)
{
    XMFLOAT3 pos = { 0.0f, 0.0f, 0.0f };
    memcpy(&pos, data, 12);

    PingTriangle ping;
    ping.Init(SceneManager::GetCurrentScene(), pos);
}



void MainScene::Reset()
{
    StateMachineComponent* pPlayerSM = GetComponentType<StateMachineComponent>(m_pCamera);
    PlayerStateGlobal* state = dynamic_cast<PlayerStateGlobal*>(pPlayerSM->GetStateGlobal());
    state->Reset();
}

void MainScene::EnterNewRoom()
{
    m_inGameData.receiveStateLever = false;
}



#endif
