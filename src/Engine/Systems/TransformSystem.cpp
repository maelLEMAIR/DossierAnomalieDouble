
#ifndef TRANSFORM_SYSTEM_CPP_DEFINED
#define TRANSFORM_SYSTEM_CPP_DEFINED

#include "TransformSystem.h"

#include "Components/TransformComponent.h"
#include "Transform.h"
#include "Components/ParentComponent.h"
#include "Components/ChildrenComponent.h"

#include <iostream>
#include <DirectXMath.h>

void TransformSystem::OnInit()
{
	SetAvoidMaskComponent<ChildrenComponent>();
	SetMaskLoadComponents<TransformComponent>();
}

void TransformSystem::Update(float dt)
{
	for (auto& [id, vComponents] : m_mComponents)
	{
		if (vComponents[0]->GetOwner()->isActive == false) continue;
		if (vComponents[0]->isActive == false) continue;

		TransformComponent* transform = reinterpret_cast<TransformComponent*>(vComponents[0]);
		transform->previousWorldPos = transform->transform.worldPos;
		
		if ((transform->transform.dirty & WORLD_POS) == WORLD_POS)
		{
			transform->transform.localPos = transform->transform.worldPos;
		}
		else if ((transform->transform.dirty & LOCAL_POS) == LOCAL_POS)
		{
			transform->transform.worldPos = transform->transform.localPos;
		}
		
		if ((transform->transform.dirty & WORLD_SCALE) == WORLD_SCALE)
		{
			transform->transform.localScale = transform->transform.worldScale;
		}
		else if ((transform->transform.dirty & LOCAL_SCALE) == LOCAL_SCALE)
		{
			transform->transform.worldScale = transform->transform.localScale;
		}
		
		if ((transform->transform.dirty & WORLD_ROTATE) == WORLD_ROTATE)
		{
			transform->transform.localQuat = transform->transform.worldQuat;
			transform->transform.UpdateLocalRotationFromQuaternion();
		}
		else if ((transform->transform.dirty & LOCAL_ROTATE) == LOCAL_ROTATE)
		{
			transform->transform.worldQuat = transform->transform.localQuat;
			transform->transform.UpdateWorldRotationFromQuaternion();
		}

		transform->transform.UpdateWorldMatrix();

		UpdateTransform(transform->GetOwner(), transform);
	}
}

void TransformSystem::SavePreviousPositions()
{
	for (auto& [id, vComponents] : m_mComponents)
	{
		if (vComponents[0]->GetOwner()->isActive == false) continue;
		if (vComponents[0]->isActive == false) continue;

		TransformComponent* transform = reinterpret_cast<TransformComponent*>(vComponents[0]);
		transform->previousWorldPos = transform->transform.worldPos;
	}
}

XMMATRIX ComposeMatrix(const XMFLOAT3& position, const XMFLOAT4& rotation, const XMFLOAT3& scale)
{
	XMMATRIX scaleMatrix = XMMatrixScaling(scale.x, scale.y, scale.z);

	XMVECTOR quatVector = XMLoadFloat4(&rotation);
	XMMATRIX rotationMatrix = XMMatrixRotationQuaternion(quatVector);

	XMMATRIX translationMatrix = XMMatrixTranslation(position.x, position.y, position.z);

	return scaleMatrix * rotationMatrix * translationMatrix;
}

