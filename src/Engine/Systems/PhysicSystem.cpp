#ifndef PHYSIC_SYSTEM_HPP_INCLUDED
#define PHYSIC_SYSTEM_HPP_INCLUDED

#include "PhysicSystem.h"
#include "../Components/Collider.h"
#include "../Components/TransformComponent.h"
#include "../Components/MeshRenderer.h"
#include "../Components/DebugColliderComponent.h"
#include "../Components/Collision.h"
#include "../Components/ForceComponent.h"
#include "../RessourceManager.h"
#include "RayCast.h"

#include <iostream>
#include <set>
#include <unordered_map>
#include <unordered_set>
#include <stack>


void PhysicSystem::OnInit()
{
	SetMaskLoadComponents<TransformComponent, Collider>();
}

static inline uint64_t CellKey(int x, int y, int z)
{
	return ((uint64_t)(uint32_t)x)
		^ ((uint64_t)(uint32_t)y << 20)
		^ ((uint64_t)(uint32_t)z << 40);
}

static inline uint64_t PairKey(int a, int b)
{
	int lo = min(a, b);
	int hi = max(a, b);
	return ((uint64_t)(uint32_t)lo) | ((uint64_t)(uint32_t)hi << 32);
}

bool Contains(std::vector<Entity*> _vector, Entity* _entity)
{
	for (Entity* entity : _vector)
	{
		if (entity->id == _entity->id)
			return true;
	}
	return false;
}

void PhysicSystem::Update(float dt)
{
	for (ColliderData* pCollider : m_vColliders)
	{
		pCollider->vOldCollisions = pCollider->pCollider->vCurrentCollision;
		pCollider->pCollider->vCurrentCollision.clear();

		if (pCollider->pTransform->hasBeenUpdated)
		{
			UpdateColliderInCell(pCollider);
		}
	}

	std::unordered_set<uint64_t> checkedPairs;
	checkedPairs.reserve(m_vColliders.size() * 4);

	for (Cell* cell : m_vGrid)
	{
		for (int i = 0; i < (int)cell->vColliders.size(); i++)
		{
			ColliderData* collider1 = cell->vColliders[i];
			if (collider1->IsActive() == false) continue;

			for (int j = i + 1; j < (int)cell->vColliders.size(); j++)
			{
				if (cell->vColliders[i]->id == cell->vColliders[j]->id) continue;

				ColliderData* collider2 = cell->vColliders[j];
				if (collider2->IsActive() == false) continue;

				uint64_t key = PairKey(collider1->id, collider2->id);
				if (checkedPairs.count(key)) continue;
				checkedPairs.insert(key);

				ColliderType::ColliderType type1 = collider1->pCollider->colliderType;
				ColliderType::ColliderType type2 = collider2->pCollider->colliderType;

				if (type1 == ColliderType::SPHERE && type2 == ColliderType::SPHERE)
					SphereSphere(collider1, collider2);
				else if (type1 == ColliderType::SPHERE && type2 == ColliderType::BOX)
					BoxSphere(collider2, collider1);
				else if (type1 == ColliderType::BOX && type2 == ColliderType::SPHERE)
					BoxSphere(collider1, collider2);
				else if (type1 == ColliderType::BOX && type2 == ColliderType::BOX)
					BoxBox(collider1, collider2);
			}
		}
	}

	for (ColliderData* pCollider : m_vColliders)
	{
		for (Entity* oldEntity : pCollider->vOldCollisions)
		{
			if (Contains(pCollider->pCollider->vCurrentCollision, oldEntity) == false)
			{
				Collision* c1 = m_pOwnerScene->GetComponentType<Collision>(pCollider->pCollider->GetOwner());
				if (c1 == nullptr)
					c1 = m_pOwnerScene->AddComponent<Collision>(pCollider->pCollider->GetOwner());
				c1->mOthers[CollisionType::EXIT].push_back(oldEntity);
			}
		}
	}

	for (int i = (int)m_vGrid.size() - 1; i >= 0; i--)
	{
		if (m_vGrid[i]->vColliders.size() == 0)
		{
			uint64_t key = CellKey(
				(int)m_vGrid[i]->pos.x,
				(int)m_vGrid[i]->pos.y,
				(int)m_vGrid[i]->pos.z);
			m_mapGrid.erase(key);

			delete m_vGrid[i]->pCollider->pCollider;
			delete m_vGrid[i]->pCollider->pTransform;
			delete m_vGrid[i]->pCollider;
			delete m_vGrid[i];

			m_vGrid[i] = m_vGrid.back();
			m_vGrid.pop_back();
		}
	}
}

void PhysicSystem::CheckRayCast(RayCast _rc, HitPoint& _hp)
{	
	XMFLOAT3 currentPos = _rc.origin;
	float currentDist = 0.0f;

	while (currentDist <= _rc.maxDist)
	{
		ColliderData* pObj = GetObjAtPos(currentPos);
		if (pObj == nullptr || _rc.avoidTag.contains(pObj->pCollider->GetOwner()->tag))
		{
			currentPos += _rc.dir * _rc.pas;
			currentDist += _rc.pas;
		}
		else
		{
			_hp.pHitEntity = pObj->pCollider->GetOwner();
			_hp.hitPoint = currentPos;
			return;
		}
	}
}

XMFLOAT3 GetRow(const XMMATRIX& m, int row)
{
	XMFLOAT4X4 m4;
	XMStoreFloat4x4(&m4, m);
	return { m4.m[row][0], m4.m[row][1], m4.m[row][2] };
}

