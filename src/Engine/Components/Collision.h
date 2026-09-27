#ifndef COLLISION_COMPONENT_H_DEFINED
#define COLLISION_COMPONENT_H_DEFINED 

#include "Component.h"

namespace CollisionType
{
	enum CollisionType
	{
		ENTER,
		STAY,
		EXIT
	};
}

class Entity;
class Collider;

class Collision : public Component
{
public :
	std::unordered_map<CollisionType::CollisionType, std::vector<Entity*>> mOthers;
};

#endif