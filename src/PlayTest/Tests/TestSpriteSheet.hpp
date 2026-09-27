#ifndef TEST_SPRITE_SHEET_HPP_DEFINED
#define TEST_SPRITE_SHEET_HPP_DEFINED

#include "pch.h"
#include "Test.h"

#include "../../Render/Generic/Render.h"
#include "Engine/Components/MeshRenderer.h"

class TestSpriteSheet : public Test
{
public:
    static void Run()
    {
        EngineManager engineManager;
        engineManager.Initialize(1080, 720, L"Test SpriteSheet 2D et 3D");
        Scene* scene = SceneManager::GetSceneWithName("Default");

        Device* pDevice = engineManager.GetDevice();

        Entity* entity2D = scene->CreateEntity();
        entity2D->name = "AnimatedSprite2D";

        TransformComponent* transform2D = scene->AddComponent<TransformComponent>(entity2D);
        transform2D->transform.SetWorldPosition({ -2.0f, 0.0f, 0.0f });

        SpriteComponent* sprite = scene->AddComponent<SpriteComponent>(entity2D);
        sprite->height  = 100;
        sprite->width   = 100;
        sprite->pRect   = SpriteFactory::BuildRectangle(pDevice, sprite->width, sprite->height);

        SpriteSheetAnimatorComponent* pAnim2D = scene->AddComponent<SpriteSheetAnimatorComponent>(entity2D);
        pAnim2D->totalFrames = 6;
        pAnim2D->timePerFrames = 0.20f;
        pAnim2D->elapsedTime = 0.0f;
        pAnim2D->currentFrame = 0;
        pAnim2D->isPlaying = true;

        pAnim2D->SetSpriteSheet(L"test");

        if (pAnim2D->vTextures.empty() == false)
        {
            sprite->SetTexture(pAnim2D->vTextures[0]);
            pAnim2D->pTargetUiMaterial = sprite->pMaterial;
            pAnim2D->textureParameterName = "Image";
        }
        else
        {
        }

        Entity* entity3D = scene->CreateEntity();
        entity3D->name = "AnimatedCube3D";

        TransformComponent* transform3D = scene->AddComponent<TransformComponent>(entity3D);
        transform3D->transform.SetWorldPosition({ 2.0f, 0.0f, 0.0f });

        MeshRenderer* meshRenderer = scene->AddComponent<MeshRenderer>(entity3D);
        meshRenderer->pGeometry = GeometryFactory::BuildCube(pDevice);

        Shader* litTextured = ShaderFactory::CreateLitTextured(pDevice);
        Material* cubeMaterial = litTextured->CreateMaterial();
        meshRenderer->pMaterial = cubeMaterial;

        SpriteSheetAnimatorComponent* pAnim3D = scene->AddComponent<SpriteSheetAnimatorComponent>(entity3D);
        pAnim3D->totalFrames = 6;
        pAnim3D->timePerFrames = 0.20f;
        pAnim3D->elapsedTime = 0.0f;
        pAnim3D->currentFrame = 0;
        pAnim3D->isPlaying = true;

        pAnim3D->SetSpriteSheet(L"test");

        if (pAnim3D->vTextures.empty() == false)
        {
            cubeMaterial->SetTexture("Albedo", pAnim3D->vTextures[0]);
            pAnim3D->pTargetMaterial = cubeMaterial;
            pAnim3D->textureParameterName = "Albedo";
        }
        else
        {
        }

        Camera cam;
        XMFLOAT3 pos = XMFLOAT3(0.0f, 0.0f, -5.0f);
        cam.SetPos(pos);
        XMFLOAT3 target = XMFLOAT3(0.0f, 0.0f, 0.0f);
        cam.LookAt(target);
        pDevice->SetMainCamera(&cam);

        engineManager.Run();
    }
};

#endif