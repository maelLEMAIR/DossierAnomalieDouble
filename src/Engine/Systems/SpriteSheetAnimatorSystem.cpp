#include "SpriteSheetAnimatorSystem.h"
#include "Components/SpriteSheetAnimatorComponent.h"
#include "../../Render/Generic/Shader-Mat/Material.h"

void SpriteSheetAnimatorSystem::OnInit()
{
	SetMaskLoadComponents<SpriteSheetAnimatorComponent>();
}

void SpriteSheetAnimatorSystem::Update(float _dt)
{
	for (auto& [id, pAnimator] : m_vSpriteSheet)
	{
		if (pAnimator->isPlaying)
		{
			if (pAnimator->vTextures.empty() == false)
			{
				pAnimator->elapsedTime += _dt;
				if (pAnimator->elapsedTime >= pAnimator->timePerFrames)
				{
					pAnimator->elapsedTime = 0.0f;
					pAnimator->currentFrame++;

					if (pAnimator->currentFrame >= pAnimator->totalFrames)
					{
						pAnimator->currentFrame = 0;
					}

					if (pAnimator->pTargetMaterial != nullptr)
					{
						pAnimator->pTargetMaterial->SetTexture(pAnimator->textureParameterName, pAnimator->vTextures[pAnimator->currentFrame]);
					}

					else if (pAnimator->pTargetUiMaterial != nullptr)
					{
							pAnimator->pTargetUiMaterial->SetTexture(pAnimator->textureParameterName, pAnimator->vTextures[pAnimator->currentFrame]);
					}
					
					
				}
			}
		}
	}
}

void SpriteSheetAnimatorSystem::OnUpdateMapEntity(uint64_t _maskUpdate, Entity* _pEntity, bool _isNew)
{
	if (_isNew)
	{
		if ((_pEntity->mask & m_mask) == m_mask)
		{
			SpriteSheetAnimatorComponent* pAnimator = m_pOwnerScene->GetComponentType<SpriteSheetAnimatorComponent>(_pEntity);
			if (pAnimator != nullptr)
			{
				m_vSpriteSheet[_pEntity->id] = pAnimator;
			}
		}
	}
	else
	{
		if (m_vSpriteSheet.contains(_pEntity->id))
		{
			m_vSpriteSheet.erase(_pEntity->id);
		}
	}
}