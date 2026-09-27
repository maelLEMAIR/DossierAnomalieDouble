#ifndef MENU_SCENE_CPP_INCLUDED
#define MENU_SCENE_CPP_INCLUDED

#include "MenuScene.h"
#include "MapLoader.h"
#include <Components/UIButtonComponent.h>
#include <Systems/UIButtonSystem.h>
#include "LobbyScene.h"
#include "JoiningScene.h"
#include "OptionsMenuScene.h"
#include "MainScene.h"

#include "MainScene_Serveur.h"
#include "MainScene_Client.h"

enum Button
{
    JOIN,
    CREATE,
    OPTION,
    CREDITS,
    EXIT,
    NOTSELECTED
};

class StateButton : public StateGlobal
{
public:
    void OnButtonUp() override
    { 
        switch (buttonType)
        {
        case JOIN:
            SceneManager::CreateSceneType<MainScene_Client>("MainScene");
            SceneManager::ChangeCurrentScene("Joining Scene");
            break;
        case CREATE:
        {
            EngineManager::InitServeur("0.0.0.0", 1888);
            EngineManager::GetSocket()->StartReading();
            SceneManager::CreateSceneType<MainScene_Serveur>("MainScene");
            SceneManager::ChangeCurrentScene("Lobby Scene");
            break;
        }
        case OPTION:
        {
            SceneManager::ChangeCurrentScene("OptionsMenuScene");
            break;
        }
        case EXIT:
        {
            EngineManager::GetWindow()->Close();
            break;
        }
        case CREDITS:
        {
            SceneManager::ChangeCurrentScene("MenuCreditScene");
            break;
        }
        default:
            break;
        }
    }

    void OnButtonPressed() override
    { 
    }

    void OnButtonDown() override
    { 
    }

    void OnHoveredEnter() override
    {
        pTarget->transform.SetWorldPosition({ 0.05f, pTarget->transform.GetWorldPosition().y, pTarget->transform.GetWorldPosition().z });
    }

    void OnHoveredExit() override
    {
        pTarget->transform.SetWorldPosition({ 0.0f, pTarget->transform.GetWorldPosition().y, pTarget->transform.GetWorldPosition().z });
    }

    TransformComponent* pTarget;

    Button buttonType = NOTSELECTED;
};


void MenuScene::Reset()
{
}

