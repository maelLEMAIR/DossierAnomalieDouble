#ifndef COLLIDER_H_DEFINED
#define COLLIDER_H_DEFINED 

#include "Component.h"

class Entity;

namespace ColliderType
{
	enum ColliderType
	{
		NONE,
		SPHERE,
		BOX
	};

}

class Collider : public Component
{
public:
	std::vector<Entity*> vCurrentCollision;

	bool canBounce = false;
	bool isStatic = false;
	bool isTrigger = false;

	XMFLOAT3 scale = { 1.0f, 1.0f, 1.0f };

	ColliderType::ColliderType colliderType = ColliderType::NONE;
};

#endif