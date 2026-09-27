#ifndef SYSTEM_INL_DEFINED
#define SYSTEM_INL_DEFINED 

#include "System.h"
#include "Scene.h"
#include "../Core/Engine_core.h"
#include "EngineManager.h"

template <typename ComponentType>
uint64_t GetMask()
{
	uint64_t mask = COMPONENT_MASK(ComponentType);
	return COMPONENT_MASK(ComponentType);
}

template <typename ...ComponentType>
inline void System::SetMaskLoadComponents()
{
	((EngineManager::RegisterComponent<ComponentType>()), ...);

	((m_mask |= GetMask<ComponentType>()), ...);

	if (m_avoidMask == -1)
		m_mComponents = m_pOwnerScene->GetAllComponentsEntitiesWithMask(m_mask);
	else
		m_mComponents = m_pOwnerScene->GetAllComponentsEntitiesWithMaskAndWithoutMask(m_mask, m_avoidMask);
}

template<typename ComponentType>
inline void System::SetAvoidMaskComponent()
{
	EngineManager::RegisterComponent<ComponentType>();

	m_avoidMask |= GetMask<ComponentType>();
}

template<typename ...ComponentType>
inline void System::SetMaskOrComponents()
{
	((EngineManager::RegisterComponent<ComponentType>()), ...);

	((m_mask |= GetMask<ComponentType>()), ...);

	//get all components of the type
	m_mComponents = m_pOwnerScene->GetEntitiesWithComponentOfMask(m_mask); //for each entity who have a mesh or a text

	m_isOr = true;
}

#endif