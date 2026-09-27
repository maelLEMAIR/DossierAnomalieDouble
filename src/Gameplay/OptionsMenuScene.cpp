#include "OptionsMenuScene.h"
#include "Engine/EngineManager.h"
#include "Engine/Entity.h"
#include "Engine/components.h"
#include "Engine/systems.h"
#include "Render/Generic/Render.h"
#include <iostream>
#include <MapLoader.h>
 

class StateUISlider : public StateGlobal
{
public:
    void OnButtonPressed() override
    {
        if (pSlider != nullptr)
        {
            float scale = pSlider->currentValue;
             
            float volumeRatio = scale / 100.0f;
             

            std::cout << "Volume modifie a : " << scale << "%" << std::endl;


            EngineManager::GetInstance().GetAudioEngine()->SetMasterVolume(volumeRatio);
        }
    }

    UISliderComponent* pSlider = nullptr;
};

class StateOptionToggle : public StateGlobal
{
public:
    void OnButtonDown() override
    {
        if (pToggle != nullptr)
        {
            Window* pWindow = EngineManager::GetWindow();
            if (pWindow != nullptr)
            {
                pWindow->ToggleFullScreen();
            }

            if (pToggle->isToggled == true)
            {
                std::cout << "Plein ecran : ACTIF" << std::endl;
            }
            else
            {
                std::cout << "Plein ecran : INACTIF" << std::endl;
            }
        }
    }

    UIToggleComponent* pToggle = nullptr;
};


