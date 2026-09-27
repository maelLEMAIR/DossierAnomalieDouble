#ifndef STATE_MACHINE_SYSTEM_CPP_DEFINED
#define STATE_MACHINE_SYSTEM_CPP_DEFINED

#include "StateMachineSystem.h"
#include "../Components/StateMachineComponent.h"

void StateMachineSystem::OnInit()
{
	SetMaskLoadComponents<StateMachineComponent>();
}

void StateMachineSystem::Update(float _dt)
{
	for (auto [id, vComponents] : m_mComponents)
	{
		if (vComponents[0]->GetOwner()->isActive == false) continue;
		if (vComponents[0]->isActive == false) continue;

		StateMachineComponent* pSM = reinterpret_cast<StateMachineComponent*>(vComponents[0]);

		if (pSM->m_pStateGlobal == nullptr) continue;

		pSM->m_pStateGlobal->OnUpdate(_dt);

		if (pSM->currentIndex != -1) //no state or no active state
		{
			pSM->vStates[pSM->currentIndex]->OnUpdate(_dt);
		}
	}
}

#endif
