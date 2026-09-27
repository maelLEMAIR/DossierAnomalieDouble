#ifndef TEST_CAMERA_HPP_DEFINED
#define TEST_CAMERA_HPP_DEFINED

#include "pch.h"
#include "Test.h"

class Camera_StateGlobal : public StateGlobal
{
public:
	void OnStart() override
	{
	}

	void OnUpdate(float _dt) override 
	{
		float dt = EngineManager::GetDeltaTime();
		if (InputSystem::IsKeyPressed(Z))
		{
			TransformComponent* pTransform = SceneManager::GetCurrentScene()->GetComponentType<TransformComponent>(m_pOwner);
			pTransform->transform.MoveWorld(XMFLOAT3(0.0f, 0.0f, 5.0f * dt));
		}if (InputSystem::IsKeyPressed(S))
		{
			TransformComponent* pTransform = SceneManager::GetCurrentScene()->GetComponentType<TransformComponent>(m_pOwner);
			pTransform->transform.MoveLocal(XMFLOAT3(0.0f, 0.0f, -5.0f * dt));
		}if (InputSystem::IsKeyPressed(SPACEBAR))
		{
			TransformComponent* pTransform = SceneManager::GetCurrentScene()->GetComponentType<TransformComponent>(m_pOwner);
			pTransform->transform.MoveLocal(XMFLOAT3(0.0f, 5.0f * dt, 0.0f));
		}if (InputSystem::IsKeyPressed(LCONTROL))
		{
			TransformComponent* pTransform = SceneManager::GetCurrentScene()->GetComponentType<TransformComponent>(m_pOwner);
			pTransform->transform.MoveLocal(XMFLOAT3(0.0f, -5.0f * dt, 0.0f));
		}if (InputSystem::IsKeyPressed(Q))
		{
			TransformComponent* pTransform = SceneManager::GetCurrentScene()->GetComponentType<TransformComponent>(m_pOwner);
			pTransform->transform.MoveLocal(XMFLOAT3(-5.0f * dt, 0.0f, 0.0f));
		}if (InputSystem::IsKeyPressed(D))
		{
			TransformComponent* pTransform = SceneManager::GetCurrentScene()->GetComponentType<TransformComponent>(m_pOwner);
			pTransform->transform.MoveLocal(XMFLOAT3(5.0f * dt, 0.0f, 0.0f));
		}

		if (InputSystem::IsKeyPressed(LEFT_ARROW))
		{
			TransformComponent* pTransform = SceneManager::GetCurrentScene()->GetComponentType<TransformComponent>(m_pOwner);
			pTransform->transform.AddLocalYPR(XMFLOAT3(-1.0f * dt, 0.0f, 0.0f));
		}if (InputSystem::IsKeyPressed(RIGHT_ARROW))
		{
			TransformComponent* pTransform = SceneManager::GetCurrentScene()->GetComponentType<TransformComponent>(m_pOwner);
			pTransform->transform.AddLocalYPR(XMFLOAT3(1.0f * dt, 0.0f, 0.0f));
		}

		if (InputSystem::IsMouseButtonDown(LEFT_MOUSE))
			InputSystem::HideMouseCursor();
		if (InputSystem::IsMouseButtonDown(RIGHT_MOUSE))
			InputSystem::ShowMouseCursor();
	}
};

class TestCamera : public Test
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

    	Entity* pEntity1 = scene->CreateEntity();
    	TransformComponent* pTransform = scene->AddComponent<TransformComponent>(pEntity1);
    	pTransform->transform.SetLocalPosition(XMFLOAT3(0.0f, 5.0f, -15.f));
    	CameraComponent* pCamComponent = scene->AddComponent<CameraComponent>(pEntity1);
		EngineManager::GetDevice()->SetMainCamera(&pCamComponent->camera);
    	
    	StateMachineComponent* pSM = scene->AddComponent<StateMachineComponent>(pEntity1);
    	pSM->SetStateGlobal(new Camera_StateGlobal);

    	Entity* f1Model = scene->CreateEntity();
    	MeshRenderer* f1ModelMR = scene->AddComponent<MeshRenderer>(f1Model);
		f1ModelMR->pGeometry = GeometryFactory::LoadGeometry(pDevice, "../../res/Obj/F1.obj");
    	//f1ModelMR->renderItem.pPso = EngineManager::GetDefaultPSO();

    	engineManager.Run();
    }
};

#endif