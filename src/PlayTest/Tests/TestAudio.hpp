#ifndef TEST_AUDIO_HPP_DEFINED
#define TEST_AUDIO_HPP_DEFINED

#include "pch.h"
#include "Test.h"

class Camera_State : public StateGlobal
{
public:
	void OnStart() override
	{
		m_pFartSound = AudioEngine::LoadWav(GetResWPath() + L"Audio/fart.wav");
		m_pGuitareSound = AudioEngine::LoadWav(GetResWPath() + L"Audio/guitare.wav");
	}

	void OnUpdate(float _dt) override 
	{
		float dt = EngineManager::GetDeltaTime();
		if (InputSystem::IsKeyDown(Z))
			AudioEngine::Play3D(m_pGuitareSound,  -1.0f, 0.0f, 1.0f);
		if (InputSystem::IsKeyDown(A))
			AudioEngine::PlaySoundW(m_pFartSound);
	}
private:
	Sound* m_pFartSound = nullptr;
	Sound* m_pGuitareSound = nullptr;
};

class TestAudio : public Test
{
public:
    static void Run()
    {
    	EngineManager engineManager;
    	engineManager.Initialize(1080, 720, L"TestCamera");

		Device* pDevice = engineManager.GetDevice();

    	Scene* scene = SceneManager::GetSceneWithName("Default");
    	scene->RegisterSystem<RenderSystem>();
    	scene->RegisterSystem<PhysicSystem>();
    	scene->RegisterSystem<StateMachineSystem>();
    	scene->RegisterSystem<TransformSystem>();
    	scene->RegisterSystem<InputSystem>();
    	scene->RegisterSystem<CameraSystem>();

    	Entity* pEntity1 = scene->pCamera->GetOwner();
    	TransformComponent* pTransform = scene->AddComponent<TransformComponent>(pEntity1);
    	pTransform->transform.SetWorldPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));
    	CameraComponent* pCamComponent = scene->AddComponent<CameraComponent>(pEntity1);
		EngineManager::GetDevice()->SetMainCamera(&pCamComponent->camera);
    	
    	StateMachineComponent* pSM = scene->AddComponent<StateMachineComponent>(pEntity1);
    	pSM->SetStateGlobal(new Camera_State);

    	Entity* f1Model = scene->CreateEntity();
    	MeshRenderer* f1ModelMR = scene->AddComponent<MeshRenderer>(f1Model);
		f1ModelMR->pGeometry = GeometryFactory::LoadGeometry(pDevice, "../../res/Obj/F1.obj");
    	
    	engineManager.Run();
    }
};

#endif