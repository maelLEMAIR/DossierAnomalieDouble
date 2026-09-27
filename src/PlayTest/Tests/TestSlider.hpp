#ifndef TEST_SLIDER_HPP_DEFINED
#define TEST_SLIDER_HPP_DEFINED

#include "pch.h"
#include "Test.h"
#include "../../Render/Generic/Render.h"
#include "components.h"
#include "systems.h"

    
class StateUISlider : public StateGlobal
{
public:
    void OnButtonPressed() override
    {
        if (pSlider != nullptr && pTarget != nullptr)
        {

            float scale = pSlider->currentValue;
            pTarget->transform.SetWorldScale({ scale, scale, scale });
            std::cout << "Valeur du slider : " << scale << std::endl;
        }
    }

    TransformComponent* pTarget = nullptr;
    UISliderComponent* pSlider = nullptr;
};

class TestSlider : public Test
{
public:
    static void Run()
    {
        EngineManager engineManager;
        engineManager.Initialize(1080, 720, L"Test UI Slider");
        Scene* scene = SceneManager::GetSceneWithName("Default");

        Device* pDevice = engineManager.GetDevice();
         
        Geometry* pCubeGeo = GeometryFactory::BuildCube(pDevice);
        Entity* Cube = scene->CreateEntity();
        TransformComponent* pCubeTransform = scene->GetComponentType<TransformComponent>(Cube);
        pCubeTransform->transform.SetWorldPosition({ 0.0f, -200.0f, 0.0f });
        pCubeTransform->transform.SetWorldScale({ 1.0f, 1.0f, 1.0f });
        MeshRenderer* pCubeMesh = scene->AddComponent<MeshRenderer>(Cube);
        pCubeMesh->pGeometry = pCubeGeo;
         
        Entity* Camera = scene->CreateEntity();
        TransformComponent* pTransform = scene->GetComponentType<TransformComponent>(Camera);
        pTransform->transform.SetWorldPosition(XMFLOAT3(0.0f, 0.0f, -8.0f));
        CameraComponent* pCamComponent = scene->AddComponent<CameraComponent>(Camera);
        pCamComponent->camera = scene->pCamera->camera;
        EngineManager::GetDevice()->SetMainCamera(&pCamComponent->camera);
         
        Entity* sliderBg = scene->CreateEntity();
        SpriteComponent* pSpriteBg = scene->AddComponent<SpriteComponent>(sliderBg);
        TransformComponent* transformBg = scene->GetComponentType<TransformComponent>(sliderBg);
        transformBg->transform.SetWorldPosition({ 0.0f, -2.0f, 0.0f });
        transformBg->transform.SetWorldScale(1.f);

        pSpriteBg->width    = 400;   
        pSpriteBg->height   = 20;
        pSpriteBg->pRect    = SpriteFactory::BuildRectangle(pDevice, pSpriteBg->width, pSpriteBg->height);
        pSpriteBg->SetTexture(pDevice->CreateTexture(L"../../res/Textures/Button/buttonRouge.dds"));  
        
        Entity* sliderKnob = scene->CreateEntity();
        SpriteComponent* pSpriteKnob = scene->AddComponent<SpriteComponent>(sliderKnob);
        TransformComponent* transformKnob = scene->GetComponentType<TransformComponent>(sliderKnob);
        transformKnob->transform.SetWorldScale(1.f);

        pSpriteKnob->width  = 30;  
        pSpriteKnob->height = 50;
        pSpriteKnob->pRect  = SpriteFactory::BuildRectangle(pDevice, pSpriteKnob->width, pSpriteKnob->height);
        pSpriteKnob->SetTexture(pDevice->CreateTexture(L"../../res/Textures/Button/buttonRouge.dds"));
         
        UISliderComponent* sliderComp = scene->AddComponent<UISliderComponent>(sliderBg);
        sliderComp->minValue = 0.1f;
        sliderComp->maxValue = 3.0f;
        sliderComp->currentValue = 1.0f;
        sliderComp->pKnobEntity = sliderKnob;  

        StateUISlider* stateGlobal = new StateUISlider;
        stateGlobal->pSlider = sliderComp;

        StateMachineComponent* pSm = scene->AddComponent<StateMachineComponent>(sliderBg);
        pSm->SetStateGlobal(stateGlobal);

        engineManager.Run();
    }
};

#endif