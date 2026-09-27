#ifndef TEST_TEXTURE_H_DEFINED
#define TEST_TEXTURE_H_DEFINED

#include "Render/Generic/Render.h"
#include "Test.h"

#include "../pch.h"

class TestTexture : public Test
{
public: 
    static void Run()
    {
        EngineManager engineManager;
        engineManager.Initialize(1080, 720, L"Test");
        Scene* scene = SceneManager::GetSceneWithName("Default");
        scene->RegisterSystem<LightSystem>();

        Device* pDevice = engineManager.GetDevice();

        TransformComponent* pCameraTransform = scene->GetComponentType<TransformComponent>(scene->pCamera->GetOwner());
        pCameraTransform->transform.SetWorldPosition({ 0.0f, 5.0f, -3.0f });
        pCameraTransform->transform.LookAt({ 0.0f, 0.0f, 0.0f });

        Material* blue = RessourceManager::GetShader("Color")->CreateMaterial();
        blue->SetFloat4("DiffuseAlbedo", XMFLOAT4(1.0f, 0.5f, 0.2f, 1.0f));
        blue->SetFloat("Roughness", 0.5f);

        Shader* litTextured = ShaderFactory::CreateLitTextured(pDevice);
        RessourceManager::AddShader("Lit Texture", litTextured);

        Material* litBrick = litTextured->CreateMaterial();
        Texture* brickAlbedo = pDevice->CreateTexture(L"../../res/Textures/Bricks/Albedo.dds");
        litBrick->SetTexture("Albedo", brickAlbedo);
        Texture* brickRoughness = pDevice->CreateTexture(L"../../res/Textures/Bricks/Roughness.dds");
        litBrick->SetTexture("Roughness", brickRoughness);
        Texture* brickNormal = pDevice->CreateTexture(L"../../res/Textures/Bricks/NormalDX.dds");
        litBrick->SetTexture("Normal", brickNormal);
        Texture* brickAmbient = pDevice->CreateTexture(L"../../res/Textures/Bricks/Ambient.dds");
        litBrick->SetTexture("Ambient", brickAmbient);
        RessourceManager::AddMaterial("Lit Material Brick", litBrick);

        Entity* pEntity = scene->CreateEntity();
        MeshRenderer* mesh = scene->AddComponent<MeshRenderer>(pEntity);
        mesh->pGeometry = GeometryFactory::BuildCylinder(pDevice, 8);
        mesh->pMaterial = litBrick;
        //mesh->pMaterial = blue;

        scene->RegisterSystem<RenderSystem>();
           
        engineManager.Run();
    }
};

#endif