//void PhysicSystem::CheckRayCast(RayCast _rc, HitPoint& _hp)
//{
//	//get begining cell
//	int posX = (int)(round(_rc.origin.x / SIZE_CELL) * SIZE_CELL);
//	int posY = (int)(round(_rc.origin.y / SIZE_CELL) * SIZE_CELL);
//	int posZ = (int)(round(_rc.origin.z / SIZE_CELL) * SIZE_CELL);
//	
//
//	int stepX = (_rc.dir.x >= 0) ? 1 : -1;
//	int stepY = (_rc.dir.y >= 0) ? 1 : -1;
//	int stepZ = (_rc.dir.z >= 0) ? 1 : -1;
//
//	float tDeltaX = (fabs(_rc.dir.x) < 1e-8f) ? FLT_MAX : fabs((float)SIZE_CELL / _rc.dir.x);
//	float tDeltaY = (fabs(_rc.dir.y) < 1e-8f) ? FLT_MAX : fabs((float)SIZE_CELL / _rc.dir.y);
//	float tDeltaZ = (fabs(_rc.dir.z) < 1e-8f) ? FLT_MAX : fabs((float)SIZE_CELL / _rc.dir.z);
//
//	auto nextBoundary = [](float origin, float dir, int cell, int step, float size) -> float {
//		float boundary = (step > 0) ? (cell + 1) * size : cell * size;
//		return (fabs(dir) < 1e-8f) ? FLT_MAX : fabs((boundary - origin) / dir);
//		};
//
//	float tMaxX = nextBoundary(_rc.origin.x, _rc.dir.x, posX, stepX, SIZE_CELL);
//	float tMaxY = nextBoundary(_rc.origin.y, _rc.dir.y, posY, stepY, SIZE_CELL);
//	float tMaxZ = nextBoundary(_rc.origin.z, _rc.dir.z, posZ, stepZ, SIZE_CELL);
//
//	float closestT = FLT_MAX;
//
//	std::unordered_set<ColliderData*> tested;
//
//	bool reachMaxDist = false;
//
//	while(reachMaxDist == false)
//	{
//		XMFLOAT3 pos = {
//			(float)(posX * SIZE_CELL),
//			(float)(posY * SIZE_CELL),
//			(float)(posZ * SIZE_CELL)
//		};
//		Cell* pCell = GetOrCreateCell({ pos });
//
//		if (pCell != nullptr)
//		{
//			for (ColliderData* pCollider : pCell->vColliders)
//			{
//				if (tested.count(pCollider)) continue;
//				tested.insert(pCollider);
//
//				if (pCollider->pCollider->colliderType == ColliderType::SPHERE)
//				{
//					XMFLOAT3 oc = pCollider->pTransform->transform.GetWorldPosition() - _rc.origin;
//					float b = Dot(oc, _rc.dir);
//					float scale = pCollider->pTransform->transform.GetWorldScale().x
//						* pCollider->pCollider->scale.x;
//					scale *= 0.5f;
//					float c = Dot(oc, oc) - scale * scale;
//					float delta = b * b - c;
//
//					if (delta < 0) continue;
//
//					float t = b - sqrt(delta);
//					if (t < 0) t = b + sqrt(delta);
//					if (t < 0) continue;
//
//					if (t < closestT)
//					{
//						if (_rc.avoidTag.contains(pCollider->pCollider->GetOwner()->tag) == false)
//						{
//							closestT = t;
//							_hp.pHitEntity = pCollider->pCollider->GetOwner();
//							_hp.hitPoint = _rc.origin + t * _rc.dir;
//						}
//					}
//				}
//				else if (pCollider->pCollider->colliderType == ColliderType::BOX)
//				{
//					XMFLOAT3 boxPos = pCollider->pTransform->transform.GetWorldPosition();
//					XMMATRIX boxRotMat = pCollider->pTransform->transform.GetWorldRotMatrix();
//
//					XMFLOAT3 worldScale = pCollider->pTransform->transform.GetWorldScale();
//					XMFLOAT3 collScale = pCollider->pCollider->scale;
//					XMFLOAT3 half = worldScale * collScale * 0.5f;
//
//					pCollider->pTransform->transform.UpdateInvMatrix();
//					XMFLOAT4X4 invMatFloat = pCollider->pTransform->transform.GetInvMatrix();
//					XMMATRIX invRot = XMLoadFloat4x4(&invMatFloat);
//
//					XMFLOAT3 diff = _rc.origin - boxPos;
//
//					XMFLOAT3 localOrigin = { Dot(diff, GetRow(invRot, 0)), Dot(diff, GetRow(invRot, 1)), Dot(diff, GetRow(invRot, 2)) };
//
//					XMFLOAT3 localDir = { Dot(_rc.dir, GetRow(invRot, 0)), Dot(_rc.dir, GetRow(invRot, 1)), Dot(_rc.dir, GetRow(invRot, 2)) };
//
//					float tMin = -FLT_MAX;
//					float tMax = FLT_MAX;
//					bool hit = true;
//
//					float localOriginArr[3] = { localOrigin.x, localOrigin.y, localOrigin.z };
//					float localDirArr[3] = { localDir.x, localDir.y, localDir.z };
//					float halfArr[3] = { half.x, half.y, half.z };
//
//					for (int i = 0; i < 3; i++)
//					{
//						if (fabs(localDirArr[i]) < 1e-8f)
//						{
//							if (fabs(localOriginArr[i]) > halfArr[i])
//							{
//								hit = false;
//								break;
//							}
//						}
//						else
//						{
//							float invD = 1.0f / localDirArr[i];
//							float t1 = (-halfArr[i] - localOriginArr[i]) * invD;
//							float t2 = (halfArr[i] - localOriginArr[i]) * invD;
//
//							if (t1 > t2)
//							{
//								float temp = t1;
//								t1 = t2;
//								t2 = temp;
//							}
//
//							tMin = max(tMin, t1);
//							tMax = min(tMax, t2);
//
//							if (tMin > tMax)
//							{
//								hit = false;
//								break;
//							}
//						}
//					}
//
//					if (!hit) continue;
//					if (tMax < 0) continue;
//
//					float t = (tMin >= 0) ? tMin : tMax;
//
//					if (t < closestT)
//					{
//						if (_rc.avoidTag.contains(pCollider->pCollider->GetOwner()->tag) == false)
//						{
//							closestT = t;
//							_hp.pHitEntity = pCollider->pCollider->GetOwner();
//							_hp.hitPoint = _rc.origin + t * _rc.dir;
//						}
//					}
//				}
//			}
//		}
//
//		float tNextBoundary;
//
//		if (tMaxX <= tMaxY && tMaxX <= tMaxZ)
//			tNextBoundary = tMaxX;
//		else if (tMaxY <= tMaxZ)
//			tNextBoundary = tMaxY;
//		else
//			tNextBoundary = tMaxZ;
//
//		if (tNextBoundary > _rc.maxDist) break;
//
//		if (tNextBoundary > closestT)
//			break;
//
//		if (tMaxX < tMaxY && tMaxX < tMaxZ)
//		{
//			posX += stepX;
//			tMaxX += tDeltaX;
//		}
//		else if (tMaxY < tMaxZ)
//		{
//			posY += stepY;
//			tMaxY += tDeltaY;
//		}
//		else
//		{
//			posZ += stepZ;
//			tMaxZ += tDeltaZ;
//		}
//	}	
//}

