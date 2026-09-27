#ifndef TEST_RESSOURCE_MANAGER_HPP_DEFINED
#define TEST_RESSOURCE_MANAGER_HPP_DEFINED

#include "pch.h"
#include "Test.h"

class TestRessourceManager : public Test
{
public:
    static void Run()
    {
        EngineManager engineManager;
        engineManager.Initialize(1080, 720, L"Test");
        Scene* scene = SceneManager::GetSceneWithName("Default");
        scene->RegisterSystem<RenderSystem>();
        scene->RegisterSystem<TransformSystem>();
        scene->RegisterSystem<StateMachineSystem>();
        scene->RegisterSystem<PhysicSystem>();
        scene->RegisterSystem<CollisionSystem>();
        scene->RegisterSystem<CameraSystem>();
        scene->RegisterSystem<ForceSystem>();

        Device* pDevice = engineManager.GetDevice();

        Entity* pEntity1 = scene->CreateEntity();
        TransformComponent* pTransform = scene->GetComponentType<TransformComponent>(pEntity1);
        pTransform->transform.SetLocalPosition(XMFLOAT3(0.0f, 0.0f, -25.f));
        CameraComponent* pCamComponent = scene->AddComponent<CameraComponent>(pEntity1);
        EngineManager::GetDevice()->SetMainCamera(&pCamComponent->camera);

        Geometry* pSphere = GeometryFactory::BuildIcosphere(pDevice, 32);
        RessourceManager::AddGeometry("Sphere", pSphere);

        Material* blue = RessourceManager::GetShader("Color")->CreateMaterial();
        blue->SetFloat4("DiffuseAlbedo", { 0.0f, 0.0f, 1.0f, 1.0f });
        RessourceManager::AddMaterial("Blue", blue);

        Entity* sphere1 = scene->CreateEntity();
        MeshRenderer* meshSphere1 = scene->AddComponent<MeshRenderer>(sphere1);
        TransformComponent* pTransformSphere = scene->GetComponentType<TransformComponent>(sphere1);
        pTransformSphere->transform.SetLocalPosition({-5.0f, 0.0f, 0.0f});
        meshSphere1->pGeometry = RessourceManager::GetGeometry("Sphere");
        meshSphere1->pMaterial = RessourceManager::GetMaterial("Blue");

        Entity* sphere2 = scene->CreateEntity();
        MeshRenderer* meshSphere2 = scene->AddComponent<MeshRenderer>(sphere2);
        meshSphere2->pGeometry = RessourceManager::GetGeometry("Sphere");
        
        engineManager.Run();
    }
};

#endif