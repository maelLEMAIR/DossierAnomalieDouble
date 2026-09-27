#ifndef SYSTEM_CPP_DEFINED
#define SYSTEM_CPP_DEFINED 

#include "System.h"
#include "Scene.h"

void System::Init(Scene* _pOwner)
{
	m_pOwnerScene = _pOwner;

	OnInit();

	//UpdateMap(m_mask);
}

void System::UpdateMap(uint64_t _maskUpdate)
{
	if ((m_mask & _maskUpdate) == _maskUpdate)
	{
		m_mComponents = m_pOwnerScene->GetAllComponentsEntitiesWithMask(m_mask);
	}
}
 
void System::UpdateMapEntity(uint64_t maskUpdate, Entity* pEntity)
{
	if ((m_mask & maskUpdate) == maskUpdate || (m_avoidMask & maskUpdate) == maskUpdate) //if system is concerned with the component
	{
		if (m_isOr == false)
		{
			if (m_mComponents.contains(pEntity->id)) //if entity already in system
			{
				if ((pEntity->mask & m_mask) != m_mask || ((pEntity->mask & m_avoidMask) == m_avoidMask && m_avoidMask != 0)) //entity doesnt have anymore the component required for this system or have bloquing components
				{
					m_mComponents.erase(pEntity->id);
					OnUpdateMapEntity(maskUpdate, pEntity, false);
				}
				else
				{
					OnUpdateMapEntity(maskUpdate, pEntity);
					m_mComponents[pEntity->id] = m_pOwnerScene->GetAllComponentEntityWithMask(pEntity, m_mask);
				}
			}
			else //system doesnt have this entity
			{
				if ((pEntity->mask & m_mask) == m_mask && ((pEntity->mask & m_avoidMask) != m_avoidMask || m_avoidMask == 0)) //if the entity have the component required for this system and doesnt have the bloquing component
				{
					m_mComponents[pEntity->id] = m_pOwnerScene->GetAllComponentEntityWithMask(pEntity, m_mask);
					OnUpdateMapEntity(maskUpdate, pEntity);
				}
				/*else if (m_isOr && pEntity->mask)
				{
					m_mComponents[pEntity->id] = m_pOwnerScene->GetAllComponentEntityWithMask(pEntity, m_mask);
					OnUpdateMapEntity(maskUpdate, pEntity);
				}*/
			}
		}
		else
		{
			if (m_mComponents.contains(pEntity->id)) //if already in system
			{
				if ((pEntity->mask & maskUpdate) != maskUpdate) //doesnt have anymore one of the component
				{
					//erase that component
					OnUpdateMapEntity(maskUpdate, pEntity, false);
				}
				else
				{
					OnUpdateMapEntity(maskUpdate, pEntity);
				}
				m_mComponents[pEntity->id] = m_pOwnerScene->GetAllComponentEntityWithMask(pEntity, m_mask);
			}
			else //not in the system
			{
				if ((pEntity->mask & maskUpdate) == maskUpdate) //if its one of the system
				{
					m_mComponents[pEntity->id] = m_pOwnerScene->GetAllComponentEntityWithMask(pEntity, m_mask);
					OnUpdateMapEntity(maskUpdate, pEntity);
				}
			}
		}
	}
}

#endif