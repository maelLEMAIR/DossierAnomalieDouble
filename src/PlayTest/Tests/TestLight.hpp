#ifndef TEST_LIGHT_H_DEFINED
#define TEST_LIGHT_H_DEFINED

#include "Render/Generic/Render.h"
#include "Test.h"

#include "../pch.h"

class TestLight : public Test
{
public: 
    static void Run()
    {
        EngineManager engineManager;
        engineManager.Initialize(1080, 720, L"Test");
        Scene* scene = SceneManager::GetSceneWithName("Default");
        scene->RegisterSystem<LightSystem>();

        Device* pDevice = engineManager.GetDevice();

        Entity* pEntity = scene->CreateEntity();
        MeshRenderer* mesh = scene->AddComponent<MeshRenderer>(pEntity);
        mesh->pGeometry = GeometryFactory::BuildCylinder(pDevice, 8);

        Entity* pEntity1 = scene->CreateEntity();
        TransformComponent* transform = scene->GetComponentType<TransformComponent>(pEntity1);
        transform->transform.SetWorldPosition({ 8.0f, 0.0f, 0.0f });
        MeshRenderer* mesh1 = scene->AddComponent<MeshRenderer>(pEntity1);
        mesh1->pGeometry = GeometryFactory::BuildCylinder(pDevice, 8);

        scene->RegisterSystem<RenderSystem>();
           
        engineManager.Run();
    }
};

#endif