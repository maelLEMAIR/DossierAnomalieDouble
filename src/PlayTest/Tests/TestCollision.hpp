#ifndef TEST_COLLISION_HPP_DEFINED
#define TEST_COLLISION_HPP_DEFINED

#include "pch.h"
#include "Test.h"
#include "Engine/Components/MeshRenderer.h"
#include "Engine/Systems/RenderSystem.h"

class Sphere1Global : public StateGlobal
{
public:
	void OnStart() override
	{
        TransformComponent* transform = SceneManager::GetSceneWithName("Default")->GetComponentType<TransformComponent>(m_pOwner);
        transform->transform.SetWorldPosition({-5.0f, 0.0f, 0.0f});
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

class Sphere2Global : public StateGlobal
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
        if (InputSystem::IsKeyDown(A))
        {
            SceneManager::GetCurrentScene()->DestroyEntity(m_pOwner);
        }
    }
};

class TestCollision : public Test
{
public:
    static void Run()
    {
        EngineManager engineManager;
        engineManager.Initialize(1080, 720, L"Test");
        Scene* scene = SceneManager::GetSceneWithName("Default");

        Device* pDevice = engineManager.GetDevice();

        Entity* sphere1 = scene->CreateEntity();
        sphere1->name = "Sphere 1";
        MeshRenderer* meshSphere1 = scene->AddComponent<MeshRenderer>(sphere1);
        meshSphere1->pGeometry = GeometryFactory::BuildIcosphere(pDevice, 32);
        StateMachineComponent* sm1 = scene->AddComponent<StateMachineComponent>(sphere1);
        sm1->SetStateGlobal(new Sphere1Global);
        Collider* collider1 = scene->AddComponent<Collider>(sphere1);
        collider1->colliderType = ColliderType::SPHERE;
        //collider1->canBounce = true;
        //collider1->scale = { 1.0f, 1.0f, 1.0f };
        ForceComponent* force1 = scene->AddComponent<ForceComponent>(sphere1);
        force1->velocity = { 0.0f, -1.0f, 0.0f };
        force1->useGravity = true;

        Entity* sphere2 = scene->CreateEntity();
        sphere2->name = "Sphere 2";
        MeshRenderer* meshSphere2 = scene->AddComponent<MeshRenderer>(sphere2);
        meshSphere2->pGeometry = GeometryFactory::BuildIcosphere(pDevice, 32);
        StateMachineComponent* sm2 = scene->AddComponent<StateMachineComponent>(sphere2);
        sm2->SetStateGlobal(new Sphere2Global);
        Collider* collider2 = scene->AddComponent<Collider>(sphere2);
        collider2->colliderType = ColliderType::SPHERE;
        collider2->canBounce = true;
        scene->AddComponent<ForceComponent>(sphere2);
        collider2->isStatic = true;

        Entity* floor = scene->CreateEntity();
        floor->name = "Floor";
        MeshRenderer* meshCube = scene->AddComponent<MeshRenderer>(floor);
        meshCube->pGeometry = GeometryFactory::BuildCube(pDevice);
        TransformComponent* transformCube = scene->GetComponentType<TransformComponent>(floor);
        transformCube->transform.SetLocalRotation({0.0f, 0.0f, -0.4f});
        transformCube->transform.SetWorldPosition({-5.5f, -4.0f, 0.0f});
        //transformCube->transform.SetWorldScale(2.0f);
        Collider* cubeCollider = scene->AddComponent<Collider>(floor);
        cubeCollider->colliderType = ColliderType::BOX;
        cubeCollider->isTrigger = true;
        //cubeCollider->isStatic = true;
        
        engineManager.Run();
    }
};

#endif