class StateOptionDropdown : public StateGlobal
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

            for (int i = 0; i < textEntities.size(); i++)
            {
                if (textEntities[i] != nullptr)
                {
                    textEntities[i]->isActive = pDropdown->isOpen;
                }
            }
        }
    }

    void OnButtonPressed() override
    {
        if (pDropdown != nullptr)
        {
            if (pText != nullptr)
            {
                std::string selection = pDropdown->options[pDropdown->selectedIndex];
                pText->SetText(selection);
                 
                Window* pWindow = EngineManager::GetWindow();
                if (pWindow != nullptr)
                {
                    if (selection == "720x480")
                    {
                        pWindow->SetSize(720 , 480);
                    }
                    else if (selection == "1280x720")
                    {
                        pWindow->SetSize(1280 , 720);
                    }
                    else if (selection == "1920x1080")
                    {
                        pWindow->SetSize(1920, 1080);
                    }
                }
            }
             
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


class StateBtnRetour : public StateGlobal
{
public:
    void OnButtonPressed() override
    {
        if (pBtn != nullptr)
        {
            pBtn->isPressed = true;
        }
    }

    UIButtonComponent* pBtn = nullptr;
};



void OptionsMenuScene::OnInit()
{
    Device* pDevice = EngineManager::GetDevice();
    float startY = 200.0f;

    
    MapLoader::LoadMap("../../res/JSON/map_menuCredits.json", pDevice, *this, nullptr, "Credits");

    ////////////////////////////////////////////////////////////////////////////////////////////////
    Entity* titleEntity = this->CreateEntity();
    TransformComponent* titleTransform = this->AddComponent<TransformComponent>(titleEntity);
    titleTransform->transform.SetLocalPosition(XMFLOAT3(-100.0f, startY + 100.0f, 0.0f));

    TextComponent* titleText = this->AddComponent<TextComponent>(titleEntity);
    titleText->SetText("OPTIONS");

    ////////////////////////////////////////////////////////////////////////////////////////////////
    Entity* sliderBg = CreateEntity();
    SpriteComponent* pSpriteBg = AddComponent<SpriteComponent>(sliderBg);
    TransformComponent* transformBg = GetComponentType<TransformComponent>(sliderBg);
    transformBg->transform.SetWorldPosition({ 0.0f, 100.0f, 0.0f });
    transformBg->transform.SetWorldScale(1.f);

    pSpriteBg->width = 400;
    pSpriteBg->height = 20;
    pSpriteBg->pRect = SpriteFactory::BuildRectangle(pDevice, pSpriteBg->width, pSpriteBg->height);
    pSpriteBg->SetTexture(pDevice->CreateTexture(L"../../res/Textures/Button/buttonRouge.dds"));

    Entity* sliderKnob = CreateEntity();
    SpriteComponent* pSpriteKnob = AddComponent<SpriteComponent>(sliderKnob);
    TransformComponent* transformKnob = GetComponentType<TransformComponent>(sliderKnob);
    transformKnob->transform.SetWorldScale(1.f);

    pSpriteKnob->width = 30;
    pSpriteKnob->height = 50;
    pSpriteKnob->pRect = SpriteFactory::BuildRectangle(pDevice, pSpriteKnob->width, pSpriteKnob->height);
    pSpriteKnob->SetTexture(pDevice->CreateTexture(L"../../res/Textures/Button/buttonRouge.dds"));

    UISliderComponent* sliderComp = AddComponent<UISliderComponent>(sliderBg);
    sliderComp->minValue = 0.f;
    sliderComp->maxValue = 100.0f;
    sliderComp->currentValue = 50.0f;
    sliderComp->pKnobEntity = sliderKnob;

    StateUISlider* stateGlobal = new StateUISlider;
    stateGlobal->pSlider = sliderComp;

    StateMachineComponent* pSm = AddComponent<StateMachineComponent>(sliderBg);
    pSm->SetStateGlobal(stateGlobal);
     
    ////////////////////////////////////////////////////////////////////////////////////////////////

    m_pToggleFullscreen = this->CreateEntity();
    TransformComponent* fsTransform = this->AddComponent<TransformComponent>(m_pToggleFullscreen);
    fsTransform->transform.SetLocalPosition(XMFLOAT3(0.0f, 300.0f, 0.0f));
    fsTransform->transform.SetWorldScale(1.0f);

    SpriteComponent* fsSprite = this->AddComponent<SpriteComponent>(m_pToggleFullscreen);
    fsSprite->width = 50;
    fsSprite->height = 50;
    fsSprite->pRect = SpriteFactory::BuildRectangle(pDevice, fsSprite->width, fsSprite->height);
    fsSprite->SetTexture(pDevice->CreateTexture(L"../../res/Textures/Button/buttonRouge.dds"));

    UIToggleComponent* fsToggle = this->AddComponent<UIToggleComponent>(m_pToggleFullscreen);

    StateOptionToggle* stateToggle = new StateOptionToggle;
    stateToggle->pToggle = fsToggle;

    StateMachineComponent* smToggle = this->AddComponent<StateMachineComponent>(m_pToggleFullscreen);
    smToggle->SetStateGlobal(stateToggle);

    ////////////////////////////////////////////////////////////////////////////////////////////////


    m_pDropdownResolution = this->CreateEntity();
    TransformComponent* resTransform = this->AddComponent<TransformComponent>(m_pDropdownResolution);
     
    resTransform->transform.SetWorldPosition({ 0.0f,-150.0f,0.0f });
    resTransform->transform.SetWorldScale(1.0f);

    SpriteComponent* resSprite = this->AddComponent<SpriteComponent>(m_pDropdownResolution);
    resSprite->width = 200;
    resSprite->height = 50;
    resSprite->pRect = SpriteFactory::BuildRectangle(pDevice, resSprite->width, resSprite->height);
    resSprite->SetTexture(pDevice->CreateTexture(L"../../res/Textures/Button/buttonRouge.dds"));

    UIDropdownComponent* dropdownComp = this->AddComponent<UIDropdownComponent>(m_pDropdownResolution);
    dropdownComp->options.push_back("720x480");
    dropdownComp->options.push_back("1280x720");
    dropdownComp->options.push_back("1920x1080");
    dropdownComp->optionHeight  = 30;
    dropdownComp->selectedIndex = 0;
     
    dropdownComp->textOffsetX   = 40.0f;
    dropdownComp->textOffsetY   = 20.0f;
    dropdownComp->gapTop        = 5.0f;
    dropdownComp->gapOptions    = 10.0f;
     
    Entity* mainTextEntity = this->CreateEntity();
    this->AddComponent<TransformComponent>(mainTextEntity);
    TextComponent* pTextComponent = this->AddComponent<TextComponent>(mainTextEntity);
    pTextComponent->SetText("1920x1080");

    dropdownComp->pMainTextEntity = mainTextEntity;

    for (int i = 0; i < dropdownComp->options.size(); i++)
    { 
        Entity* optEntity = this->CreateEntity();
        optEntity->isActive = false;

        SpriteComponent* optSprite = this->AddComponent<SpriteComponent>(optEntity);
        TransformComponent* optTransform = this->AddComponent<TransformComponent>(optEntity);
        optTransform->transform.SetWorldScale(1.0f);

        optSprite->width = resSprite->width;
        optSprite->height = (int)dropdownComp->optionHeight;
        optSprite->pRect = SpriteFactory::BuildRectangle(pDevice, optSprite->width, optSprite->height);
        optSprite->SetTexture(pDevice->CreateTexture(L"../../res/Textures/Button/buttonRouge.dds"));
         
        Entity* textEntity = this->CreateEntity();
        textEntity->isActive = false;

        this->AddComponent<TransformComponent>(textEntity);
        TextComponent* optText = this->AddComponent<TextComponent>(textEntity);
        optText->SetText(dropdownComp->options[i]);
         
        dropdownComp->optionBackgroundEntities.push_back(optEntity);
        dropdownComp->optionTextEntities.push_back(textEntity);

        m_dropdownOptions.push_back(optEntity);
        m_dropdownTexts.push_back(textEntity);
    }

    StateOptionDropdown* stateDropdown = new StateOptionDropdown;
    stateDropdown->pDropdown = dropdownComp;
    stateDropdown->pText = pTextComponent;
    stateDropdown->optionEntities = m_dropdownOptions;
    stateDropdown->textEntities = m_dropdownTexts;

    StateMachineComponent* smDropdown = this->AddComponent<StateMachineComponent>(m_pDropdownResolution);
    smDropdown->SetStateGlobal(stateDropdown);

    ////////////////////////////////////////////////////////////////////////////////////////////////

    m_pBtnRetour = this->CreateEntity();
    TransformComponent* backTransform = this->AddComponent<TransformComponent>(m_pBtnRetour);
    backTransform->transform.SetWorldPosition(XMFLOAT3(-600.0f, -400.0f, 0.0f));
    backTransform->transform.SetWorldScale(1.0f);

    SpriteComponent* btnSprite = this->AddComponent<SpriteComponent>(m_pBtnRetour);
    btnSprite->width = 50;
    btnSprite->height = 50;
    btnSprite->pRect = SpriteFactory::BuildRectangle(pDevice, btnSprite->width, btnSprite->height);
    btnSprite->SetTexture(pDevice->CreateTexture(L"../../res/Textures/Button/buttonRouge.dds"));

    UIButtonComponent* btnRetour = this->AddComponent<UIButtonComponent>(m_pBtnRetour);

    StateBtnRetour* stateBtn = new StateBtnRetour;
    stateBtn->pBtn = btnRetour;

    StateMachineComponent* smBtn = this->AddComponent<StateMachineComponent>(m_pBtnRetour);
    smBtn->SetStateGlobal(stateBtn);


    /////////////////////////////////////////////////////////////////////////////////////////////////

    m_pCamera = pCamera->GetOwner();

    Entity* light = CreateEntity();
    LightComponent* lc = AddComponent<LightComponent>(light);
    XMFLOAT3 pos = {
        0.0f,
        0.0f,
        -2.0f
    };
    XMFLOAT3 color = ToColor(255, 222, 156);
    lc->SetLight(LightType::Spot, 1.0f,
        { 0.0f, 0.0f, 1.0f }, { color.x, color.y, color.z, 0.5f },
        pos, 0.1f, 5.0f);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    TransformComponent* pTransformCamera = AddComponent<TransformComponent>(m_pCamera);
    pTransformCamera->transform.SetWorldPosition(XMFLOAT3(0.0f, 0.0f, -4.0f));
    CameraComponent* pCamComponent = GetComponentType<CameraComponent>(m_pCamera);
    pCamComponent->camera.nearPlane = 0.1f;
    pCamComponent->camera.fov = 0.25f * MathHelper::Pi;
    EngineManager::GetDevice()->SetMainCamera(&pCamComponent->camera);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    m_isInit = true;

}

void OptionsMenuScene::OnUpdate(float dt)
{
    if (m_pBtnRetour != nullptr)
    {
        UIButtonComponent* btn = this->GetComponentType<UIButtonComponent>(m_pBtnRetour);
        if (btn != nullptr)
        {
            if (btn->isPressed == true)
            {
                SceneManager::GetInstance().ChangeCurrentScene("MenuScene");
                btn->isPressed = false;
            }
        }
    }

}