#include "CollisionSystem.h"

#include <iostream>

#include "../components.h"

void CollisionSystem::OnInit()
{
	SetMaskLoadComponents<Collision>();
}

void CollisionSystem::Update(float dt)
{
	for (auto [id, vComponents] : m_mComponents)
	{
		if (vComponents[0]->GetOwner()->isActive == false) continue;
		if (vComponents[0]->isActive == false) continue;

		StateMachineComponent* pSM = m_pOwnerScene->GetComponentType<StateMachineComponent>(vComponents[0]->GetOwner());
		Collision* pCollision = reinterpret_cast<Collision*>(vComponents[0]);

		if (pSM != nullptr)
		{
			for (auto [type, others] : pCollision->mOthers)
			{
				for (Entity* other : others)
				{
					if (other->isActive == false) continue;

					switch (type)
					{
					case CollisionType::ENTER:
						pSM->GetStateGlobal()->OnCollisionEnter(other);
						break;
						
					case CollisionType::STAY:
						pSM->GetStateGlobal()->OnCollisionStay(other);
						break;
						
					case CollisionType::EXIT:
						pSM->GetStateGlobal()->OnCollisionExit(other);
						break;
					}
				}
			}
		}
		pCollision->mOthers.clear();
	}
}
