#ifndef STATE_MACHINE_SYSTEM_H_DEFINED
#define STATE_MACHINE_SYSTEM_H_DEFINED

#include "System.h"

class StateMachineSystem : public System
{
public:
	void OnInit() override;
private:
	void Update(float _dt) override;
};

#endif