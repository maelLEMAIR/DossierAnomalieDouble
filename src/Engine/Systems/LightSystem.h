#ifndef LIGHT_SYSTEM_H_DEFINED
#define LIGHT_SYSTEM_H_DEFINED

#include "System.h"

class LightComponent;
class Device;

class LightSystem : public System
{
private:
	void OnInit() override;
	void Update(float dt) override;
	void OnUpdateMapEntity(uint64_t _maskUpdate, Entity* _pEntity, bool _isNew = true) override;

	void UpdateLight();

	Device* m_pDevice = nullptr;

	bool m_resend = false;
};

#endif