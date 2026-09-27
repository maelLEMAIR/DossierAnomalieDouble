#ifndef TEST_BUTTON_HPP_DEFINED
#define TEST_BUTTON_HPP_DEFINED


#include "pch.h"
#include "Test.h"

#include "../../Render/Generic/Render.h"

class StateButton : public StateGlobal
{
public:
    void OnButtonUp() override
    {
        std::cout << "caca" << std::endl;
    }

    void OnButtonPressed() override
    {
        std::cout << "kaka" << std::endl;
    }

    void OnButtonDown() override
    {
        std::cout << "quaqua" << std::endl;
    }

    void OnHoveredEnter() override
    {
        pTarget->transform.SetWorldPosition({ pTarget->transform.GetWorldPosition().x, pTarget->transform.GetWorldPosition().y, -1.0f });
    }

    void OnHoveredExit() override
    {
        pTarget->transform.SetWorldPosition({ pTarget->transform.GetWorldPosition().x, pTarget->transform.GetWorldPosition().y, 0.0f });
    }

    TransformComponent* pTarget;
};

class TestButton : public Test
{
public:
    static void Run()
    {
        EngineManager engineManager;
        engineManager.Initialize(1080, 720, L"Test");
        Scene* scene = SceneManager::GetSceneWithName("Default");


        Device* pDevice = engineManager.GetDevice();

        Geometry* pCubeGeo = GeometryFactory::BuildCube(pDevice);

        Entity* sprite = scene->CreateEntity();
        Entity* Cube = scene->CreateEntity();
        Entity* Camera = scene->CreateEntity();

        TransformComponent* pTransform = scene->GetComponentType<TransformComponent>(Camera);
        pTransform->transform.SetWorldPosition(XMFLOAT3(0.0f, 0.0f, -5.0f));
        CameraComponent* pCamComponent = scene->AddComponent<CameraComponent>(Camera);
        pCamComponent->camera = scene->pCamera->camera;
        EngineManager::GetDevice()->SetMainCamera(&pCamComponent->camera);

        TransformComponent* pCubeTransform = scene->GetComponentType<TransformComponent>(Cube);
        pCubeTransform->transform.SetWorldPosition({ 0.0f, 0.0, 0.0f });
        pCubeTransform->transform.SetWorldScale({ 0.25f, 1.5f, 1.0f });
        MeshRenderer* pCubeMesh = scene->AddComponent<MeshRenderer>(Cube);
        pCubeMesh->pGeometry = pCubeGeo;

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        SpriteComponent* pSprite = scene->AddComponent<SpriteComponent>(sprite);

        TransformComponent* transformSprite = scene->GetComponentType<TransformComponent>(sprite);
        transformSprite->transform.SetWorldPosition({ 0.0f , 0.0f, 0.0f });
        transformSprite->transform.SetLocalRotation({ 0.0f, 0.0f, 0.0f });
        transformSprite->transform.SetWorldScale(1.f);

        pSprite->width = 50;
        pSprite->height = 300;
        pSprite->pRect = SpriteFactory::BuildRectangle(pDevice, pSprite->width, pSprite->height);

        UIButtonComponent* button = scene->AddComponent<UIButtonComponent>(sprite);

        StateButton* stateGlobal = new StateButton;
        stateGlobal->pTarget = pCubeTransform;
        StateMachineComponent* pSm = scene->AddComponent<StateMachineComponent>(sprite);
        pSm->SetStateGlobal(stateGlobal);

        pSprite->SetTexture(pDevice->CreateTexture(L"../../res/Textures/Button/buttonRouge.dds"));

        engineManager.Run();
    }
};

#endif