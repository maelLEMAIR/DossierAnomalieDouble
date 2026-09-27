#ifndef TEST_SCENE_OR_H_DEFINED
#define TEST_SCENE_OR_H_DEFINED

#include "../Render/Camera.h"
#include "Test.h"
#include "../Render/Generic/Device.h"
#include "../Render/Generic/Geometry.h"
#include "../Render/Generic/Material.h"
#include "../Render/Generic/Shader.h"
#include "../Render/Generic/ShaderFormat.h"

#include "../Render/Window.h"
#include "../Render/Utils/MathHelper.h"
#include "../Render/GeometryFactory.hpp"
#include "../Render/ShaderFactory.hpp"

#include "../pch.h"

class TestSceneOr : public Test
{
public: 
    static void Run()
    {
        EngineManager engineManager;
        engineManager.Initialize(1080, 720, L"Test");
        Scene* scene = SceneManager::GetSceneWithName("Default");
        scene->RegisterSystem<LightSystem>();

        Device* pDevice = engineManager.GetDevice();

        Entity* pCamera = scene->CreateEntity();
        pCamera->name = "Camera";
        TransformComponent* pTransformCamera = scene->AddComponent<TransformComponent>(pCamera);
        pTransformCamera->transform.SetWorldPosition(XMFLOAT3(0.0f, 0.0f, -3.0f));
        CameraComponent* pCamComponent = scene->AddComponent<CameraComponent>(pCamera);
        pDevice->SetMainCamera(&pCamComponent->camera);

        Material* blue = RessourceManager::GetShader("Color")->CreateMaterial();
        blue->SetFloat4("DiffuseAlbedo", XMFLOAT4(1.0f, 0.5f, 0.2f, 1.0f));
        blue->SetFloat("Roughness", 0.5f);

        Entity* pEntity = scene->CreateEntity();
        MeshRenderer* mesh = scene->AddComponent<MeshRenderer>(pEntity);
        mesh->pGeometry = GeometryFactory::BuildCylinder(pDevice, 8);
        mesh->pMaterial = blue;



        

        Entity* lightEntity = scene->CreateEntity();
        LightComponent* light = scene->AddComponent<LightComponent>(lightEntity);
        light->m_position = {4.0f, 0.0f, -2.0f};
        light->m_type = LightType::Spot;
        light->m_direction = { -0.5f, 0.0f, 0.5f };
        light->m_falloffStart = 1.0f;
        light->m_falloffEnd = 10.0f;
        light->m_strength = 1.5f;
        light->m_spotPower = 3.0f;

        LightSystem::UpdateLight();
           
        engineManager.Run();
    }
};

#endif