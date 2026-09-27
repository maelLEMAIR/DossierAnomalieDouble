#ifndef PING_TRIANGLE_CPP_DEFINED
#define PING_TRIANGLE_CPP_DEFINED
#include "PingTriangle.h"

#include "Scene.h"
#include "components.h"
#include "Tween.h"
#include "Components/LifeTimeComponent.h"

void PingTriangle::Init(Scene* _pScene, XMFLOAT3 _pos)
{
	Entity* pEntity = _pScene->CreateEntity();
	MeshRenderer* pMesh = _pScene->AddComponent<MeshRenderer>(pEntity);
	pMesh->pGeometry = RessourceManager::GetGeometry("Pyramide");
	pMesh->pMaterial = RessourceManager::GetMaterial("Blue");
	TransformComponent* pTr = _pScene->GetComponentType<TransformComponent>(pEntity);
	pTr->transform.AddLocalYPR({0.0f, -PI, 0.0f});
	pTr->transform.SetWorldPosition(_pos + XMFLOAT3(0.0f, 0.4f, 0.0f));
	pTr->transform.SetWorldScale(0.2f);

	Tween* tween = TweenSystem::Create(
		pTr->transform.GetWorldPosition(),
		pTr->transform.GetWorldPosition() - XMFLOAT3(0.0f, 0.3f, 0.0f),
		Interpolation::easingInAndOut_linear
	);
	tween->StartLoop(0.5f, Function::Position, &pTr->transform);

	LifeTimeComponent* plt = _pScene->AddComponent<LifeTimeComponent>(pEntity);
	plt->m_lifeCooldown = 10.0f;
}

#endif