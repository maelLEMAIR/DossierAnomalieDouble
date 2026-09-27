#ifndef FORCE_COMPONENT_H_DEFINED
#define FORCE_COMPONENT_H_DEFINED

#include "Component.h"

class ForceComponent : public Component
{
public:
	XMFLOAT3 velocity = { 0.0f, 0.0f, 0.0f };

	bool useGravity = false;
    float drag = 0.0001f;
	bool isGrounded = false;
	bool wasGroundedLastFrame = false;
	Vector<XMFLOAT3> appliedForce;
};

#endif