void PhysicSystem::OnUpdateMapEntity(uint64_t _maskUpdate, Entity* _pEntity, bool _isNew)
{
	if (_isNew)
	{
		if (_maskUpdate != COMPONENT_MASK(Collider)) return;

		ColliderData* newCollider = new ColliderData;
		newCollider->pCollider  = m_pOwnerScene->GetComponentType<Collider>(_pEntity);
		newCollider->pTransform = m_pOwnerScene->GetComponentType<TransformComponent>(_pEntity);
		newCollider->id = _pEntity->id;

		m_vColliders.push_back(newCollider);

		DebugColliderComponent* newDebugCollider = m_pOwnerScene->AddComponent<DebugColliderComponent>(_pEntity);
		if (newCollider->pCollider->colliderType == ColliderType::SPHERE)
			newDebugCollider->pGeo = RessourceManager::GetGeometry("SphereWireframe");
		else if (newCollider->pCollider->colliderType == ColliderType::BOX)
			newDebugCollider->pGeo = RessourceManager::GetGeometry("CubeWireframe");
		else
			newDebugCollider->pGeo = RessourceManager::GetGeometry("CubeWireframe");

		newDebugCollider->pColliderData = newCollider;
		UpdateColliderInCell(newCollider);
	}
	else
	{
		for (Cell* cell : m_vGrid)
		{
			for (int i = (int)cell->vColliders.size() - 1; i >= 0; i--)
			{
				if (cell->vColliders[i]->id == _pEntity->id)
					cell->vColliders.erase(cell->vColliders.begin() + i);
			}
		}

		int i = 0;
		for (ColliderData* collider : m_vColliders)
		{
			if (collider->id == _pEntity->id)
			{
				m_vColliders.erase(m_vColliders.begin() + i);
				delete collider;
				return;
			}
			i++;
		}
	}
}

static void CancelNormalVelocity(ForceComponent* f, XMFLOAT3 normal)
{
	if (f == nullptr) return;
	XMFLOAT3 vel = f->velocity;
	float proj = Dot(vel, normal);
	if (proj < 0)
		f->velocity = vel - normal * proj;
}

static void ApplyRestThreshold(ForceComponent* f)
{
	if (f == nullptr) return;
	XMFLOAT3& vel = f->velocity;
	float speedSq = vel.x * vel.x + vel.y * vel.y + vel.z * vel.z;
	if (speedSq < 0.0001f)
		vel = { 0.0f, 0.0f, 0.0f };
}

