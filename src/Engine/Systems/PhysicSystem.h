#ifndef PHYSIC_SYSTEM_H_DEFINED
#define PHYSIC_SYSTEM_H_DEFINED

#include "System.h"
#include "../Components/ColliderData.h"

#include <set>
#include <tuple>

class Collider;
class Transform;
class Entity;
struct ColliderData;
class RayCast;

#define SIZE_CELL 16.0f

struct Cell
{
public:

	XMFLOAT3 pos = { 0.0f, 0.0f, 0.0f };
	XMFLOAT3 size = { SIZE_CELL, SIZE_CELL, SIZE_CELL };

	ColliderData* pCollider = nullptr;

	std::vector<ColliderData*> vColliders;
};

class PhysicSystem : public System
{
public:
	void OnInit() override;
	void Update(float dt) override;

	void CheckRayCast(RayCast _rc, HitPoint& _hp);
private:
	void OnUpdateMapEntity(uint64_t _maskUpdate, Entity* _pEntity, bool _isNew = true) override;

	void SphereSphere(ColliderData* colliderData1, ColliderData* colliderData2);
	void BoxSphere(ColliderData* boxData, ColliderData* sphereData);
	void BoxBox(ColliderData* boxData1, ColliderData* boxData2);

	bool IsCollisionSphereSphere(ColliderData* colliderData1, ColliderData* colliderData2);
	bool IsCollisionBoxSphere(ColliderData* boxData, ColliderData* sphereData);
	bool IsCollisionBoxBox(ColliderData* boxData1, ColliderData* boxData2);

	ColliderData* GetObjAtPos(XMFLOAT3 _pos);
	bool IsInside(ColliderData* _pCollider, XMFLOAT3 _pos);

	void UpdateColliderInCell(ColliderData* _pCollider);
	Cell* GetOrCreateCell(const XMFLOAT3& _pos);

	std::unordered_map<uint64_t, Cell*> m_mapGrid;
	void AddCollision(ColliderData* collider1, ColliderData* collider2);

	std::vector<ColliderData*> m_vColliders;
	
	std::vector<Cell*> m_vGrid;
};

#endif