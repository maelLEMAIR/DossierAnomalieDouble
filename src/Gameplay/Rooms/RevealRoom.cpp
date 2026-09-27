#ifndef REVEAL_ROOM_CPP_INCLUDED
#define REVEAL_ROOM_CPP_INCLUDED

#include "RevealRoom.h"
#include "MapLoader.h"
#include "MainScene.h"

void RevealRoom::OnInit(Device* _pDevice, Scene& _scene)
{
    name = "RevealRoom";
    
    Entity* pWallLeft = _scene.CreateEntity();
    pWallLeft->name = "wallLeft";
    Entity* pWallRight = _scene.CreateEntity();
    pWallRight->name = "wallRight";
    Entity* pWallFront1 = _scene.CreateEntity();
    pWallFront1->name = "wallFront1";
    Entity* pWallFront2 = _scene.CreateEntity();
    pWallFront2->name = "wallFront2";
    Entity* pWallFront3 = _scene.CreateEntity();
    pWallFront3->name = "wallFront3";
    Entity* pWallBack1 = _scene.CreateEntity();
    pWallBack1->name = "wallBack1";
    Entity* pWallBack2 = _scene.CreateEntity();
    pWallBack2->name = "wallBack2";
    Entity* pWallBack3 = _scene.CreateEntity();
    pWallBack3->name = "wallBack3";
    Entity* pDoor = _scene.CreateEntity();
    pDoor->name = "Door";
    Entity* pTrap = _scene.CreateEntity();
    pTrap->name = "trap";
    Entity* pCeiling = _scene.CreateEntity();
    pCeiling->name = "ceiling";

    
    Shader* s = RessourceManager::GetShader("Texture");
    Material* matFloor = s->CreateMaterial();
    Texture* texFloorAlbedo = _pDevice->CreateTexture(L"../../res/Textures/TrapWood/wood12_baseColor.dds");
    matFloor->SetTexture("Albedo", texFloorAlbedo);
    Texture* texFloorNormal = _pDevice->CreateTexture(L"../../res/Textures/TrapWood/wood12_normal.dds");
    matFloor->SetTexture("Normal", texFloorNormal);
    Texture* texFloorRough = _pDevice->CreateTexture(L"../../res/Textures/TrapWood/wood12_roughness.dds");
    matFloor->SetTexture("Roughness", texFloorRough);
    
    TransformComponent* pWallLeftComponent = _scene.GetComponentType<TransformComponent>(pWallLeft);
    pWallLeftComponent->transform.SetWorldPosition({-2.5f, 1.5f, 0.0f});
    pWallLeftComponent->transform.SetWorldScale({0.25f, 3.0f, 5.0f});

    Collider* pWallLeftCollider = _scene.AddComponent<Collider>(pWallLeft);
    pWallLeftCollider->isStatic = true;
    pWallLeftCollider->colliderType = ColliderType::BOX;
    
    MeshRenderer* pWallLeftRenderer = _scene.AddComponent<MeshRenderer>(pWallLeft);
    pWallLeftRenderer->pGeometry = RessourceManager::GetGeometry("Cube");
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    TransformComponent* pWallRightComponent = _scene.GetComponentType<TransformComponent>(pWallRight);
    pWallRightComponent->transform.SetWorldPosition({2.5f, 1.5f, 0.0f});
    pWallRightComponent->transform.SetWorldScale({0.25f, 3.0f, 5.0f});

    Collider* pWallRightCollider = _scene.AddComponent<Collider>(pWallRight);
    pWallRightCollider->isStatic = true;
    pWallRightCollider->colliderType = ColliderType::BOX;
    
    MeshRenderer* pWallRightRenderer = _scene.AddComponent<MeshRenderer>(pWallRight);
    pWallRightRenderer->pGeometry = RessourceManager::GetGeometry("Cube");
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    TransformComponent* pWallFront1Transform = _scene.GetComponentType<TransformComponent>(pWallFront1);
    pWallFront1Transform->transform.SetWorldPosition({-1.5f, 1.5f, 2.5f});
    pWallFront1Transform->transform.SetWorldScale({2.0f, 3.0f, 0.25f});

    Collider* pWallFront1Collider = _scene.AddComponent<Collider>(pWallFront1);
    pWallFront1Collider->isStatic = true;
    pWallFront1Collider->colliderType = ColliderType::BOX;

    MeshRenderer* pWallFront1Mesh = _scene.AddComponent<MeshRenderer>(pWallFront1);
    pWallFront1Mesh->pGeometry = RessourceManager::GetGeometry("Cube");
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    TransformComponent* pWallFront2Transform = _scene.GetComponentType<TransformComponent>(pWallFront2);
    pWallFront2Transform->transform.SetWorldPosition({0.0f, 2.55f, 2.5f});
    pWallFront2Transform->transform.SetWorldScale({1.0f, 0.9f, 0.25f});

    Collider* pWallFront2Collider = _scene.AddComponent<Collider>(pWallFront2);
    pWallFront2Collider->isStatic = true;
    pWallFront2Collider->colliderType = ColliderType::BOX;

    MeshRenderer* pWallFront2Mesh = _scene.AddComponent<MeshRenderer>(pWallFront2);
    pWallFront2Mesh->pGeometry = RessourceManager::GetGeometry("Cube");
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    TransformComponent* pWallFront3Transform = _scene.GetComponentType<TransformComponent>(pWallFront3);
    pWallFront3Transform->transform.SetWorldPosition({1.5f, 1.5f, 2.5f});
    pWallFront3Transform->transform.SetWorldScale({2.0f, 3.0f, 0.25f});

    Collider* pWallFront3Collider = _scene.AddComponent<Collider>(pWallFront3);
    pWallFront3Collider->isStatic = true;
    pWallFront3Collider->colliderType = ColliderType::BOX;

    MeshRenderer* pWallFront3Mesh = _scene.AddComponent<MeshRenderer>(pWallFront3);
    pWallFront3Mesh->pGeometry = RessourceManager::GetGeometry("Cube");
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    TransformComponent* pDoorTransform = _scene.GetComponentType<TransformComponent>(pDoor);
    pDoorTransform->transform.SetWorldPosition({0.0f, 1.05f, 2.5f});
    pDoorTransform->transform.SetWorldScale({1.0f, 2.1f, 0.25f});

    Collider* pDoorCollider = _scene.AddComponent<Collider>(pDoor);
    pDoorCollider->isStatic = true;
    pDoorCollider->colliderType = ColliderType::BOX;
    
    MeshRenderer* pDoorRenderer = _scene.AddComponent<MeshRenderer>(pDoor);
    pDoorRenderer->pGeometry = RessourceManager::GetGeometry("Cube");
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    TransformComponent* pWallBack1Transform = _scene.GetComponentType<TransformComponent>(pWallBack1);
    pWallBack1Transform->transform.SetWorldPosition({-1.5f, 1.5f, -2.5f});
    pWallBack1Transform->transform.SetWorldScale({2.0f, 3.0f, 0.25f});

    Collider* pWallBack1Collider = _scene.AddComponent<Collider>(pWallBack1);
    pWallBack1Collider->isStatic = true;
    pWallBack1Collider->colliderType = ColliderType::BOX;

    MeshRenderer* pWallBack1Mesh = _scene.AddComponent<MeshRenderer>(pWallBack1);
    pWallBack1Mesh->pGeometry = RessourceManager::GetGeometry("Cube");
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    TransformComponent* pWallBack2Transform = _scene.GetComponentType<TransformComponent>(pWallBack2);
    pWallBack2Transform->transform.SetWorldPosition({0.0f, 2.55f, -2.5f});
    pWallBack2Transform->transform.SetWorldScale({1.0f, 0.9f, 0.25f});

    Collider* pWallBack2Collider = _scene.AddComponent<Collider>(pWallBack2);
    pWallBack2Collider->isStatic = true;
    pWallBack2Collider->colliderType = ColliderType::BOX;

    MeshRenderer* pWallBack2Mesh = _scene.AddComponent<MeshRenderer>(pWallBack2);
    pWallBack2Mesh->pGeometry = RessourceManager::GetGeometry("Cube");
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    TransformComponent* pWallBack3Transform = _scene.GetComponentType<TransformComponent>(pWallBack3);
    pWallBack3Transform->transform.SetWorldPosition({1.5f, 1.5f, -2.5f});
    pWallBack3Transform->transform.SetWorldScale({2.0f, 3.0f, 0.25f});

    Collider* pWallBack3Collider = _scene.AddComponent<Collider>(pWallBack3);
    pWallBack3Collider->isStatic = true;
    pWallBack3Collider->colliderType = ColliderType::BOX;

    MeshRenderer* pWallBack3Mesh = _scene.AddComponent<MeshRenderer>(pWallBack3);
    pWallBack3Mesh->pGeometry = RessourceManager::GetGeometry("Cube");
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    TransformComponent* pCeilingTransform = _scene.GetComponentType<TransformComponent>(pCeiling);
    pCeilingTransform->transform.SetWorldPosition({0.0f, 3.125f, 0.0f});
    pCeilingTransform->transform.SetWorldScale({2.5f, 0.125f, 2.5f});

    Collider* pCeilingCollider = _scene.AddComponent<Collider>(pCeiling);
    pCeilingCollider->isStatic = true;
    pCeilingCollider->colliderType = ColliderType::BOX;
    pCeilingCollider->scale = {2.0f, 2.0f, 2.0f};
    
    MeshRenderer* pCeilingRenderer = _scene.AddComponent<MeshRenderer>(pCeiling);
    pCeilingRenderer->pGeometry = RessourceManager::GetGeometry("Trap");
    pCeilingRenderer->pMaterial = matFloor;
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    TransformComponent* pTrapTransform = _scene.GetComponentType<TransformComponent>(pTrap);
    pTrapTransform->transform.SetWorldPosition({0.0f, 0.0f, 0.0f});
    pTrapTransform->transform.SetWorldScale({2.5f, 0.125f, 2.5f});

    Collider* pTrapCollider = _scene.AddComponent<Collider>(pTrap);
    pTrapCollider->isStatic = true;
    pTrapCollider->colliderType = ColliderType::BOX;
    pTrapCollider->scale = {2.0f, 2.0f, 2.0f};
    
    MeshRenderer* pTrapRenderer = _scene.AddComponent<MeshRenderer>(pTrap);
    pTrapRenderer->pGeometry = RessourceManager::GetGeometry("Trap");
    pTrapRenderer->pMaterial = matFloor;
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Entity* pTrapTrigger = _scene.CreateEntity();

    TransformComponent* pTrapTriggerTransform = _scene.GetComponentType<TransformComponent>(pTrapTrigger);
    pTrapTriggerTransform->transform.SetWorldPosition({0.0f, 1.5f, 0.0f});
    pTrapTriggerTransform->transform.SetWorldScale({5.0f, 3.0f, 5.0f});

    Collider* pTrapTriggerCollider = _scene.AddComponent<Collider>(pTrapTrigger);
    pTrapTriggerCollider->isTrigger = true;
    pTrapTriggerCollider->colliderType = ColliderType::BOX;

    m_pTrapStateMachine = new TrapStateMachine();
    m_pTrapStateMachine->m_pRoom = this;
    m_pTrapStateMachine->pDoorTransform = pDoorTransform;
    m_pTrapStateMachine->pTrapTransform = pTrapTransform;
    
    StateMachineComponent* pTrapStateMachine = _scene.AddComponent<StateMachineComponent>(pTrapTrigger);
    pTrapStateMachine->SetStateGlobal(m_pTrapStateMachine);

    centerPos = { 0.0f, 0.0f, 0.0f};
    size = {5.0f, 0.25f, 5.0f};
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    

    m_vEntities.push_back(pDoor);
    m_vEntities.push_back(pTrap);
    m_vEntities.push_back(pCeiling);
    m_vEntities.push_back(pTrapTrigger);
    m_vEntities.push_back(pWallLeft);
    m_vEntities.push_back(pWallRight);
    m_vEntities.push_back(pWallBack1);
    m_vEntities.push_back(pWallBack2);
    m_vEntities.push_back(pWallBack3);
    m_vEntities.push_back(pWallFront1);
    m_vEntities.push_back(pWallFront2);
    m_vEntities.push_back(pWallFront3);
    
    m_vLightsPos.push_back({-1.75f, 2.75f, 0.0f});
    m_vLightsPos.push_back({1.75f, 2.75f, 0.0f});

    Room::OnInit(_pDevice, _scene);
}

