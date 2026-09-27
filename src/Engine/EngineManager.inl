#ifndef ENGINE_MANAGER_INL_DEFINED
#define ENGINE_MANAGER_INL_DEFINED 

#include "EngineManager.h"
#include "Component.h"
#include "Core/Engine_core.h"

#include <cassert>

template<typename ComponentType>
void EngineManager::RegisterComponent()
{	
	uint64_t mask = COMPONENT_MASK(ComponentType);

	if (COMPONENT_MASK(ComponentType) == -1)
	{
		assert(s_pInstance->m_componentCount < 64);
		uint64_t id = 1ULL << s_pInstance->m_componentCount;

		COMPONENT_MASK(ComponentType) = id;
		s_pInstance->m_componentCount++;
	}
}

#endif