void PhysicSystem::SphereSphere(ColliderData* colliderData1, ColliderData* colliderData2)
{
	if (IsCollisionSphereSphere(colliderData1, colliderData2) == false) return;

	AddCollision(colliderData1, colliderData2);

	XMFLOAT3 pos1 = colliderData1->pTransform->transform.GetWorldPosition();
	XMFLOAT3 pos2 = colliderData2->pTransform->transform.GetWorldPosition();

	float rayon1 = colliderData1->pTransform->transform.GetWorldScale().x / 2.0f * colliderData1->pCollider->scale.x;
	float rayon2 = colliderData2->pTransform->transform.GetWorldScale().x / 2.0f * colliderData2->pCollider->scale.x;

	float distance = DistanceSq(pos2, pos1);
	float minDist  = rayon1 + rayon2;

	XMFLOAT3 normalF = Normalize(pos2 - pos1);

	bool notTrigger = (colliderData1->pCollider->isTrigger == false &&
	                   colliderData2->pCollider->isTrigger == false);

	if (notTrigger == false)
		return;
	
	if (notTrigger)
	{
		float overlap = (minDist - distance) / 2.0f;

		if (colliderData1->pCollider->isStatic == false)
		{
			XMFLOAT3 newPos1 = pos1 - normalF * overlap;
			colliderData1->pTransform->transform.SetWorldPosition(newPos1);
		}
		if (colliderData2->pCollider->isStatic == false)
		{
			XMFLOAT3 newPos2 = pos2 + normalF * overlap;
			colliderData2->pTransform->transform.SetWorldPosition(newPos2);
		}
	}

	ForceComponent* f1 = m_pOwnerScene->GetComponentType<ForceComponent>(colliderData1->pCollider->GetOwner());
	ForceComponent* f2 = m_pOwnerScene->GetComponentType<ForceComponent>(colliderData2->pCollider->GetOwner());

	bool c1CanBounce = colliderData1->pCollider->canBounce;
	bool c2CanBounce = colliderData2->pCollider->canBounce;

	XMFLOAT3 rv;
	if (f1 != nullptr && f2 != nullptr)
		XMStoreFloat3(&rv, XMLoadFloat3(&f2->velocity) - XMLoadFloat3(&f1->velocity));
	else if (f1 != nullptr)
		XMStoreFloat3(&rv, -XMLoadFloat3(&f1->velocity));
	else if (f2 != nullptr)
		rv = f2->velocity;
	else
		return;

	float velAlongNormal = rv.x * normalF.x + rv.y * normalF.y + rv.z * normalF.z;
	if (velAlongNormal > 0) return;

	if (!c1CanBounce && !c2CanBounce)
	{
		CancelNormalVelocity(f1, -normalF);
		CancelNormalVelocity(f2,  normalF);
		ApplyRestThreshold(f1);
		ApplyRestThreshold(f2);
		return;
	}

	float restitution = 0.8f;
	float cImpulse = -(1.0f + restitution) * velAlongNormal / 2.0f;
	XMFLOAT3 impulse = normalF * cImpulse;

	int factor = (f1 == nullptr || f2 == nullptr) ? 2 : 1;

	if (f1 != nullptr && c1CanBounce)
		XMStoreFloat3(&f1->velocity, XMLoadFloat3(&f1->velocity) - XMLoadFloat3(&impulse) * (float)factor);
	if (f2 != nullptr && c2CanBounce)
		XMStoreFloat3(&f2->velocity, XMLoadFloat3(&f2->velocity) + XMLoadFloat3(&impulse) * (float)factor);

	ApplyRestThreshold(f1);
	ApplyRestThreshold(f2);
}

void PhysicSystem::BoxSphere(ColliderData* boxData, ColliderData* sphereData)
{
	XMFLOAT3 size = boxData->pTransform->transform.GetWorldScale() * boxData->pCollider->scale;

	float halfX = size.x / 2.0f;
	float halfY = size.y / 2.0f;
	float halfZ = size.z / 2.0f;

	float radius = sphereData->pTransform->transform.GetWorldScale().x
		* sphereData->pCollider->scale.x * 0.5f;

	XMMATRIX worldBox    = XMLoadFloat4x4(&boxData->pTransform->transform.GetWorldMatrix());
	XMMATRIX invBoxWorld = XMMatrixInverse(nullptr, worldBox);

	XMFLOAT3 spherePos = sphereData->pTransform->transform.GetWorldPosition();

	XMVECTOR localSpherePosV = XMVector3Transform(XMLoadFloat3(&spherePos), invBoxWorld);
	XMFLOAT3 localSpherePos;
	XMStoreFloat3(&localSpherePos, localSpherePosV);

	float minRadius = sqrtf(halfX * halfX + halfY * halfY + halfZ * halfZ);
	float distLocal;
	XMStoreFloat(&distLocal, XMVector3Length(localSpherePosV));
	if (distLocal > minRadius + radius) return;

	float closestX = Clamp(localSpherePos.x, -halfX, halfX);
	float closestY = Clamp(localSpherePos.y, -halfY, halfY);
	float closestZ = Clamp(localSpherePos.z, -halfZ, halfZ);
	XMFLOAT3 localClosest = { closestX, closestY, closestZ };

	XMVECTOR worldClosestV = XMVector3Transform(XMLoadFloat3(&localClosest), worldBox);

	XMVECTOR toSphere = XMLoadFloat3(&spherePos) - worldClosestV;
	float distance;
	XMStoreFloat(&distance, XMVector3Length(toSphere));

	if (distance >= radius) return;

	AddCollision(boxData, sphereData);

	XMVECTOR dir;
	if (distance > 0.0001f)
		dir = toSphere / distance;
	else
		dir = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	float penetration = radius - distance;

	bool notTrigger = (boxData->pCollider->isTrigger == false &&
	                   sphereData->pCollider->isTrigger == false);

	if (notTrigger == false)
		return;
	
	if (notTrigger)
	{
		if (boxData->pCollider->isStatic == false)
		{
			XMFLOAT3 invPush;
			XMStoreFloat3(&invPush, dir * -(penetration / 2.0f));
			boxData->pTransform->transform.MoveWorld(invPush);
		}
		if (sphereData->pCollider->isStatic == false)
		{
			XMFLOAT3 push;
			XMStoreFloat3(&push, dir * (penetration / 2.0f));
			sphereData->pTransform->transform.MoveWorld(push);
		}
	}

	ForceComponent* f1 = m_pOwnerScene->GetComponentType<ForceComponent>(boxData->pCollider->GetOwner());
	ForceComponent* f2 = m_pOwnerScene->GetComponentType<ForceComponent>(sphereData->pCollider->GetOwner());

	float overlapPosX = (halfX + radius) - ( localSpherePos.x);
	float overlapNegX = (halfX + radius) - (-localSpherePos.x);
	float overlapPosY = (halfY + radius) - ( localSpherePos.y);
	float overlapNegY = (halfY + radius) - (-localSpherePos.y);
	float overlapPosZ = (halfZ + radius) - ( localSpherePos.z);
	float overlapNegZ = (halfZ + radius) - (-localSpherePos.z);

	float minOverlap = overlapPosX;
	XMFLOAT3 contactNormal = { 1.0f, 0.0f, 0.0f };

	if (overlapNegX < minOverlap) { minOverlap = overlapNegX; contactNormal = {-1.0f,  0.0f,  0.0f}; }
	if (overlapPosY < minOverlap) { minOverlap = overlapPosY; contactNormal = { 0.0f,  1.0f,  0.0f}; }
	if (overlapNegY < minOverlap) { minOverlap = overlapNegY; contactNormal = { 0.0f, -1.0f,  0.0f}; }
	if (overlapPosZ < minOverlap) { minOverlap = overlapPosZ; contactNormal = { 0.0f,  0.0f,  1.0f}; }
	if (overlapNegZ < minOverlap) { minOverlap = overlapNegZ; contactNormal = { 0.0f,  0.0f, -1.0f}; }

	XMMATRIX boxRot = boxData->pTransform->transform.GetWorldRotMatrix();
	XMVECTOR normal = XMVector3Normalize(
		XMVector3TransformNormal(XMLoadFloat3(&contactNormal), boxRot));
	XMFLOAT3 normalF;
	XMStoreFloat3(&normalF, normal);

	if (normalF.y > 0.9f)
	{
		if (f2 != nullptr) f2->isGrounded = true;
	}
	else if (normalF.y < -0.9f)
	{
		if (f1 != nullptr) f1->isGrounded = true;
	}

	XMFLOAT3 rv;
	if (f1 != nullptr && f2 != nullptr)
		XMStoreFloat3(&rv, XMLoadFloat3(&f2->velocity) - XMLoadFloat3(&f1->velocity));
	else if (f1 != nullptr)
		XMStoreFloat3(&rv, -XMLoadFloat3(&f1->velocity));
	else if (f2 != nullptr)
		rv = f2->velocity;
	else
		return;

	float velAlongNormal = rv.x * normalF.x + rv.y * normalF.y + rv.z * normalF.z;
	if (velAlongNormal > 0) return;

	bool boxCanBounce    = boxData->pCollider->canBounce;
	bool sphereCanBounce = sphereData->pCollider->canBounce;

	if (!boxCanBounce && !sphereCanBounce)
	{
		CancelNormalVelocity(f1, -normalF);
		CancelNormalVelocity(f2,  normalF);
		ApplyRestThreshold(f1);
		ApplyRestThreshold(f2);
		return;
	}

	float restitution = 0.8f;
	float cImpulse = -(1.0f + restitution) * velAlongNormal / 2.0f;
	XMFLOAT3 impulse;
	XMStoreFloat3(&impulse, normal * cImpulse);

	int factor = (f1 == nullptr || f2 == nullptr) ? 2 : 1;

	if (f1 != nullptr && boxCanBounce)
		XMStoreFloat3(&f1->velocity, XMLoadFloat3(&f1->velocity) - XMLoadFloat3(&impulse) * (float)factor);
	if (f2 != nullptr && sphereCanBounce)
		XMStoreFloat3(&f2->velocity, XMLoadFloat3(&f2->velocity) + XMLoadFloat3(&impulse) * (float)factor);

	ApplyRestThreshold(f1);
	ApplyRestThreshold(f2);
}

