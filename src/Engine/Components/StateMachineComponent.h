#ifndef STATE_MACHINE_COMPONENT_H_DEFINED
#define STATE_MACHINE_COMPONENT_H_DEFINED

#include "Component.h"

class Entity;

class State
{
public:
	virtual void OnStart() {};
	virtual void OnUpdate(float _dt) {};
	virtual void OnEnd() {};

	void SetOwner(Entity* _pOwner) { m_pOwner = _pOwner; }

protected:
	Entity* m_pOwner = nullptr;

};

class StateGlobal : public State
{
public:
	virtual void OnCollisionEnter(Entity* other) {};
	virtual void OnCollisionStay(Entity* other) {};
	virtual void OnCollisionExit(Entity* other) {};

	virtual void OnButtonDown() {};
	virtual void OnButtonPressed() {};
	virtual void OnButtonUp() {};

	virtual void OnHoveredEnter() {};
	virtual void OnHoveredExit() {};

};

class StateMachineComponent : public Component
{
public:
	//inline static bool hasBeenRegister = false;
	inline static uint64_t typeMask = 0;

	void SetStateGlobal(StateGlobal* _pState);
	StateGlobal* GetStateGlobal() { return m_pStateGlobal; }

	template <typename StateType>
	StateType* AddState();

	void ToState(int to);

	std::vector<State*> vStates;
	int currentIndex = -1;

private:
	StateGlobal* m_pStateGlobal = nullptr;

	friend class StateMachineSystem;
};

#include "StateMachineComponent.inl"

#endif