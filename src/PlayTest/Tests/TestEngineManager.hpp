#ifndef TEST_ENGINE_MANAGER_HPP_DEFINED
#define TEST_ENGINE_MANAGER_HPP_DEFINED

#include "pch.h"
#include "Test.h"
#include "Engine/Components/CameraComponent.h"
#include "Engine/Components/MeshRenderer.h"
#include "Engine/Systems/RenderSystem.h"

class TestEngineManager : public Test
{
public:
    static void Run()
    {
        EngineManager engineManager;
        engineManager.Initialize(1080, 720, L"Test");
        Scene* scene = SceneManager::GetSceneWithName("Default");
        scene->RegisterSystem<RenderSystem>();

        Device* pDevice = engineManager.GetDevice();

        Entity* pEntity1 = scene->CreateEntity();
        CameraComponent* pCamComponent = scene->AddComponent<CameraComponent>(pEntity1);
    	
        XMFLOAT4X4 cWorld = MathHelper::Identity4x4();
        //cWorld._43 = -25.0f;
        cWorld._42 = 10.0f;
        cWorld._41 = 15.0f;
    	
        pCamComponent->camera.SetWorld(cWorld);
        XMFLOAT3 target = {0.0f, 0.0f, 0.0f};
        pCamComponent->camera.LookAt(target);
        EngineManager::GetDevice()->SetMainCamera(&pCamComponent->camera);
        
        Entity* f1Model = scene->CreateEntity();
        MeshRenderer* f1ModelMR = scene->AddComponent<MeshRenderer>(f1Model);
        f1ModelMR->pGeometry = GeometryFactory::LoadGeometry(pDevice,  "../../res/Obj/F1.obj");
        //f1ModelMR->renderItem.pPso = EngineManager::GetDefaultPSO();
        
        /*GraphicsContext::ResetCmdList();
        f1ModelMR->renderItem.pGeo = GeometryFactory::LoadGeometry("../../res/Obj/F1.obj");
        GraphicsContext::CloseCmdListAndFlush();*/
        
        engineManager.Run();
    }
};

#endif