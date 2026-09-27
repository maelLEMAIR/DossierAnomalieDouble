#ifndef STATE_MACHINE_COMPONENT_INL_DEFINED
#define STATE_MACHINE_COMPONENT_INL_DEFINED 

#include "StateMachineComponent.h"
#include "../Core/Engine_core.h"

template<typename StateType>
inline StateType* StateMachineComponent::AddState()
{
	StateType* pNewState = new StateType();

	State* pState = pNewState;
	pState->SetOwner(m_pEntity);

	int id = (int)vStates.size();
	if (STATE_ID(StateType) == -1)
		STATE_ID(StateType) = id;

	vStates.push_back(pState);

	return pNewState;
}

#endif