bool PhysicSystem::IsCollisionSphereSphere(ColliderData* colliderData1, ColliderData* colliderData2)
{
	XMFLOAT3 pos1 = colliderData1->pTransform->transform.GetWorldPosition();
	XMFLOAT3 pos2 = colliderData2->pTransform->transform.GetWorldPosition();

	float rayon1 = colliderData1->pTransform->transform.GetWorldScale().x / 2.0f * colliderData1->pCollider->scale.x;
	float rayon2 = colliderData2->pTransform->transform.GetWorldScale().x / 2.0f * colliderData2->pCollider->scale.x;

	float distance = Length(pos2 - pos1);
	return distance <= rayon1 + rayon2;
}

bool PhysicSystem::IsCollisionBoxSphere(ColliderData* boxData, ColliderData* sphereData)
{
	XMFLOAT3 size = boxData->pTransform->transform.GetWorldScale() * boxData->pCollider->scale;

	float radius = sphereData->pTransform->transform.GetWorldScale().x
		* sphereData->pCollider->scale.x * 0.5f;

	float halfX = size.x / 2.0f;
	float halfY = size.y / 2.0f;
	float halfZ = size.z / 2.0f;

	XMFLOAT3 boxPos    = boxData->pTransform->transform.GetWorldPosition();
	XMFLOAT3 spherePos = sphereData->pTransform->transform.GetWorldPosition();

	float minRadius = sqrtf(halfX*halfX + halfY*halfY + halfZ*halfZ);
	XMVECTOR diff = XMLoadFloat3(&boxPos) - XMLoadFloat3(&spherePos);
	float distance;
	XMStoreFloat(&distance, XMVector3Length(diff));
	if (distance > minRadius + radius) return false;

	boxData->pTransform->transform.UpdateWorldMatrix();
	boxData->pTransform->transform.UpdateInvMatrix();

	XMFLOAT3 boxScale = boxData->pTransform->transform.GetWorldScale();

	// Matrice monde sans scale
	XMMATRIX worldBox = XMLoadFloat4x4(&boxData->pTransform->transform.GetWorldMatrix());
	
	// Diviser chaque axe par sa scale pour enlever la scale
	worldBox.r[0] = XMVectorScale(worldBox.r[0], 1.0f / boxScale.x);
	worldBox.r[1] = XMVectorScale(worldBox.r[1], 1.0f / boxScale.y);
	worldBox.r[2] = XMVectorScale(worldBox.r[2], 1.0f / boxScale.z);
	
	XMMATRIX invBoxWorld = XMMatrixInverse(nullptr, worldBox);
	XMVECTOR localSphereV = XMVector3TransformCoord(XMLoadFloat3(&spherePos), invBoxWorld);
	XMFLOAT3 localSphere;
	XMStoreFloat3(&localSphere, localSphereV);

	float closestX = Clamp(localSphere.x, -halfX, halfX);
	float closestY = Clamp(localSphere.y, -halfY, halfY);
	float closestZ = Clamp(localSphere.z, -halfZ, halfZ);
	XMFLOAT3 localClosest = { closestX, closestY, closestZ };

	XMVECTOR worldClosest = XMVector3Transform(XMLoadFloat3(&localClosest), worldBox);
	XMVECTOR toSphere = XMLoadFloat3(&spherePos) - worldClosest;
	XMStoreFloat(&distance, XMVector3Length(toSphere));

	return distance <= radius;
}

