#ifndef FORCE_SYSTEM_H_DEFINED
#define FORCE_SYSTEM_H_DEFINED

#include "System.h"

class ForceComponent;

class ForceSystem : public System
{
public:
	void OnInit() override;
	static void AddForce(ForceComponent* _forceComponent, XMFLOAT3 const& _force);
	void Update(float dt) override;
	void ResetGrounded();
	
	float m_Gravity = -9.81f;

};

#endif