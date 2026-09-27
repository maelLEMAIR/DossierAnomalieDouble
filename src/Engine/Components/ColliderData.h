#ifndef COLLIDER_DATA_H_DEFINED
#define COLLIDER_DATA_H_DEFINED

#include "Collider.h"
#include "TransformComponent.h"

struct ColliderData
{
public:

	Collider* pCollider = nullptr;

	TransformComponent* pTransform = nullptr;

	int id = -1;

	std::vector<Entity*> vOldCollisions;

	bool IsActive() {
		return pCollider->GetOwner()->isActive && pCollider->isActive && pTransform->isActive;
	}
};

#endif