#ifndef TEST_RAY_CAST_H_DEFINED
#define TEST_RAY_CAST_H_DEFINED

#include "Test.h"
#include "Engine/EngineManager.h"

#include "components.h"

class RaycastScene : public Scene
{
public:
    void OnStart()
    {
        m_pCameraTransform = GetComponentType<TransformComponent>(pCamera->GetOwner());
        m_pCameraTransform->transform.SetWorldPosition({ 0.0f, 0.0f, 0.0f });
    }

    void OnUpdate(float _dt) override
    {
        if (InputSystem::IsMouseButtonDown(InputMouse::MIDDLE_MOUSE))
        {
            std::cout << "Start raycast" << std::endl;

            RayCast ray;
            ray.origin = m_pCameraTransform->transform.GetWorldPosition();
            ray.maxDist = 10.0f;
            ray.dir = m_pCameraTransform->transform.GetForward();
            ray.avoidTag.insert(0);

            HitPoint hp;
            GetPhysicSystem()->CheckRayCast(ray, hp);
            if (hp.pHitEntity != nullptr)
                std::cout << "hit entity with name : " << hp.pHitEntity->name << " at pos : " << hp.hitPoint.x << ".x " << hp.hitPoint.y << ".y " << hp.hitPoint.z << ".z" << std::endl;
        }
    }

private:
    TransformComponent* m_pCameraTransform;
};

class TestRayCast : public Test
{
public: 
    static void Run()
    {
         EngineManager engineManager;
         engineManager.Initialize(1080, 720, L"Test");
         Scene* scene = SceneManager::CreateSceneType<RaycastScene>("RaycastScene");
         SceneManager::ChangeCurrentScene("RaycastScene");
        
         Device* pDevice = engineManager.GetDevice();

         Geometry* pCube = GeometryFactory::BuildCube(pDevice);

         Entity* pFloor = scene->CreateEntity();
         pFloor->name = "Floor";
         pFloor->tag = -1;
         MeshRenderer* pMesh = scene->AddComponent<MeshRenderer>(pFloor);
         pMesh->pGeometry = pCube;
         Collider* pCollider = scene->AddComponent<Collider>(pFloor);
         pCollider->colliderType = ColliderType::BOX;
         TransformComponent* pTransform = scene->GetComponentType<TransformComponent>(pFloor);
         pTransform->transform.SetWorldPosition({0.0f, 0.0f, 5.0f});
         pTransform->transform.SetLocalRotation({PI / 6.0f, PI / 6.0f, 0.0f});
        
         engineManager.Run();
    }
};

#endif