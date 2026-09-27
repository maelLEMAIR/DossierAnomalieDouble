#ifndef LIGHT_SYSTEM_CPP_DEFINED
#define LIGHT_SYSTEM_CPP_DEFINED

#include "LightSystem.h"
#include "Generic/Base/Device.h"
#include "../Components/LightComponent.h"

void LightSystem::OnInit()
{
	SetMaskLoadComponents<LightComponent>();

	m_pDevice = EngineManager::GetDevice();
}

void LightSystem::Update(float dt)
{
	for (auto& [id, vComponent] : m_mComponents)
	{
		LightComponent* pLight = reinterpret_cast<LightComponent*>(vComponent[0]);

		if (pLight->m_isUpdate)
		{
			m_resend = true;
			pLight->m_isUpdate = false;
		}
	}

	UpdateLight();
}

void LightSystem::OnUpdateMapEntity(uint64_t _maskUpdate, Entity* _pEntity, bool _isNew)
{
	if (_isNew == false)
		m_resend = true;
}

void LightSystem::UpdateLight()
{
	if (m_resend) 
	{
		//resent lightdata
		std::vector<LightDescriptor> vLights;

		for (auto [id, vComponents] : m_mComponents)
		{
			if (vComponents[0]->GetOwner()->isActive == false) continue;
			if (vComponents[0]->isActive == false) continue;

			LightComponent* pLight = reinterpret_cast<LightComponent*>(vComponents[0]);

			LightDescriptor light;
			light.type = pLight->m_type;
			light.light.Strength = { pLight->m_strength, pLight->m_strength, pLight->m_strength };
			light.light.FalloffStart = pLight->m_falloffStart;
			light.light.Direction = pLight->m_direction;
			light.light.FalloffEnd = pLight->m_falloffEnd;
			light.light.Position = pLight->m_position;
			light.light.SpotPower = pLight->m_spotPower;
			light.light.Color = pLight->m_color;

			vLights.push_back(light);
		}

		m_pDevice->SetLights(vLights);
		std::cout << "Update light : " << vLights.size() << std::endl;;

		m_resend = false;
	}
}

#endif