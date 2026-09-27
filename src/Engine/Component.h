#ifndef COMPONENT_H_DEFINED
#define COMPONENT_H_DEFINED 

#include "Entity.h"

class Entity;

class Component
{
public:
	void SetEntity(Entity* _pEntity) { m_pEntity = _pEntity; }
	Entity* GetOwner() { return m_pEntity; }

	uint64_t mask = 0;
	bool isActive = true;

protected:
	Entity* m_pEntity = nullptr;
};

#endif