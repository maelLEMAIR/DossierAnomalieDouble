#ifndef TRANSFORM_COMPONENT_H_DEFINED
#define TRANSFORM_COMPONENT_H_DEFINED 

#include "Component.h"

class PhysicComponent
{
public:
	XMFLOAT3 velocity = { 0.0f, 0.0f, 0.0f };

	bool useGravity = true;
};

#endif