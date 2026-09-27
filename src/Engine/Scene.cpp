#ifndef SCENE_CPP_DEFINED
#define SCENE_CPP_DEFINED

#include "Scene.h"
#include "components.h"

#include "systems.h"
#include "Components/CameraComponent.h"
#include "Systems/SpriteSheetAnimatorSystem.h"

void Scene::Init(std::string _name)
{
    m_name = _name;

    // RegisterSystemGlobal
    RegisterSystem<InputSystem>();
    RegisterSystem<StateMachineSystem>();
    pForceSystem = RegisterSystem<ForceSystem>();
    pTransformSystem = RegisterSystem<TransformSystem>();
    pPhysicSystem    = RegisterSystem<PhysicSystem>();
    RegisterSystem<CameraSystem>();
    RegisterSystem<RenderSystem>();
    pCollisionSystem = RegisterSystem<CollisionSystem>();
    pLightSystem     = RegisterSystem<LightSystem>(); 
    RegisterSystem<SpriteSheetAnimatorSystem>(); 
    RegisterSystem<UITextInputSystem>();
    RegisterSystem<UISliderSystem>();
    RegisterSystem<UIToggleSystem>();
    RegisterSystem<UIDropdownSystem>();
    RegisterSystem<UIButtonSystem>(); 

    //Camera
    Entity* pEntity = CreateEntity();
    TransformComponent* pTransform = GetComponentType<TransformComponent>(pEntity);
    pTransform->transform.SetLocalPosition(XMFLOAT3(0.0f, 0.0f, -25.f));
    pCamera = AddComponent<CameraComponent>(pEntity);
    EngineManager::GetDevice()->SetMainCamera(&pCamera->camera);

    //Directional light
    /*Entity* pLightEntity = CreateEntity();
    LightComponent* pDefaultLight = AddComponent<LightComponent>(pLightEntity);
    pDefaultLight->SetLight(LightType::Directional, 0.2f, { 0.0f, 0.0f, 1.0f });*/

    for (int i = 0; i < 32; i++)
    {
        Entity* pLight = CreateEntity();
        pLight->name = "Light";
        m_vLights.push_back(AddComponent<LightComponent>(pLight));
        pLight->isActive = false;
    }
    
    OnInit();

    m_isInit = true;
}

void Scene::Start()
{
    EngineManager::GetDevice()->SetMainCamera(&pCamera->camera);

    OnStart();
}

void Scene::Destroy()
{
    for (auto& [archetype, vEntities] : m_mArchetype)
    {
        for (Entity* pEntity : vEntities)
        {
            delete pEntity;
        }
    }

    for (auto& [archetype, vComponents] : m_mComponents)
    {
        for (Component* pComponet : vComponents)
        {
            delete pComponet;
        }
    }

    for (System* pSystem : m_vSystems)
    {
        delete pSystem;
    }

    delete this;
}

void Scene::UpdateEntityInArchetype(Entity* _pEntity, uint64_t _mask, bool _isPlus)
{
    for (int i = 0; i < m_mArchetype[_pEntity->mask].size(); i++)
    {
        if (_pEntity == m_mArchetype[_pEntity->mask][i])
        {
            m_mArchetype[_pEntity->mask].erase(m_mArchetype[_pEntity->mask].begin() + i);

            i = (int)m_mArchetype[_pEntity->mask].size();
        }

        i++;
    }

    if (m_mArchetype[_pEntity->mask].size() == 0)
    {
        m_mArchetype.erase(_pEntity->mask);
    }

    if (_isPlus)
        _pEntity->mask |= _mask;
    else
        _pEntity->mask &= ~_mask;

    m_mArchetype[_pEntity->mask].push_back(_pEntity);
}

void Scene::CreateComponents()
{
    Vector<Component*> vCreationComponents = m_vCreationComponents;
    m_vCreationComponents.clear();
    
    for (Component* component : vCreationComponents)
    {
        for (System* system : m_vSystems)
            system->UpdateMapEntity(component->mask, component->GetOwner());
    }

    vCreationComponents.clear();
}

void Scene::DestroyComponents()
{
    for (Component* component : m_vDestroyComponents)
    {
        UpdateEntityInArchetype(component->GetOwner(), component->mask, false);

        //Update les systemes
        for (System* system : m_vSystems)
            system->UpdateMapEntity(component->mask, component->GetOwner());

        delete component;
    }

    m_vDestroyComponents.clear();
}

void Scene::DeleteEntities()
{
    for (Entity* pEntity : m_vDestroyEntities)
    {
        delete pEntity;
    }

    m_vDestroyEntities.clear();
}

void Scene::UpdateComponentInSystem(uint64_t maskUpdate)
{
    for (System* pSystem : m_vSystems)
    {
        pSystem->UpdateMap(maskUpdate);
    }
}

