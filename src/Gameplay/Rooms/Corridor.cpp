#ifndef CORRIDOR_CPP_INCLUDED
#define CORRIDOR_CPP_INCLUDED

#include "Corridor.h"
#include "MapLoader.h"

void Corridor::OnInit(Device* _pDevice, Scene& _scene)
{
    name = "Corridor";
    MapLoader::LoadMap("../../res/JSON/map_Corridor.json", _pDevice, _scene, this, "Corridor");

    centerPos = {0.0f, -0.125f, 0.0f};
    size = {4.0f, 0.125f, 20.0f};

    Geometry* pLeverGeo = GeometryFactory::LoadGeometry(_pDevice, "../../res/Obj/lever.obj");
    RessourceManager::AddGeometry("lever", pLeverGeo);
    Shader* s = RessourceManager::GetShader("Texture");
    Material* leverMat = s->CreateMaterial();
    Texture* leverText = _pDevice->CreateTexture(L"../../res/Textures/Lever/lever.dds");
    leverMat->SetTexture("Albedo", leverText);
    RessourceManager::AddMaterial("lever", leverMat);
    
    Entity* pLeverSupport = _scene.CreateEntity();
    TransformComponent* pLeverSupportTransform = _scene.GetComponentType<TransformComponent>(pLeverSupport);
    pLeverSupportTransform->transform.SetWorldPosition({1.0f, 1.5f, size.z / 2.0f - 0.3f});
    pLeverSupportTransform->transform.SetWorldScale({0.5f, 0.5f, 0.1f});
    MeshRenderer* pLeverSupportMesh = _scene.AddComponent<MeshRenderer>(pLeverSupport);
    pLeverSupportMesh->pGeometry = RessourceManager::GetGeometry("Cube");
    
    Entity* pLever = _scene.CreateEntity();
    TransformComponent* pTransformLever = _scene.GetComponentType<TransformComponent>(pLever);
    pTransformLever->transform.SetWorldPosition({1.0f, 1.5f, size.z / 2.0f - 0.4f});
    pTransformLever->transform.SetWorldScale({0.25f, 0.25f, 0.25f});
    MeshRenderer* pLeverMesh = _scene.AddComponent<MeshRenderer>(pLever);
    pLeverMesh->pGeometry = pLeverGeo;
    pLeverMesh->pMaterial = leverMat;

    Entity* pLeverIndicator = _scene.CreateEntity();
    TransformComponent* pTransformLeverIndicator = _scene.GetComponentType<TransformComponent>(pLeverIndicator);
    pTransformLeverIndicator->transform.SetWorldPosition({1.0f, 2.0f, size.z / 2.0f - 0.35f});
    pTransformLeverIndicator->transform.SetWorldScale({0.1f, 0.5f, 0.1f});
    pTransformLeverIndicator->transform.SetLocalRotation({0.0f, 0.0f, XM_PIDIV2});
    MeshRenderer* pLeverIndicatorMesh = _scene.AddComponent<MeshRenderer>(pLeverIndicator);
    pLeverIndicatorMesh->pGeometry = RessourceManager::GetGeometry("Cylinder");
    pLeverIndicatorMesh->pMaterial = RessourceManager::GetMaterial("Red");
    
    Entity* pLeverTrigger = _scene.CreateEntity();
    TransformComponent* pTriggerTransform = _scene.GetComponentType<TransformComponent>(pLeverTrigger);
    pTriggerTransform->transform.SetWorldPosition({1.0f, 0.5f, size.z / 2.0f - 1.0f});
    pTriggerTransform->transform.SetWorldScale({0.5f, 2.0f, 1.5f});
    Collider* pTriggerCollider = _scene.AddComponent<Collider>(pLeverTrigger);
    pTriggerCollider->isTrigger = true;
    pTriggerCollider->colliderType = ColliderType::BOX;

    m_pLeverStateMachine = new LeverStateMachine();
    m_pLeverStateMachine->pTransformLever = pTransformLever;
    m_pLeverStateMachine->pRoom = this;
    m_pLeverStateMachine->pMeshRendererLeverIndicator = pLeverIndicatorMesh;

    StateMachineComponent* pLeverStateMachine = _scene.AddComponent<StateMachineComponent>(pLeverTrigger);
    pLeverStateMachine->SetStateGlobal(m_pLeverStateMachine);
    
    m_vEntities.push_back(pLever);
    m_vEntities.push_back(pLeverIndicator);
    m_vEntities.push_back(pLeverSupport);
    m_vEntities.push_back(pLeverTrigger);
    
    Room::OnInit(_pDevice, _scene);
}

void Corridor::ActiveRoom(XMFLOAT3 newCenter)
{
    Room::ActiveRoom(newCenter);
}

void Corridor::DisabledRoom(float _disableEntity)
{
    Room::DisabledRoom(_disableEntity);
}

#endif