static void ProjectOBB(
	const XMFLOAT3& center,
	const XMFLOAT3 axes[3],
	const XMFLOAT3& halfExtents,
	const XMFLOAT3& axis,
	float& outMin, float& outMax)
{
	float c = Dot(center, axis);
	float r = 0.0f;
	for (int i = 0; i < 3; i++)
	{
		float extent = (&halfExtents.x)[i];
		r += abs(Dot(axes[i], axis)) * extent;
	}
	outMin = c - r;
	outMax = c + r;
}

static bool TestAxis(
	const XMFLOAT3& axis,
	const XMFLOAT3& center1, const XMFLOAT3 axes1[3], const XMFLOAT3& half1,
	const XMFLOAT3& center2, const XMFLOAT3 axes2[3], const XMFLOAT3& half2,
	float& minOverlap, XMFLOAT3& bestAxis)
{
	if (Length(axis) < 1e-10f)
		return true;

	XMFLOAT3 normAxis = Normalize(axis);

	float min1, max1, min2, max2;
	ProjectOBB(center1, axes1, half1, normAxis, min1, max1);
	ProjectOBB(center2, axes2, half2, normAxis, min2, max2);

	float overlap = min(max1, max2) - max(min1, min2);
	if (overlap <= 0.0f)
		return false;

	if (overlap < minOverlap)
	{
		minOverlap = overlap;
		bestAxis   = normAxis;
	}
	return true;
}

void PhysicSystem::BoxBox(ColliderData* boxData1, ColliderData* boxData2)
{
	XMFLOAT3 center1 = boxData1->pTransform->transform.GetWorldPosition();
	XMFLOAT3 center2 = boxData2->pTransform->transform.GetWorldPosition();

	XMFLOAT3 scale1 = boxData1->pTransform->transform.GetWorldScale() * boxData1->pCollider->scale;
	XMFLOAT3 scale2 = boxData2->pTransform->transform.GetWorldScale() * boxData2->pCollider->scale;

	XMFLOAT3 half1 = { scale1.x / 2.0f, scale1.y / 2.0f, scale1.z / 2.0f };
	XMFLOAT3 half2 = { scale2.x / 2.0f, scale2.y / 2.0f, scale2.z / 2.0f };

	XMMATRIX rot1 = boxData1->pTransform->transform.GetWorldRotMatrix();
	XMMATRIX rot2 = boxData2->pTransform->transform.GetWorldRotMatrix();

	XMFLOAT3 axes1[3], axes2[3];
	XMStoreFloat3(&axes1[0], rot1.r[0]);
	XMStoreFloat3(&axes1[1], rot1.r[1]);
	XMStoreFloat3(&axes1[2], rot1.r[2]);
	XMStoreFloat3(&axes2[0], rot2.r[0]);
	XMStoreFloat3(&axes2[1], rot2.r[1]);
	XMStoreFloat3(&axes2[2], rot2.r[2]);

	float minOverlap = FLT_MAX;
	XMFLOAT3 bestAxis = {0.0f, 0.0f, 0.0f};

	for (int i = 0; i < 3; i++)
	{
		if (!TestAxis(axes1[i], center1, axes1, half1, center2, axes2, half2, minOverlap, bestAxis)) return;
		if (!TestAxis(axes2[i], center1, axes1, half1, center2, axes2, half2, minOverlap, bestAxis)) return;
	}

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			XMFLOAT3 cross = Cross(axes1[i], axes2[j]);
			if (!TestAxis(cross, center1, axes1, half1, center2, axes2, half2, minOverlap, bestAxis)) return;
		}
	}

	AddCollision(boxData1, boxData2);

	XMFLOAT3 centerDiff = center1 - center2;
	if (Dot(centerDiff, bestAxis) < 0.0f)
		bestAxis = -bestAxis;

	XMFLOAT3 normalF = bestAxis;

	bool notTrigger = (boxData1->pCollider->isTrigger == false &&
	                   boxData2->pCollider->isTrigger == false);

	if (notTrigger == false)
		return;
	
	if (notTrigger)
	{
		float correction = minOverlap;

		bool s1 = boxData1->pCollider->isStatic;
		bool s2 = boxData2->pCollider->isStatic;

		if (!s1 && !s2)
		{
			XMFLOAT3 p1 = bestAxis * (correction / 2.0f) ;
			XMFLOAT3 p2 = -bestAxis * (correction / 2.0f);
			boxData1->pTransform->transform.MoveWorld(p1);
			boxData2->pTransform->transform.MoveWorld(p2);
		}
		else if (!s1)
		{
			XMFLOAT3 p1 = bestAxis * correction;
			boxData1->pTransform->transform.MoveWorld(p1);
		}
		else if (!s2)
		{
			XMFLOAT3 p2 = -bestAxis * correction;
			boxData2->pTransform->transform.MoveWorld(p2);
		}
	}

	ForceComponent* f1 = m_pOwnerScene->GetComponentType<ForceComponent>(boxData1->pCollider->GetOwner());
	ForceComponent* f2 = m_pOwnerScene->GetComponentType<ForceComponent>(boxData2->pCollider->GetOwner());

	if (normalF.y > 0.9f)
	{
		if (f2 != nullptr) f2->isGrounded = true;
	}
	else if (normalF.y < -0.9f)
	{
		if (f1 != nullptr) f1->isGrounded = true;
	}

	XMFLOAT3 rv = { 0.0f, 0.0f, 0.0f };
	if (f1 != nullptr && f2 != nullptr)
		XMStoreFloat3(&rv, XMLoadFloat3(&f1->velocity) - XMLoadFloat3(&f2->velocity));
	else if (f1 != nullptr)
		rv = f1->velocity;
	else if (f2 != nullptr)
		XMStoreFloat3(&rv, XMVectorScale(XMLoadFloat3(&f2->velocity), -2.0f));
	else
		return;

	float velAlongNormal = rv.x * normalF.x + rv.y * normalF.y + rv.z * normalF.z;

	bool b1CanBounce = boxData1->pCollider->canBounce;
	bool b2CanBounce = boxData2->pCollider->canBounce;

	if (velAlongNormal > -0.01f)
	{
		if (notTrigger)
		{
			if (!b1CanBounce) CancelNormalVelocity(f1,  bestAxis);
			if (!b2CanBounce) CancelNormalVelocity(f2, -bestAxis);
			ApplyRestThreshold(f1);
			ApplyRestThreshold(f2);
		}
		return;
	}

	if (!b1CanBounce && !b2CanBounce)
	{
		CancelNormalVelocity(f1,  bestAxis);
		CancelNormalVelocity(f2, -bestAxis);
		ApplyRestThreshold(f1);
		ApplyRestThreshold(f2);
		return;
	}

	float restitution = 0.8f;
	float cImpulse = -(1.0f + restitution) * velAlongNormal;

	XMFLOAT3 impulse = bestAxis * cImpulse;

	if (f1 != nullptr && b1CanBounce)
		XMStoreFloat3(&f1->velocity, XMLoadFloat3(&f1->velocity) - XMLoadFloat3(&impulse));
	if (f2 != nullptr && b2CanBounce)
		XMStoreFloat3(&f2->velocity, XMLoadFloat3(&f2->velocity) + XMLoadFloat3(&impulse));

	ApplyRestThreshold(f1);
	ApplyRestThreshold(f2);
}