Entity* Scene::CreateEntity(bool hasTransform)
{
    Entity* pNewEntity = new Entity;
    pNewEntity->id = m_entityCount;
    pNewEntity->pScene = this;

    m_entityCount++;

    m_mArchetype[0].push_back(pNewEntity);

    if (hasTransform)
        AddComponent<TransformComponent>(pNewEntity);

    return pNewEntity;
}

void Scene::DestroyEntity(Entity* _pEntity)
{
    if (_pEntity->isDestroy) return;

    uint64_t maskEntity = _pEntity->mask;
    _pEntity->mask = 0;

    for (auto& [mask, vComponents] : m_mComponents)
    {
        if ((maskEntity & mask) == mask)
        {
            for (int i = (int)vComponents.size() - 1; i >= 0; i--)
            {
                if (vComponents[i]->GetOwner() == _pEntity)
                {
                    m_vDestroyComponents.push_back(vComponents[i]);
                    vComponents.erase(vComponents.begin() + i);
                }
            }
        }
    }

    for (int i = (int)m_mArchetype[maskEntity].size() - 1; i >= 0; i--)
    {
        if (m_mArchetype[maskEntity][i] == _pEntity)
        {
            m_mArchetype[maskEntity].erase(m_mArchetype[maskEntity].begin() + i);
        }
    }

    _pEntity->isDestroy = true;
    m_vDestroyEntities.push_back(_pEntity);
}

std::vector<Component*> Scene::GetAllComponentEntityWithMask(Entity* _pEntity, uint64_t _mask)
{
    std::vector<Component*> vComponents;

    for (auto [mask, vComponentsTemp] : m_mComponents)
    {
        if ((_mask & mask) == mask)
        {
            for (Component* pComponent : vComponentsTemp)
            {
                if (pComponent->GetOwner() == _pEntity)
                {
                    vComponents.push_back(pComponent);
                }
            }
        }
    }

    return vComponents;
}

std::unordered_map<int, std::vector<Component*>> Scene::GetAllComponentsEntitiesWithMask(uint64_t _mask)
{
    std::unordered_map<int, std::vector<Component*>> mComponents;

    for (auto& [mask, vEntities] : m_mArchetype)
    {
        if ((mask & _mask) == _mask)
        {
            for (Entity* pEntity : vEntities)
            {
                mComponents[pEntity->id] = GetAllComponentEntityWithMask(pEntity, _mask);
            }
        }
    }

    return mComponents;
}

std::unordered_map<int, std::vector<Component*>> Scene::GetAllComponentsEntitiesWithMaskAndWithoutMask(uint64_t _withMask, uint64_t _withoutMask)
{
    std::unordered_map<int, std::vector<Component*>> mComponents;

    for (auto& [mask, vEntities] : m_mArchetype)
    {
        if ((mask & _withMask) == _withMask && (mask & _withMask) != _withoutMask)
        {
            for (Entity* pEntity : vEntities)
            {
                mComponents[pEntity->id] = GetAllComponentEntityWithMask(pEntity, _withMask);
            }
        }
    }

    return mComponents;
}

std::unordered_map<int, std::vector<Component*>> Scene::GetEntitiesWithComponentOfMask(uint64_t _mask)
{
    std::unordered_map<int, std::vector<Component*>> mComponents;

    for (auto& [mask, vEntities] : m_mArchetype)
    {
        if (mask & _mask)
        {
            for (Entity* pEntity : vEntities)
            {
                mComponents[pEntity->id] = GetAllComponentEntityWithMask(pEntity, _mask);
            }
        }
    }

    return mComponents;
}

LightComponent* Scene::GetFirstAvailableLight()
{
    /*int i = 0;
    for (LightComponent* component : m_vLights)
    {
        if (component->GetOwner()->isActive == false)
        {
            std::cout << "Pos in vector : " << i << std::endl;
            return component;
        }
        i++;
    }
    
    td::cout << "No light left" << std::endl;
    return nullptr;
    */

    if (m_vLights.size() == 0)
    {
        std::cout << "No light left" << std::endl;
        return nullptr;
    }

    LightComponent* pLight = m_vLights[0];
    m_vLights.erase(m_vLights.begin());
    pLight->GetOwner()->isActive = true;
    pLight->isActive = true;
    pLight->CallUpdate();

    std::cout << "Light left : " << m_vLights.size() << std::endl;

    return pLight;
}

void Scene::DisableLight(LightComponent* _pLight)
{
    if (_pLight->GetOwner()->isActive == true)
        _pLight->GetOwner()->isActive = false;

    _pLight->CallUpdate();

    m_vLights.push_back(_pLight);

    std::cout << "Light added, new light left : " << m_vLights.size() << std::endl;
}


#endif
