#ifndef CHILDREN_COMPONENT_H_DEFINED
#define CHILDREN_COMPONENT_H_DEFINED 

#include "Component.h"
#include "Scene.h"

class ChildrenComponent : public Component
{
public:
	void SetParent(Entity* _pParent)
	{
		if (_pParent == nullptr) return;

		m_pParent = _pParent;

		TransformComponent* pParent = SceneManager::GetSceneWithName(m_pParent->pScene->GetName())->GetComponentType<TransformComponent>(m_pParent);
		TransformComponent* pChild= SceneManager::GetSceneWithName(m_pEntity->pScene->GetName())->GetComponentType<TransformComponent>(m_pEntity);

		pChild->transform.worldPos = pChild->transform.localPos;

		XMStoreFloat3(&pChild->transform.localPos, XMLoadFloat3(&pChild->transform.worldPos) - XMLoadFloat3(&pParent->transform.worldPos));
		pChild->transform.dirty |= LOCAL_POS;

		pChild->transform.worldScale = pChild->transform.localScale;

		XMStoreFloat3(&pChild->transform.localScale, XMLoadFloat3(&pChild->transform.worldScale) / XMLoadFloat3(&pParent->transform.worldScale));
		pChild->transform.dirty |= LOCAL_SCALE;


		pChild->transform.worldRot = pChild->transform.localRot;

		XMStoreFloat4x4(&pChild->transform.localRot, XMLoadFloat4x4(&pParent->transform.invMatrix) * XMLoadFloat4x4(&pChild->transform.worldRot));

		pChild->transform.dirty |= LOCAL_ROTATE;
	}

private:
	Entity* m_pParent = nullptr;
};

#endif