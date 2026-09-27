#ifndef TEST_GRAVITY_HPP_DEFINED
#define TEST_GRAVITY_HPP_DEFINED

#include "pch.h"
#include "Test.h"
#include "Engine/Components/MeshRenderer.h"
#include "Engine/Systems/RenderSystem.h"

class Sphere1GravityGlobal : public StateGlobal
{
public:
	void OnStart() override
	{
        TransformComponent* transform = SceneManager::GetSceneWithName("Default")->GetComponentType<TransformComponent>(m_pOwner);
        transform->transform.SetLocalPosition({-5.0f, 0.0f, 0.0f});
	}

	void OnUpdate(float _dt) override
	{
        TransformComponent* transform = SceneManager::GetCurrentScene()->GetComponentType<TransformComponent>(m_pOwner);
        //transform->transform.MoveLocal({0.1f, 0.0f, 0.0f});
        //std::cout << "Pos 1 : " << transform->transform.GetWorldPosition().x << ".x " << transform->transform.GetWorldPosition().y << ".y " << transform->transform.GetWorldPosition().z << ".z" << std::endl;
	}

    void OnCollisionEnter(Entity* _pOther) override
    {
        std::cout << "Start collision" << std::endl;
    }

    void OnCollisionStay(Entity* _pOther) override
    {
        std::cout << "Collide stay" << std::endl;
    }

    void OnCollisionExit(Entity* _pOther) override
    {
        std::cout << "Collision stop" << std::endl;
    }
};

class Sphere2GravityGlobal : public StateGlobal
{
public:
	void OnStart() override
	{
        TransformComponent* transform = SceneManager::GetSceneWithName("Default")->GetComponentType<TransformComponent>(m_pOwner);
        transform->transform.SetLocalPosition({ 5.0f, -1.0f, 0.0f });
	}

	void OnUpdate(float _dt) override
	{
        TransformComponent* transform = SceneManager::GetCurrentScene()->GetComponentType<TransformComponent>(m_pOwner);
        //transform->transform.MoveLocal({ -0.1f, 0.0f, 0.0f });
        //std::cout << "Pos 1 : " << transform->transform.GetWorldPosition().x << ".x " << transform->transform.GetWorldPosition().y << ".y " << transform->transform.GetWorldPosition().z << ".z" << std::endl;
	}
};

class TestGravity : public Test
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
        scene->RegisterSystem<PhysicSystem>();
        scene->RegisterSystem<CollisionSystem>();
        scene->RegisterSystem<CameraSystem>();
        scene->RegisterSystem<ForceSystem>();

        Device* pDevice = engineManager.GetDevice();

        Entity* pEntity1 = scene->CreateEntity();
        TransformComponent* pTransform = scene->GetComponentType<TransformComponent>(pEntity1);
        pTransform->transform.SetLocalPosition(XMFLOAT3(0.0f, 0.0f, -25.f));
        CameraComponent* pCamComponent = scene->AddComponent<CameraComponent>(pEntity1);
        EngineManager::GetDevice()->SetMainCamera(&pCamComponent->camera);

        Entity* sphere1 = scene->CreateEntity();
        MeshRenderer* meshSphere1 = scene->AddComponent<MeshRenderer>(sphere1);
        meshSphere1->pGeometry = GeometryFactory::BuildIcosphere(pDevice, 32);
        StateMachineComponent* sm1 = scene->AddComponent<StateMachineComponent>(sphere1);
        sm1->SetStateGlobal(new Sphere1Global);
        Collider* collider1 = scene->AddComponent<Collider>(sphere1);
        collider1->canBounce = true;
        ForceComponent* force1 = scene->AddComponent<ForceComponent>(sphere1);
        force1->velocity = { 7.0f, 1.0f, 0.0f };
        force1->useGravity = true;

        Entity* sphere2 = scene->CreateEntity();
        MeshRenderer* meshSphere2 = scene->AddComponent<MeshRenderer>(sphere2);
        meshSphere2->pGeometry = GeometryFactory::BuildIcosphere(pDevice, 32);
        StateMachineComponent* sm2 = scene->AddComponent<StateMachineComponent>(sphere2);
        sm2->SetStateGlobal(new Sphere2Global);
        Collider* collider2 = scene->AddComponent<Collider>(sphere2);
        collider2->canBounce = true;
        scene->AddComponent<ForceComponent>(sphere2);
        //collider2->isStatic = true;

        
        engineManager.Run();
    }
};

#endif