bool PhysicSystem::IsCollisionBoxBox(ColliderData* boxData1, ColliderData* boxData2)
{
	XMFLOAT3 center1 = boxData1->pTransform->transform.GetWorldPosition();
	XMFLOAT3 center2 = boxData2->pTransform->transform.GetWorldPosition();

	XMFLOAT3 scale1 = boxData1->pTransform->transform.GetWorldScale() * boxData1->pCollider->scale;
	XMFLOAT3 scale2 = boxData2->pTransform->transform.GetWorldScale() * boxData2->pCollider->scale;

	XMFLOAT3 half1 = { scale1.x / 2.0f, scale1.y / 2.0f, scale1.z / 2.0f };
	XMFLOAT3 half2 = { scale2.x / 2.0f, scale2.y / 2.0f, scale2.z / 2.0f };

	XMMATRIX rot1 = boxData1->pTransform->transform.GetWorldRotMatrix();
	XMMATRIX rot2 = boxData2->pTransform->transform.GetWorldRotMatrix();

	XMFLOAT3 axes1[3], axes2[3];
	XMStoreFloat3(&axes1[0], rot1.r[0]);
	XMStoreFloat3(&axes1[1], rot1.r[1]);
	XMStoreFloat3(&axes1[2], rot1.r[2]);
	XMStoreFloat3(&axes2[0], rot2.r[0]);
	XMStoreFloat3(&axes2[1], rot2.r[1]);
	XMStoreFloat3(&axes2[2], rot2.r[2]);

	float minOverlap = FLT_MAX;
	XMFLOAT3 bestAxis = {0.0f, 0.0f, 0.0f};

	for (int i = 0; i < 3; i++)
	{
		if (!TestAxis(axes1[i], center1, axes1, half1, center2, axes2, half2, minOverlap, bestAxis)) return false;
		if (!TestAxis(axes2[i], center1, axes1, half1, center2, axes2, half2, minOverlap, bestAxis)) return false;
	}

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			XMFLOAT3 cross = Cross(axes1[i], axes2[j]);
			if (!TestAxis(cross, center1, axes1, half1, center2, axes2, half2, minOverlap, bestAxis)) return false;
		}
	}

	return true;
}

ColliderData* PhysicSystem::GetObjAtPos(XMFLOAT3 _pos)
{
	int posX = (int)(round(_pos.x / SIZE_CELL) * SIZE_CELL);
	int posY = (int)(round(_pos.y / SIZE_CELL) * SIZE_CELL);
	int posZ = (int)(round(_pos.z / SIZE_CELL) * SIZE_CELL);

	XMFLOAT3 pos = { (float)posX, (float)posY, (float)posZ };

	Cell* pCell = GetOrCreateCell({ pos });

	for (ColliderData* pCollider : pCell->vColliders)
	{
		if (pCollider->pCollider->isTrigger)
			continue;
		
		if (IsInside(pCollider, _pos))
		{
			return pCollider;
		}
	}

	return nullptr;
}

