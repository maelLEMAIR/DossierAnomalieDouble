#ifndef TEST_UI_TOGGLE_HPP_DEFINED
#define TEST_UI_TOGGLE_HPP_DEFINED

#include "pch.h"
#include "Test.h"

#include "../../Render/Generic/Render.h"
#include "components.h"
#include "systems.h"


class StateUIToggle : public StateGlobal
{
public:
    void OnButtonDown() override
    {

        if (pToggle != nullptr && pTarget != nullptr)
        {
            if (pToggle->isToggled == true)
            {
                std::cout << "Toggle: ACTIF (ON)" << std::endl;
                pTarget->transform.SetWorldPosition({ pTarget->transform.GetWorldPosition().x, 1.0f, pTarget->transform.GetWorldPosition().z });
            }
            else
            {
                std::cout << "Toggle: INACTIF (OFF)" << std::endl;
                pTarget->transform.SetWorldPosition({ pTarget->transform.GetWorldPosition().x, 0.0f, pTarget->transform.GetWorldPosition().z });
            }
        }
    }

    void OnHoveredEnter() override
    {
        std::cout << "Souris sur le Toggle" << std::endl;
    }

    void OnHoveredExit() override
    {
        std::cout << "Souris quitte le Toggle" << std::endl;
    }

    TransformComponent* pTarget = nullptr;
    UIToggleComponent* pToggle = nullptr; 
};


class TestUIToggle : public Test
{
public:
    static void Run()
    {
        EngineManager engineManager;
        engineManager.Initialize(1080, 720, L"Test UI Toggle");
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
        pCubeTransform->transform.SetWorldPosition({ 0.0f, 0.0f, 0.0f });
        pCubeTransform->transform.SetWorldScale({ 0.5f, 0.5f, 0.5f });
        MeshRenderer* pCubeMesh = scene->AddComponent<MeshRenderer>(Cube);
        pCubeMesh->pGeometry = pCubeGeo;

        SpriteComponent* pSprite = scene->AddComponent<SpriteComponent>(sprite);

        TransformComponent* transformSprite = scene->GetComponentType<TransformComponent>(sprite);
        transformSprite->transform.SetWorldPosition({ 0.0f , 0.0f, 0.0f });
        transformSprite->transform.SetLocalRotation({ 0.0f, 0.0f, 0.0f });
        transformSprite->transform.SetWorldScale(1.f);


        pSprite->width  = 100;
        pSprite->height = 100;
        pSprite->pRect  = SpriteFactory::BuildRectangle(pDevice, pSprite->width, pSprite->height);


        UIToggleComponent* toggle = scene->AddComponent<UIToggleComponent>(sprite);


        StateUIToggle* stateGlobal = new StateUIToggle;
        stateGlobal->pTarget = pCubeTransform;
        stateGlobal->pToggle = toggle; 

        StateMachineComponent* pSm = scene->AddComponent<StateMachineComponent>(sprite);
        pSm->SetStateGlobal(stateGlobal);


        pSprite->SetTexture(pDevice->CreateTexture(L"../../res/Textures/Button/buttonRouge.dds"));

        engineManager.Run();
    }
};

#endif