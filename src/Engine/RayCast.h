#ifndef RAY_CAST_H_DEFINED
#define RAY_CAST_H_DEFINED

#include "define.h"
#include <unordered_set>

class Entity;

class HitPoint
{
public:
	Entity* pHitEntity = nullptr;
	XMFLOAT3 hitPoint;
};

class RayCast
{
public:
	XMFLOAT3 origin;
	XMFLOAT3 dir;
	float maxDist;
	float pas = 0.1f;
	std::unordered_set<int> avoidTag;
};

#endif