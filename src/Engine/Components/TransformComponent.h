#ifndef TRANSFORM_COMPONENT_H_DEFINED
#define TRANSFORM_COMPONENT_H_DEFINED 

#include "Component.h"
#include "Transform.h"

class TransformComponent : public Component
{
public:
	Transform transform;
	bool hasBeenUpdated = false;
	XMFLOAT3 previousWorldPos = {0.f, 0.f, 0.f};
};

#endif