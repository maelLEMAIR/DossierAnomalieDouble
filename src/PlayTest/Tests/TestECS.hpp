#ifndef TEST_ECS_HPP_DEFINED
#define TEST_ECS_HPP_DEFINED

#include "pch.h"
#include "Test.h"

class Player_StateGlobal : public StateGlobal
{
public:
	void OnStart() override
	{
		TransformComponent* pTransform = SceneManager::GetSceneWithName("GameScene")->GetComponentType<TransformComponent>(m_pOwner);
		//pTransform->transform.ScaleLocal(1.5f);
		pTransform->transform.AddLocalYPR({0.2f, 0.0f, 0.0f});
	}

	void OnUpdate(float _dt) override 
	{
		//std::cout << "State global update is call" << std::endl;
		TransformComponent* pTransform = SceneManager::GetCurrentScene()->GetComponentType<TransformComponent>(m_pOwner);
		//pTransform->transform.MoveLocal({ 0.1f, 0.0f, 0.0f });
		//pTransform->transform.SetLocalPosition({10.0f, 0.0f, 0.0f});		
	}
};

class Child_StateGlobal : public StateGlobal
{
public:
	void OnStart() override
	{
		TransformComponent* pTransform = SceneManager::GetSceneWithName("GameScene")->GetComponentType<TransformComponent>(m_pOwner);
		//pTransform->transform.SetWorldPosition({ 5.0f, 0.0f, 0.0f });
		//pTransform->transform.SetLocalScale(2.f);
		//pTransform->transform.SetWorldScale(4.f);
	}

	void OnUpdate(float _dt) override 
	{
		TransformComponent* pTransform = SceneManager::GetCurrentScene()->GetComponentType<TransformComponent>(m_pOwner);
		//pTransform->transform.MoveLocal({0.1f, 0.0f, 0.0f});
		//std::cout << "State global update is call" << std::endl;
	}
};

struct TestState1 : public State
{
	void OnUpdate(float _dt) override {
		TransformComponent* pTransform = SceneManager::GetCurrentScene()->GetComponentType<TransformComponent>(m_pOwner);
		XMFLOAT4 rot = pTransform->transform.GetWorldRotation();
		std::cout << rot.x << " " << rot.y << " " << rot.z << " " << rot.w << std::endl;
	}
};

struct TestState2 : public State
{

};

class GameScene : public Scene
{
private:
	void OnInit() override 
	{
		RegisterSystem<PhysicSystem>();
		RegisterSystem<StateMachineSystem>();
		RegisterSystem<TransformSystem>();

		Entity* pEntity1 = CreateEntity();
		AddComponent<Collider>(pEntity1);
		StateMachineComponent* pSM = AddComponent<StateMachineComponent>(pEntity1);
		pSM->SetStateGlobal(new Player_StateGlobal);
		pSM->AddState<TestState1>();
		pSM->AddState<TestState2>();
		pSM->ToState(STATE_ID(TestState1));

		Entity* pEntity2 = CreateEntity();
		StateMachineComponent* pSM2 = AddComponent<StateMachineComponent>(pEntity2);
		pSM2->SetStateGlobal(new Child_StateGlobal);
		pSM2->AddState<TestState1>();
		pSM2->AddState<TestState2>();
		pSM2->ToState(STATE_ID(TestState1));
		Entity* pEntity3 = CreateEntity();

		ParentComponent* c = AddComponent<ParentComponent>(pEntity1);
		c->vChildrens = {pEntity2, pEntity3};
		//AddComponent<ParentComponent>(pEntity2)->m_pParents = pEntity1;
		//AddComponent<ParentComponent>(pEntity3)->m_pParents = pEntity1;

		bool test = HasComponent<Collider>(pEntity1);
	}
};

class TestECS : public Test
{
public:
    static void Run()
    {
		//ECS ecs;
		SceneManager sceneManager;

		Scene* pDefaultScene = sceneManager.GetSceneWithName("Default");

		//pDefaultScene->RegisterSystem<PhysicSystem>();

		//Entity* entity = pDefaultScene->CreateEntity();
		//pDefaultScene->AddComponent<Collider>(entity);

		//bool has = pDefaultScene->HasComponent<TransformComponent>(entity);
		
		//std::unordered_map<int, std::vector<Component*>> result = pDefaultScene->GetAllComponentsEntities<TransformComponent, Collider>();

		GameScene* pGameScene = sceneManager.CreateSceneType<GameScene>("GameScene");

		sceneManager.ChangeCurrentScene("GameScene");

		while (true)
		{
			//ecs.Update(0);
		}
    }
};

#endif