#ifndef TEST_INPUT_TEXT_HPP_DEFINED
#define TEST_INPUT_TEXT_HPP_DEFINED

#include "pch.h"
#include "Test.h"
#include "../../Render/Generic/Render.h"
 
#include "../../Engine/Components/UITextInputComponent.h"
#include "../../Engine/Systems/UITextInputSystem.h"

class InputTextState : public StateGlobal
{
public:
    void OnButtonDown() override
    { 
        if (m_pOwner != nullptr && m_pOwner->pScene != nullptr)
        {
            TextComponent* pText = m_pOwner->pScene->GetComponentType<TextComponent>(m_pOwner);
            if (pText != nullptr)
            {
                pText->SetColor(1.0f, 0.0f, 0.0f);
            }
        }
    }

    void OnButtonUp() override
    { 
        if (m_pOwner != nullptr && m_pOwner->pScene != nullptr)
        {
            TextComponent* pText = m_pOwner->pScene->GetComponentType<TextComponent>(m_pOwner);
            if (pText != nullptr)
            {
                pText->SetColor(1.0f, 1.0f, 1.0f);
            }
        }
    }

    void OnButtonPressed() override
    { 
        if (m_pOwner != nullptr && m_pOwner->pScene != nullptr)
        {
            UITextInputComponent* pInput = m_pOwner->pScene->GetComponentType<UITextInputComponent>(m_pOwner);
            TextComponent* pText = m_pOwner->pScene->GetComponentType<TextComponent>(m_pOwner);

            if (pInput != nullptr && pText != nullptr)
            {
                std::cout << "Validation ! Le texte recupere est : " << pInput->currentText << std::endl;
                pText->SetColor(0.0f, 1.0f, 0.0f);
            }
        }
    }

    void OnHoveredEnter() override
    { 
        if (m_pOwner != nullptr && m_pOwner->pScene != nullptr)
        {
            TextComponent* pText = m_pOwner->pScene->GetComponentType<TextComponent>(m_pOwner);
            UITextInputComponent* pInput = m_pOwner->pScene->GetComponentType<UITextInputComponent>(m_pOwner);

            if (pText != nullptr && pInput != nullptr)
            {
                if (pInput->isFocused == false)
                {
                    pText->SetColor(1.0f, 1.0f, 0.0f);
                }
            }
        }
    }

    void OnHoveredExit() override
    { 
        if (m_pOwner != nullptr && m_pOwner->pScene != nullptr)
        {
            TextComponent* pText = m_pOwner->pScene->GetComponentType<TextComponent>(m_pOwner);
            UITextInputComponent* pInput = m_pOwner->pScene->GetComponentType<UITextInputComponent>(m_pOwner);

            if (pText != nullptr && pInput != nullptr)
            {
                if (pInput->isFocused == false)
                {
                    pText->SetColor(1.0f, 1.0f, 1.0f);
                }
            }
        }
    }
};


class TestInputText : public Test
{
public:
    static void Run()
    {
        EngineManager engineManager;
        engineManager.Initialize(1080, 720, L"Test Input Text");
        Scene* scene = SceneManager::GetSceneWithName("Default");
         
        scene->RegisterSystem<UITextInputSystem>();

        Device* pDevice = engineManager.GetDevice();

        Entity* sprite = scene->CreateEntity();
        Entity* Camera = scene->CreateEntity();
         
        TransformComponent* pTransformCam = scene->GetComponentType<TransformComponent>(Camera);
        pTransformCam->transform.SetWorldPosition({ 0.0f, 0.0f, -5.0f });
        CameraComponent* pCamComponent = scene->AddComponent<CameraComponent>(Camera);
        pCamComponent->camera = scene->pCamera->camera;
        EngineManager::GetDevice()->SetMainCamera(&pCamComponent->camera);
         
        SpriteComponent* pSprite = scene->AddComponent<SpriteComponent>(sprite);
        TransformComponent* transformSprite = scene->GetComponentType<TransformComponent>(sprite);
        transformSprite->transform.SetWorldPosition({ 0.0f, 0.0f, 0.0f });
        transformSprite->transform.SetLocalRotation({ 0.0f, 0.0f, 0.0f });
        transformSprite->transform.SetWorldScale(1.f);

        pSprite->width = 400.f;
        pSprite->height = 50.f;
        pSprite->pRect = SpriteFactory::BuildRectangle(pDevice, pSprite->width, pSprite->height);

        TextComponent* pText = scene->AddComponent<TextComponent>(sprite);
        pText->SetText("Cliquez ici pour ecrire...");
        pText->SetColor(1.0f, 1.0f, 1.0f);


        UITextInputComponent* pInput = scene->AddComponent<UITextInputComponent>(sprite);


        InputTextState* stateGlobal = new InputTextState();
        StateMachineComponent* pSm = scene->AddComponent<StateMachineComponent>(sprite);
        pSm->SetStateGlobal(stateGlobal);

        engineManager.Run();
    }
};

#endif