#ifndef TEST_PARENT_CHILDREN_HPP_DEFINED
#define TEST_PARENT_CHILDREN_HPP_DEFINED

#include "pch.h"
#include "Test.h"
#include "Engine/Components/MeshRenderer.h"
#include "Engine/Systems/RenderSystem.h"

class TestParent_StateGlobal : public StateGlobal
{
public:
	void OnStart() override
	{
	}

	void OnUpdate(float _dt) override
	{	
        TransformComponent* transform = SceneManager::GetCurrentScene()->GetComponentType<TransformComponent>(m_pOwner);
        /*transform->transform.MoveWorld({ 0.0f, 0.0f, 0.1f });
        std::cout << "Cube 1 : " << transform->transform.GetWorldPosition().z << std::endl; */ 
        /*transform->transform.ScaleLocal({1.0f, 1.0f, 1.005f});
        std::cout << "Cube 1 : " << transform->transform.GetWorldScale().x << std::endl;*/
        transform->transform.AddLocalYPR({0.0f, 0.01f, 0.0f});
	}
};

class TestChild_StateGlobal : public StateGlobal
{
public:
	void OnStart() override
	{
	}

	void OnUpdate(float _dt) override
	{
        TransformComponent* transform = SceneManager::GetCurrentScene()->GetComponentType<TransformComponent>(m_pOwner);
        /*transform->transform.MoveWorld({ 0.0f, 0.0f, -0.01f });
        std::cout << "Cube 2 : " << transform->transform.GetWorldPosition().z << std::endl;*/
        /*transform->transform.ScaleLocal({1.0f, 1.0f, 0.995f});
        std::cout << "Cube 2 : " << transform->transform.GetWorldScale().x << std::endl;*/
        transform->transform.MoveLocal({0.01f, 0.0f, 0.0f});
	}
};

class TestParentChildren : public Test
{
public:
    static void Run()
    {
        EngineManager engineManager;
        engineManager.Initialize(1080, 720, L"Test");
        Scene* scene = SceneManager::GetSceneWithName("Default");
        scene->RegisterSystem<RenderSystem>();
        scene->RegisterSystem<TransformSystem>();
        scene->RegisterSystem<StateMachineSystem>();

        Device* pDevice = engineManager.GetDevice();
        
        Entity* cube1 = scene->CreateEntity();
        MeshRenderer* meshCube1 = scene->AddComponent<MeshRenderer>(cube1);
        meshCube1->pGeometry = GeometryFactory::BuildCube(pDevice);
        StateMachineComponent* sm1 = scene->AddComponent<StateMachineComponent>(cube1);
        sm1->SetStateGlobal(new TestParent_StateGlobal);
        ParentComponent* parent = scene->AddComponent<ParentComponent>(cube1);
        
        Entity* cube2 = scene->CreateEntity();
        MeshRenderer* meshCube2 = scene->AddComponent<MeshRenderer>(cube2);
        meshCube2->pGeometry = GeometryFactory::BuildCube(pDevice);
        TransformComponent* transform = scene->GetComponentType<TransformComponent>(cube2);
        transform->transform.SetLocalPosition({2.0f, 2.0f, 0.0f});
        StateMachineComponent* sm2 = scene->AddComponent<StateMachineComponent>(cube2);
        sm2->SetStateGlobal(new TestChild_StateGlobal);
        ChildrenComponent* children = scene->AddComponent<ChildrenComponent>(cube2);

        children->SetParent(cube1);
        parent->vChildrens.push_back(cube2);
        
        engineManager.Run();
    }
};

#endif