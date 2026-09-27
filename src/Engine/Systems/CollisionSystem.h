#ifndef COLLISION_SYSTEM_H_DEFINED
#define COLLISION_SYSTEM_H_DEFINED 

#include "System.h"
class CollisionSystem : public System
{
public:
	void OnInit() override;
	void Update(float dt) override;
};

#endif