bool PhysicSystem::IsInside(ColliderData* _pCollider, XMFLOAT3 _pos)
{
	XMFLOAT3 posCollider = _pCollider->pTransform->transform.GetWorldPosition();

	if (_pCollider->pCollider->colliderType == ColliderType::SPHERE)
	{

		float dist = sqrt((posCollider.x - _pos.x) * (posCollider.x - _pos.x) + (posCollider.y - _pos.y) * (posCollider.y - _pos.y) + (posCollider.z - _pos.z) * (posCollider.z - _pos.z));

		if (dist <= _pCollider->pTransform->transform.GetWorldScale().x / 2.0f)
		{
			return true;
		}

		return false;
	}
	else if (_pCollider->pCollider->colliderType == ColliderType::BOX)
	{
		XMFLOAT3 scaleCollider = _pCollider->pTransform->transform.GetWorldScale() * _pCollider->pCollider->scale;

		float half[3];
		half[0] = scaleCollider.x / 2.0f;
		half[1] = scaleCollider.y / 2.0f;
		half[2] = scaleCollider.z / 2.0f;

		XMMATRIX rot = _pCollider->pTransform->transform.GetWorldRotMatrix();

		XMFLOAT3 diff = _pos - posCollider;

		XMVECTOR vDiff = XMLoadFloat3(&diff);
		for (int i = 0; i < 3; i++)
		{
			XMFLOAT3 axis;
			XMStoreFloat3(&axis, rot.r[i]);
			float proj = XMVectorGetX(XMVector3Dot(vDiff, XMLoadFloat3(&axis)));
			if (proj < -half[i] || proj > half[i])
				return false;
		}
	}

	return true;
}

void PhysicSystem::UpdateColliderInCell(ColliderData* _pCollider)
{
	XMFLOAT3 posCollider = _pCollider->pTransform->transform.GetWorldPosition();

	int posX = (int)(round(posCollider.x / SIZE_CELL) * SIZE_CELL);
	int posY = (int)(round(posCollider.y / SIZE_CELL) * SIZE_CELL);
	int posZ = (int)(round(posCollider.z / SIZE_CELL) * SIZE_CELL);

	XMFLOAT3 centerPos = { (float)posX, (float)posY, (float)posZ };

	for (Cell* cell : m_vGrid)
	{
		for (int i = (int)cell->vColliders.size() - 1; i >= 0; i--)
		{
			if (cell->vColliders[i] == _pCollider)
				cell->vColliders.erase(cell->vColliders.begin() + i);
		}
	}

	std::unordered_set<uint64_t> visited;
	std::stack<XMFLOAT3> toVisit;
	toVisit.push(centerPos);

	static const float dx[] = { SIZE_CELL, -SIZE_CELL, 0.f,       0.f,        0.f,        0.f        };
	static const float dy[] = { 0.f,        0.f,       SIZE_CELL, -SIZE_CELL, 0.f,        0.f        };
	static const float dz[] = { 0.f,        0.f,       0.f,        0.f,       SIZE_CELL, -SIZE_CELL  };

	while (!toVisit.empty())
	{
		XMFLOAT3 pos = toVisit.top();
		toVisit.pop();

		uint64_t key = CellKey((int)pos.x, (int)pos.y, (int)pos.z);
		if (!visited.insert(key).second) continue;

		Cell* pCell = GetOrCreateCell(pos);

		bool hit = false;
		if (_pCollider->pCollider->colliderType == ColliderType::SPHERE)
			hit = IsCollisionBoxSphere(pCell->pCollider, _pCollider);
		else if (_pCollider->pCollider->colliderType == ColliderType::BOX)
			hit = IsCollisionBoxBox(pCell->pCollider, _pCollider);

		if (!hit) continue;

		pCell->vColliders.push_back(_pCollider);

		for (int i = 0; i < 6; i++)
			toVisit.push({ pos.x + dx[i], pos.y + dy[i], pos.z + dz[i] });
	}
}

Cell* PhysicSystem::GetOrCreateCell(const XMFLOAT3& _pos)
{
	uint64_t key = CellKey((int)_pos.x, (int)_pos.y, (int)_pos.z);

	auto it = m_mapGrid.find(key);
	if (it != m_mapGrid.end())
		return it->second;

	Cell* newCell = new Cell;
	newCell->pos = _pos;

	ColliderData* cellCollider = new ColliderData();
	cellCollider->pCollider = new Collider();
	cellCollider->pCollider->colliderType = ColliderType::BOX;
	cellCollider->pTransform = new TransformComponent();
	cellCollider->pTransform->transform.SetWorldPosition(_pos);
	cellCollider->pTransform->transform.SetWorldScale(SIZE_CELL);

	newCell->pCollider = cellCollider;

	m_mapGrid[key] = newCell;
	m_vGrid.push_back(newCell);
	return newCell;
}

void PhysicSystem::AddCollision(ColliderData* colliderData1, ColliderData* colliderData2)
{
	Collision* c1 = m_pOwnerScene->GetComponentType<Collision>(colliderData1->pCollider->GetOwner());
	Collision* c2 = m_pOwnerScene->GetComponentType<Collision>(colliderData2->pCollider->GetOwner());

	if (c1 == nullptr)
		c1 = m_pOwnerScene->AddComponent<Collision>(colliderData1->pCollider->GetOwner());
	if (c2 == nullptr)
		c2 = m_pOwnerScene->AddComponent<Collision>(colliderData2->pCollider->GetOwner());

	CollisionType::CollisionType type;
	if (Contains(colliderData1->vOldCollisions, colliderData2->pCollider->GetOwner()))
		type = CollisionType::STAY;
	else
		type = CollisionType::ENTER;

	c1->mOthers[type].push_back(colliderData2->pCollider->GetOwner());
	c2->mOthers[type].push_back(colliderData1->pCollider->GetOwner());

	colliderData1->pCollider->vCurrentCollision.push_back(colliderData2->pCollider->GetOwner());
	colliderData2->pCollider->vCurrentCollision.push_back(colliderData1->pCollider->GetOwner());
}

#endif