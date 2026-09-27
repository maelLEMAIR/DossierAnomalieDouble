#ifndef TEST_DROPDOWN_HPP_DEFINED
#define TEST_DROPDOWN_HPP_DEFINED

#include "pch.h"
#include "Test.h"
#include "../../Render/Generic/Render.h"
#include <string>
#include <vector>
#include <iostream>
 
class StateUIDropdown : public StateGlobal
{
public:
    void OnButtonDown() override
    {
        if (pDropdown != nullptr)
        { 
            for (int i = 0; i < optionEntities.size(); i++)
            {
                if (optionEntities[i] != nullptr)
                {
                    optionEntities[i]->isActive = pDropdown->isOpen;
                }
            }

            // On affiche ou masque les Textes des options
            for (int i = 0; i < textEntities.size(); i++)
            {
                if (textEntities[i] != nullptr)
                {
                    textEntities[i]->isActive = pDropdown->isOpen;
                }
            }

            if (pDropdown->isOpen == true)
            {
                std::cout << "Dropdown Ouvert" << std::endl;
            }
            else
            {
                std::cout << "Dropdown Ferme" << std::endl;
            }
        }
    }

    void OnButtonPressed() override
    { 
        if (pDropdown != nullptr && pText != nullptr)
        {
            std::string selection = pDropdown->options[pDropdown->selectedIndex];
            std::cout << "Option selectionnee : " << selection << std::endl;
             
            pText->SetText(selection);
             
            for (int i = 0; i < optionEntities.size(); i++)
            {
                if (optionEntities[i] != nullptr)
                {
                    optionEntities[i]->isActive = false;
                }
            }
             
            for (int i = 0; i < textEntities.size(); i++)
            {
                if (textEntities[i] != nullptr)
                {
                    textEntities[i]->isActive = false;
                }
            }
        }
    }

    UIDropdownComponent* pDropdown = nullptr;
    TextComponent* pText = nullptr;

    std::vector<Entity*> optionEntities;  
    std::vector<Entity*> textEntities;     
};

class TestDropdown : public Test
{
public:
    static void Run()
    {
        EngineManager engineManager;
        engineManager.Initialize(1080, 720, L"Test UI Dropdown");
        Scene* scene = SceneManager::GetSceneWithName("Default");

        scene->RegisterSystem<UIDropdownSystem>();
        Device* pDevice = engineManager.GetDevice();
         
        Entity* Camera = scene->CreateEntity();
        TransformComponent* pTransform = scene->GetComponentType<TransformComponent>(Camera);
        pTransform->transform.SetWorldPosition(XMFLOAT3(0.0f, 0.0f, -8.0f));
        CameraComponent* pCamComponent = scene->AddComponent<CameraComponent>(Camera);
        pCamComponent->camera = scene->pCamera->camera;
        EngineManager::GetDevice()->SetMainCamera(&pCamComponent->camera);
         
        Entity* dropdownEntity = scene->CreateEntity();

        SpriteComponent* pSprite = scene->AddComponent<SpriteComponent>(dropdownEntity);
        TransformComponent* transformDropdown = scene->GetComponentType<TransformComponent>(dropdownEntity);
         
        transformDropdown->transform.SetWorldPosition({ 0.0f, 2.0f, 0.0f });
        transformDropdown->transform.SetWorldScale(1.f);

        pSprite->width  = 200;
        pSprite->height = 50;
        pSprite->pRect  = SpriteFactory::BuildRectangle(pDevice, pSprite->width, pSprite->height);
        pSprite->SetTexture(pDevice->CreateTexture(L"../../res/Textures/Button/buttonRouge.dds"));
         
        TextComponent* pTextComponent = scene->AddComponent<TextComponent>(dropdownEntity);
        pTextComponent->SetText("1920x1080");
         
        UIDropdownComponent* dropdownComp = scene->AddComponent<UIDropdownComponent>(dropdownEntity);
        dropdownComp->options.push_back("1920x1080");
        dropdownComp->options.push_back("2560x1440");
        dropdownComp->options.push_back("3840x2160");
        dropdownComp->optionHeight  = 30; 
        dropdownComp->selectedIndex = 0;
         
        std::vector<Entity*> spawnedOptions;
        std::vector<Entity*> spawnedTexts;

        float startX = transformDropdown->transform.GetWorldPosition().x;
        float startY = transformDropdown->transform.GetWorldPosition().y;

        float gapTop = 5.0f;      
        float gapOptions = 2.0f;  

        float firstOptionY = startY + (float)pSprite->height + gapTop;

        float textOffsetX = 10.0f;  
        float textOffsetY = 5.0f;   

        for (int i = 0; i < dropdownComp->options.size(); i++)
        {
            Entity* optEntity = scene->CreateEntity();
            optEntity->isActive = false; 

            SpriteComponent* optSprite = scene->AddComponent<SpriteComponent>(optEntity);
            TransformComponent* optTransform = scene->GetComponentType<TransformComponent>(optEntity);

            optTransform->transform.SetWorldScale(1.f);

            optSprite->width    = pSprite->width;
            optSprite->height   = dropdownComp->optionHeight;
            optSprite->pRect    = SpriteFactory::BuildRectangle(pDevice, optSprite->width, optSprite->height);
            optSprite->SetTexture(pDevice->CreateTexture(L"../../res/Textures/Button/buttonRouge.dds"));

            Entity* textEntity = scene->CreateEntity();
            textEntity->isActive = false;

            TransformComponent* textTransform = scene->GetComponentType<TransformComponent>(textEntity);
            TextComponent* optText = scene->AddComponent<TextComponent>(textEntity);
            optText->SetText(dropdownComp->options[i]);

            spawnedOptions.push_back(optEntity);
            spawnedTexts.push_back(textEntity);

            float currentY = firstOptionY + ((float)i * ((float)dropdownComp->optionHeight + gapOptions));

            optTransform->transform.SetWorldPosition({ startX, -currentY, -0.1f });
            textTransform->transform.SetWorldPosition({ startX + textOffsetX, currentY + textOffsetY, -0.15f });
        }

        StateUIDropdown* stateGlobal = new StateUIDropdown;
        stateGlobal->pDropdown = dropdownComp;
        stateGlobal->pText = pTextComponent;
        stateGlobal->optionEntities = spawnedOptions;
        stateGlobal->textEntities = spawnedTexts;

        StateMachineComponent* pSm = scene->AddComponent<StateMachineComponent>(dropdownEntity);
        pSm->SetStateGlobal(stateGlobal);

        engineManager.Run();
    }
};

#endif