void MenuScene::OnInit()
{

    RegisterSystem<UIButtonSystem>();

    Device* pDevice = EngineManager::GetDevice();
    MapLoader::LoadMap("../../res/JSON/map_Menu.json", pDevice, *this, nullptr, "Menu");

    Geometry* book1Geo = GeometryFactory::LoadGeometry(pDevice, "../../res/Obj/BookCreate.obj");
    Geometry* book2Geo = GeometryFactory::LoadGeometry(pDevice, "../../res/Obj/BookJoin.obj");
    Geometry* book3Geo = GeometryFactory::LoadGeometry(pDevice, "../../res/Obj/BookOptions.obj");
    Geometry* book4Geo = GeometryFactory::LoadGeometry(pDevice, "../../res/Obj/BookCredits.obj");
    Geometry* book5Geo = GeometryFactory::LoadGeometry(pDevice, "../../res/Obj/BookExit.obj");
    
    m_pCamera = pCamera->GetOwner();
    m_pCamera->name = "Camera";
    Entity* book1 = CreateEntity();
    book1->name = "book1";
    Entity* book2 = CreateEntity();
    book2->name = "book2";
    Entity* book3 = CreateEntity();
    book3->name = "book3";
    Entity* book4 = CreateEntity();
    book4->name = "book4";
    Entity* book5 = CreateEntity();
    book5->name = "book5";

    TransformComponent* book1Transform = GetComponentType<TransformComponent>(book1);
    book1Transform->transform.SetWorldPosition({.0f, 0.0f, 0.0f});
    MeshRenderer* book1Mesh = AddComponent<MeshRenderer>(book1);
    book1Mesh->pGeometry = book1Geo;
    Texture* book1Tex = pDevice->CreateTexture(L"../../res/Textures/Books/bordeauxC.dds");
    Material* book1Mat = RessourceManager::GetShader("Texture")->CreateMaterial();
    book1Mat->SetTexture("Albedo", book1Tex);
    book1Mesh->pMaterial = book1Mat;

    TransformComponent* book2Transform = GetComponentType<TransformComponent>(book2);
    book2Transform->transform.SetWorldPosition({0.0f, 0.0f, 0.0f});
    MeshRenderer* book2Mesh = AddComponent<MeshRenderer>(book2);
    book2Mesh->pGeometry = book2Geo;
    Texture* book2Tex = pDevice->CreateTexture(L"../../res/Textures/Books/bleuJ.dds");
    Material* book2Mat = RessourceManager::GetShader("Texture")->CreateMaterial();
    book2Mat->SetTexture("Albedo", book2Tex);
    book2Mesh->pMaterial = book2Mat;

    TransformComponent* book3Transform = GetComponentType<TransformComponent>(book3);
    book3Transform->transform.SetWorldPosition({0.0f, 0.0f, 0.0f});
    MeshRenderer* book3Mesh = AddComponent<MeshRenderer>(book3);
    book3Mesh->pGeometry = book3Geo;
    Texture* book3Tex = pDevice->CreateTexture(L"../../res/Textures/Books/vertO.dds");
    Material* book3Mat = RessourceManager::GetShader("Texture")->CreateMaterial();
    book3Mat->SetTexture("Albedo", book3Tex);
    book3Mesh->pMaterial = book3Mat;

    TransformComponent* book4Transform = GetComponentType<TransformComponent>(book4);
    book4Transform->transform.SetWorldPosition({0.0f, 0.0f, 0.0f});
    MeshRenderer* book4Mesh = AddComponent<MeshRenderer>(book4);
    book4Mesh->pGeometry = book4Geo;
    Texture* book4Tex = pDevice->CreateTexture(L"../../res/Textures/Books/cyanC.dds");
    Material* book4Mat = RessourceManager::GetShader("Texture")->CreateMaterial();
    book4Mat->SetTexture("Albedo", book4Tex);
    book4Mesh->pMaterial = book4Mat;

    TransformComponent* book5Transform = GetComponentType<TransformComponent>(book5);
    book5Transform->transform.SetWorldPosition({0.0f, 0.0f, 0.0f});
    MeshRenderer* book5Mesh = AddComponent<MeshRenderer>(book5);
    book5Mesh->pGeometry = book5Geo;
    Texture* book5Tex = pDevice->CreateTexture(L"../../res/Textures/Books/jauneE.dds");
    Material* book5Mat = RessourceManager::GetShader("Texture")->CreateMaterial();
    book5Mat->SetTexture("Albedo", book5Tex);
    book5Mesh->pMaterial = book5Mat;
    
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    TransformComponent* pTransformCamera = AddComponent<TransformComponent>(m_pCamera);
    pTransformCamera->transform.SetWorldPosition(XMFLOAT3(0.9f, 0.0f, 0.0f)); 
    pTransformCamera->transform.SetLocalRotation(XMFLOAT3(-XM_PIDIV2, 0.0f, 0.0f)); 
    CameraComponent* pCamComponent = GetComponentType<CameraComponent>(m_pCamera);
    pCamComponent->camera.nearPlane = 0.1f;
    pCamComponent->camera.fov = 0.3f * MathHelper::Pi;
    EngineManager::GetDevice()->SetMainCamera(&pCamComponent->camera);
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Entity* book1Btn = CreateEntity();
    SpriteComponent* book1Sprite = AddComponent<SpriteComponent>(book1Btn);
    book1Sprite->width = 80;
    book1Sprite->height = 300;
    book1Sprite->pRect = SpriteFactory::BuildRectangle(pDevice, book1Sprite->width, book1Sprite->height);
    book1Sprite->isToDraw = false;

    Entity* book2Btn = CreateEntity();
    SpriteComponent* book2Sprite = AddComponent<SpriteComponent>(book2Btn);
    book2Sprite->width = 63;
    book2Sprite->height = 300;
    book2Sprite->pRect = SpriteFactory::BuildRectangle(pDevice, book2Sprite->width, book2Sprite->height);
    //book2Sprite->offsetCenter = { -0.052f, 0.0f };
    book2Sprite->isToDraw = false;

    Entity* book3Btn = CreateEntity();
    SpriteComponent* book3Sprite = AddComponent<SpriteComponent>(book3Btn);
    book3Sprite->width = 65;
    book3Sprite->height = 300;
    book3Sprite->pRect = SpriteFactory::BuildRectangle(pDevice, book3Sprite->width, book3Sprite->height);
    //book3Sprite->offsetCenter = { -0.005f, 0.0f };
    book3Sprite->isToDraw = false;

    Entity* book4Btn = CreateEntity();
    SpriteComponent* book4Sprite = AddComponent<SpriteComponent>(book4Btn);
    book4Sprite->width = 67;
    book4Sprite->height = 300;
    book4Sprite->pRect = SpriteFactory::BuildRectangle(pDevice, book4Sprite->width, book4Sprite->height);
    //book4Sprite->offsetCenter = { 0.042f, 0.0f };
    book4Sprite->isToDraw = false;

    Entity* book5Btn = CreateEntity();
    SpriteComponent* book5Sprite = AddComponent<SpriteComponent>(book5Btn);
    book5Sprite->width = 77;
    book5Sprite->height = 300;
    book5Sprite->pRect = SpriteFactory::BuildRectangle(pDevice, book5Sprite->width, book5Sprite->height);
    //book5Sprite->offsetCenter = { 0.086f, 0.0f };
    book5Sprite->isToDraw = false;

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    TransformComponent* pTransformbtn1 = GetComponentType<TransformComponent>(book1Btn);
    pTransformbtn1->transform.SetWorldPosition({ -150.0f,0.0f,0.0f });
    TransformComponent* pTransformbtn2 = GetComponentType<TransformComponent>(book2Btn);
    pTransformbtn2->transform.SetWorldPosition({ -75.0f,0.0f,0.0f });
    TransformComponent* pTransformbtn3 = GetComponentType<TransformComponent>(book3Btn);
    pTransformbtn3->transform.SetWorldPosition({ -10.0f,0.0f,0.0f });
    TransformComponent* pTransformbtn4 = GetComponentType<TransformComponent>(book4Btn);
    pTransformbtn4->transform.SetWorldPosition({ 65.0f,0.0f,0.0f });
    TransformComponent* pTransformbtn5 = GetComponentType<TransformComponent>(book5Btn);
    pTransformbtn5->transform.SetWorldPosition({ 140.0f,0.0f,0.0f });

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    AddComponent<UIButtonComponent>(book1Btn);
    StateButton* stateGlobal1 = new StateButton;
    stateGlobal1->pTarget = book1Transform;
    StateMachineComponent* pSm1 = AddComponent<StateMachineComponent>(book1Btn);
    pSm1->SetStateGlobal(stateGlobal1);
    stateGlobal1->buttonType = CREATE;

    AddComponent<UIButtonComponent>(book2Btn);
    StateButton* stateGlobal2 = new StateButton;
    stateGlobal2->pTarget = book2Transform;
    StateMachineComponent* pSm2 = AddComponent<StateMachineComponent>(book2Btn);
    pSm2->SetStateGlobal(stateGlobal2);
    stateGlobal2->buttonType = JOIN;

    AddComponent<UIButtonComponent>(book3Btn);
    StateButton* stateGlobal3 = new StateButton;
    stateGlobal3->pTarget = book3Transform;
    StateMachineComponent* pSm3 = AddComponent<StateMachineComponent>(book3Btn);
    pSm3->SetStateGlobal(stateGlobal3);
    stateGlobal3->buttonType = OPTION;

    AddComponent<UIButtonComponent>(book4Btn);
    StateButton* stateGlobal4 = new StateButton;
    stateGlobal4->pTarget = book4Transform;
    StateMachineComponent* pSm4 = AddComponent<StateMachineComponent>(book4Btn);
    pSm4->SetStateGlobal(stateGlobal4);
    stateGlobal4->buttonType = CREDITS;

    AddComponent<UIButtonComponent>(book5Btn);
    StateButton* stateGlobal5 = new StateButton;
    stateGlobal5->pTarget = book5Transform;
    StateMachineComponent* pSm5 = AddComponent<StateMachineComponent>(book5Btn);
    pSm5->SetStateGlobal(stateGlobal5);
    stateGlobal5->buttonType = EXIT;

    m_pLight = GetFirstAvailableLight();
    m_pLight->GetOwner()->isActive = true;
    m_pLight->SetLight(LightType::Point, 1.0f,
            { -0.5f, 0.0f, 0.5f }, { 0.8f, 0.74f, 0.62f, 1.0f },
            {1.25f, 0.0f, 0.0f});
    
    SceneManager::CreateSceneType<LobbyScene>("Lobby Scene");
    SceneManager::CreateSceneType<JoiningScene>("Joining Scene");    

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


    
    m_isInit = true;
}

void MenuScene::OnStart()
{
    InputSystem::UnlockMouseCursor();
    InputSystem::ShowMouseCursor();
    m_pLight->CallUpdate();
}

void MenuScene::OnUpdate(float _dt)
{
}

#endif
