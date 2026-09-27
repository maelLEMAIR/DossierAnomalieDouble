#ifndef TEST_UI_H_DEFINED
#define TEST_UI_H_DEFINED

#include "pch.h"
#include "Test.h"

#include "../../Render/Generic/Render.h"

class StateUIButton : public StateGlobal
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

    TransformComponent* pTarget;
};

class TestUI : public Test
{
public: 
    static void Run()
    {
         EngineManager engineManager;
         engineManager.Initialize(1080, 720, L"Test");
         Scene* scene = SceneManager::GetSceneWithName("Default");
        
         scene->RegisterSystem<UIButtonSystem>();

         Device* pDevice = engineManager.GetDevice();
        
         /*Entity* text = scene->CreateEntity();
         TextComponent* pText = scene->AddComponent<TextComponent>(text);
         pText->SetText("RUBSDVolbhevgsldfvbBSDLKBQQQQQQQQQ");
         TransformComponent* transform = scene->GetComponentType<TransformComponent>(text);
         transform->transform.SetWorldPosition({-500.0f, 0.0f, 0.0f });
         transform->transform.SetLocalRotation({0.0f, 0.0f, 0.0f});
         transform->transform.SetWorldScale(1.f);*/
        


         Entity* sprite = scene->CreateEntity();
         SpriteComponent* pSprite = scene->AddComponent<SpriteComponent>(sprite);

         TransformComponent* transformSprite = scene->GetComponentType<TransformComponent>(sprite);
         transformSprite->transform.SetWorldPosition({ 0.0f , 0.0f, 0.0f });
         transformSprite->transform.SetLocalRotation({ 0.0f, 0.0f, 0.0f });
         transformSprite->transform.SetWorldScale(1.f);

         pSprite->width = 100;
         pSprite->height = 100;
         pSprite->pRect = SpriteFactory::BuildRectangle(pDevice, pSprite->width, pSprite->height);

         UIButtonComponent* button = scene->AddComponent<UIButtonComponent>(sprite);

         StateMachineComponent* pSm = scene->AddComponent<StateMachineComponent>(sprite);
         pSm->SetStateGlobal(new StateUIButton);

         button->textureNormal = pDevice->CreateTexture(L"../../res/Textures/Button/buttonRouge.dds");
         button->textureHover = pDevice->CreateTexture(L"../../res/Textures/Button/buttonVert.dds");

         


         engineManager.Run();
    }
};

#endif