void TransformSystem::UpdateTransform(Entity* _pEntity, TransformComponent* _transformComponent, TransformComponent* _transformComponentParent)
{
	Transform* transform = &_transformComponent->transform;

	//si parent
	if (_transformComponentParent != nullptr)
	{
		Transform* pTransformParent = &_transformComponentParent->transform;

		bool needsWorldPosRecalc = false;

		//scale
		if ((transform->GetDirty() & WORLD_SCALE) == WORLD_SCALE)
		{
			XMStoreFloat3(&transform->localScale, XMLoadFloat3(&transform->worldScale) / XMLoadFloat3(&pTransformParent->worldScale));

			transform->dirty |= LOCAL_SCALE;
		}
		if ((pTransformParent->GetDirty() & LOCAL_SCALE) == LOCAL_SCALE ||
			(pTransformParent->GetDirty() & WORLD_SCALE) == WORLD_SCALE ||
			(transform->GetDirty() & LOCAL_SCALE) == LOCAL_SCALE)
		{
			XMStoreFloat3(&transform->worldScale, XMLoadFloat3(&transform->localScale) * XMLoadFloat3(&pTransformParent->worldScale));
			needsWorldPosRecalc = true;

			transform->dirty |= WORLD_SCALE;
		}

		//rotation
		if ((transform->GetDirty() & WORLD_ROTATE) == WORLD_ROTATE)
		{
			XMVECTOR worldQuat = XMLoadFloat4(&transform->worldQuat);
			XMVECTOR parentQuat = XMLoadFloat4(&pTransformParent->worldQuat);
			XMVECTOR parentQuatInv = XMQuaternionInverse(parentQuat);
			XMStoreFloat4(&transform->localQuat,	XMQuaternionMultiply(parentQuatInv, worldQuat));
			transform->UpdateLocalRotationFromQuaternion();
		}
		if ((pTransformParent->GetDirty() & LOCAL_ROTATE) == LOCAL_ROTATE ||
			(pTransformParent->GetDirty() & WORLD_ROTATE) == WORLD_ROTATE ||
			(transform->GetDirty() & LOCAL_ROTATE) == LOCAL_ROTATE)
		{
			XMVECTOR localQuat = XMLoadFloat4(&transform->localQuat);
			XMVECTOR parentQuat = XMLoadFloat4(&pTransformParent->worldQuat);
			XMStoreFloat4(&transform->worldQuat,	XMQuaternionMultiply(localQuat, parentQuat));
			transform->UpdateWorldRotationFromQuaternion();
			needsWorldPosRecalc = true;
		}

		//pos
		if ((transform->GetDirty() & WORLD_POS) == WORLD_POS && (pTransformParent->GetDirty() & LOCAL_POS) == LOCAL_POS)
		{
			XMStoreFloat3(&transform->worldPos, XMLoadFloat3(&transform->localPos) + XMLoadFloat3(&pTransformParent->worldPos));

			transform->dirty |= WORLD_POS;
		}
		else if ((transform->GetDirty() & WORLD_POS) == WORLD_POS)
		{
			XMVECTOR offset = XMVectorSubtract(XMLoadFloat3(&transform->worldPos), XMLoadFloat3(&pTransformParent->worldPos));
			XMVECTOR parentQuatInv = XMQuaternionInverse(XMLoadFloat4(&pTransformParent->worldQuat));
			XMVECTOR unrotated = XMVector3Rotate(offset, parentQuatInv);
			XMStoreFloat3(&transform->localPos, unrotated / XMLoadFloat3(&pTransformParent->worldScale));

			transform->dirty |= LOCAL_POS;
		}
		else if ((pTransformParent->GetDirty() & LOCAL_POS) == LOCAL_POS ||
			(pTransformParent->GetDirty() & WORLD_POS) == WORLD_POS)
		{
			needsWorldPosRecalc = true;
		}

		if (needsWorldPosRecalc)
		{
			XMVECTOR localPosScaled = XMVectorMultiply(XMLoadFloat3(&transform->localPos), XMLoadFloat3(&pTransformParent->worldScale));
			XMVECTOR rotatedPos = XMVector3Rotate(localPosScaled, XMLoadFloat4(&pTransformParent->worldQuat));
			XMStoreFloat3(&transform->worldPos, XMVectorAdd(rotatedPos, XMLoadFloat3(&pTransformParent->worldPos)));

			transform->dirty |= WORLD_POS;
		}	
	}

	if (transform->dirty != 0) //some change have been applied
		_transformComponent->hasBeenUpdated = true;
	else
		_transformComponent->hasBeenUpdated = false;

	//worldmatrix
	XMMATRIX localMatrix = ComposeMatrix(
		transform->GetLocalPosition(),
		transform->GetLocalRotation(),
		transform->GetLocalScale()
	);

	XMMATRIX parentWorld = XMMatrixIdentity();

	if (_transformComponentParent != nullptr)
		parentWorld = XMLoadFloat4x4(&_transformComponentParent->transform.GetWorldMatrix());

	XMMATRIX worldMatrix = localMatrix * parentWorld;

	transform->UpdateWorldMatrix();
	
	ParentComponent* parent = m_pOwnerScene->GetComponentType<ParentComponent>(_pEntity);
	if (parent != nullptr)
	{
		for (Entity* pEntity : parent->vChildrens)
		{
			TransformComponent* pTransformChild = m_pOwnerScene->GetComponentType<TransformComponent>(pEntity);

			if (pTransformChild == nullptr) continue;

			UpdateTransform(pEntity, pTransformChild, _transformComponent);
		}
	}

	transform->dirty = 0;
}

#endif