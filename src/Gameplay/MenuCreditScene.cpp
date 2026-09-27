#ifndef MENU_CREDIT_SCENE_CPP_INCLUDED
#define MENU_CREDIT_SCENE_CPP_INCLUDED

#include "MenuCreditScene.h"
#include "MapLoader.h"

void MenuCreditScene::Reset()
{
}

void MenuCreditScene::OnInit()
{
    Device* pDevice = EngineManager::GetDevice();
    
    MapLoader::LoadMap("../../res/JSON/map_menuCredits.json", pDevice, *this, nullptr, "Credits");

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

void MenuCreditScene::OnStart()
{
}

void MenuCreditScene::OnUpdate(float _dt)
{
    if (InputSystem::IsKeyDown(InputKeyboard::ESC))
    {
        SceneManager::ChangeCurrentScene("MenuScene");
    }
}

#endif
