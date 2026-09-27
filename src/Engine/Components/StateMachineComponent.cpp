#ifndef STATE_MACHINE_COMPONENT_CPP_DEFINED
#define STATE_MACHINE_COMPONENT_CPP_DEFINED

#include "StateMachineComponent.h"

#endif

void StateMachineComponent::SetStateGlobal(StateGlobal* _pState)
{
	m_pStateGlobal = _pState;
	m_pStateGlobal->SetOwner(m_pEntity); 
	m_pStateGlobal->OnStart();
}

void StateMachineComponent::ToState(int to)
{
	if (currentIndex != -1)
		vStates[currentIndex]->OnEnd();

	currentIndex = to;

	if (currentIndex != -1)
		vStates[currentIndex]->OnStart();
}
