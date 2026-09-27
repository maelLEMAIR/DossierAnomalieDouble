#ifndef SCENE_INL_DEFINED
#define SCENE_INL_DEFINED 

#include "Scene.h"

#include "Entity.h"
#include "Component.h"
#include "System.h"
#include "../Core/Engine_core.h"
#include "EngineManager.h"

template<typename T>
inline T* Scene::AddComponent(Entity* _pEntity)
{
	if (_pEntity == nullptr) return nullptr;

	EngineManager::RegisterComponent<T>();

	if (HasComponent<T>(_pEntity)) return GetComponentType<T>(_pEntity);

	///////
	UpdateEntityInArchetype(_pEntity, COMPONENT_MASK(T));
	///////

	T* pNewComponent = new T;

	Component* pComponent = pNewComponent;
	pComponent->SetEntity(_pEntity);
	pComponent->mask = COMPONENT_MASK(T);

	m_mComponents[COMPONENT_MASK(T)].push_back(pComponent);
	m_vCreationComponents.push_back(pComponent);

	return pNewComponent;
}

template<typename T>
inline bool Scene::HasComponent(Entity* _pEntity)
{
	if (m_mComponents.count(COMPONENT_MASK(T)) == false) return false;
	if (_pEntity == nullptr) return false;

	for (Component* pComponent : m_mComponents[COMPONENT_MASK(T)])
	{
		if (pComponent->GetOwner() == _pEntity)
			return true;
	}

	return false;
}

template<typename T>
inline Component* Scene::GetComponent(Entity* _pEntity)
{
	if (_pEntity == nullptr) return nullptr;

	if (Scene::HasComponent<T>(_pEntity) == false) return nullptr;

	for (Component* pComponent : m_mComponents[COMPONENT_MASK(T)])
	{
		if (pComponent->GetOwner() == _pEntity)
		{
			return pComponent;
		}
	}

	return nullptr;
}

template<typename T>
inline T* Scene::GetComponentType(Entity* _pEntity)
{
	if (_pEntity == nullptr) return nullptr;

	if (Scene::HasComponent<T>(_pEntity) == false) return nullptr;

	for (Component* pComponent : m_mComponents[COMPONENT_MASK(T)])
	{
		if (pComponent->GetOwner() == _pEntity)
		{
			return reinterpret_cast<T*>(pComponent);
		}
	}

	return nullptr;
}

template<typename T>
inline void Scene::RemoveComponent(Entity* _pEntity)
{
	if (_pEntity == nullptr) return;
	if (Scene::HasComponent<T>(_pEntity) == false) return;

	T* toDelete = GetComponentType<T>(_pEntity);

	///////
	//UpdateEntityInArchetype<T>(_pEntity, false);
	///////

	int i = 0;
	for (Component* pComponent : m_mComponents[COMPONENT_MASK(T)])
	{
		if (pComponent->GetOwner() == _pEntity)
		{
			//m_mComponents[COMPONENT_MASK(T)].erase(m_mComponents[COMPONENT_MASK(T)].begin() + i);

			//Update les systemes
			//uint64_t mask = COMPONENT_MASK(T);
			//for (System* system : m_vSystems)
				//system->UpdateMapEntity(mask, _pEntity);
			
			m_vDestroyComponents.push_back(m_mComponents[COMPONENT_MASK(T)][i]);
			m_mComponents[COMPONENT_MASK(T)].erase(m_mComponents[COMPONENT_MASK(T)].begin() + i);

			return;
		}

		i++;
	}

	//delete toDelete;
}

template<typename ...ComponentType>
inline UnorderedMap<int, Vector<Component*>> Scene::GetAllComponentsEntities()
{
	UnorderedMap<int, Vector<Component*>> mComponents;

	uint64_t _mask = 0;

	((_mask |= GetMask<ComponentType>()), ...);

	for (auto& [mask, vector] : m_mArchetype)
	{
		uint64_t temp = mask;

		if ((temp & _mask) == _mask)
		{
			for (Entity* pEntity : vector)
			{
				(
					mComponents[pEntity->id].push_back(GetComponent<ComponentType>(pEntity)
					), ...);
			}
		}
	}

	return mComponents;
}

template<typename ComponentType>
inline Vector<Component*> Scene::GetAllComponentOfType()
{
	return m_mComponents[COMPONENT_MASK(ComponentType)];
}

template<typename SystemType>
inline SystemType* Scene::RegisterSystem(int _priority)
{
	for (System* pSystem : m_vSystems)
	{
		SystemType* pSystemType = dynamic_cast<SystemType*>(pSystem);

		if (pSystemType != nullptr) return pSystemType;
	}

	SystemType* newSystem = new SystemType;

	System* pSystem = newSystem;
	pSystem->priority = _priority;
	pSystem->Init(this);

	//add to queue	
	int i = 0;
	for (System* system : m_vSystems)
	{
		if (system->priority > _priority)
		{
			m_vSystems.insert(m_vSystems.begin() + i, pSystem);
			return newSystem;
		}
		i++;
	}
	m_vSystems.push_back(pSystem);
	return newSystem;
}

#endif