void RevealRoom::ActiveRoom(XMFLOAT3 newCenter)
{
    Room::ActiveRoom(newCenter);
    XMFLOAT3 color = ToColor(255, 165, 0);
    for ( LightComponent* pLight : m_vLightsUse )
    {
        pLight->SetColor({color.x, color.y, color.z, 1.0f});
        pLight->SetFalloffEnd(4.0f);
        pLight->SetStrength(0.8f);
    }

    MainScene* pScene = reinterpret_cast<MainScene*>(SceneManager::GetSceneWithName("MainScene"));
    pScene->m_inGameData.vTrapeSM.push_back(m_pTrapStateMachine);
}

void RevealRoom::DisabledRoom(float _disableEntity)
{
    Room::DisabledRoom(_disableEntity);
    m_pTrapStateMachine->Reset();
}

void RevealRoom::ChangeColorLight(bool _toGreen)
{
    for (LightComponent* pLight : m_vLightsUse)
    {
        if (_toGreen)
            pLight->SetColor({ 0.0f, 1.0f, 0.0f, 1.0f });
        else
            pLight->SetColor({ 1.0f, 0.0f, 0.0f, 1.0f });
    }
}

void RevealRoom::ResetColorLight()
{
    for (LightComponent* pLight : m_vLightsUse)
    {
        pLight->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });
    }
}


#endif
