#include "SceneManager.h"

#include "Scene.h"
#include "System.h"
#include "AudioEngine/AudioEngine.h"
#include "Systems/ForceSystem.h"
#include "Systems/PhysicSystem.h"
#include "Systems/TransformSystem.h"
#include "Systems/CollisionSystem.h"
#include "Components/TransformComponent.h"
#include "Components/CameraComponent.h"


SceneManager::SceneManager()
{
	s_pSceneManager = this;

	Scene* pDefaultScene = CreateScene("Default");
	m_pCurrentScene = pDefaultScene;
}

Scene* SceneManager::GetSceneWithName(std::string _name)
{
	for (auto [name, pScene] : s_pSceneManager->m_mScenes)
	{
		if (name == _name)
			return pScene;
	}

	return nullptr;
}

Scene* SceneManager::CreateScene(std::string _name)
{
	Scene* pNewScene = new Scene;
	pNewScene->Init(_name);

	s_pSceneManager->m_mScenes[_name] = pNewScene;

	return pNewScene;
}

void SceneManager::ChangeCurrentScene(Scene* _pScene)
{
	if (_pScene == nullptr) return;

	if (!s_pSceneManager->m_mScenes.contains(_pScene->GetName()))
		s_pSceneManager->CreateScene(_pScene->GetName());

	s_pSceneManager->m_pCurrentScene = _pScene;
	s_pSceneManager->m_pCurrentScene->Start();
}

void SceneManager::ChangeCurrentScene(std::string _name)
{
	ChangeCurrentScene(s_pSceneManager->GetSceneWithName(_name));
}

void SceneManager::Update(float _dt, AudioEngine* _pAudioEngine)
{
	m_pCurrentScene->CreateComponents();

	const float FIXED_DT = 1.0f / 60.0f;
	m_accumulator += _dt;
	m_pCurrentScene->pForceSystem->ResetGrounded();
	
	while (m_accumulator >= FIXED_DT)
	{
		m_pCurrentScene->pTransformSystem->SavePreviousPositions();
        
		m_pCurrentScene->pForceSystem->Update(FIXED_DT);
		m_pCurrentScene->pTransformSystem->Update(FIXED_DT);
		m_pCurrentScene->pPhysicSystem->Update(FIXED_DT);
		m_pCurrentScene->pCollisionSystem->Update(FIXED_DT);
		m_pCurrentScene->pTransformSystem->Update(FIXED_DT);
        
		m_accumulator -= FIXED_DT;
	}

	m_pCurrentScene->interpolationAlpha = m_accumulator / FIXED_DT;

	if (m_pCurrentScene->pCamera != nullptr)
	{
		TransformComponent* pCamTransform = m_pCurrentScene->GetComponentType<TransformComponent>(
			m_pCurrentScene->pCamera->GetOwner());

		if (pCamTransform != nullptr)
			_pAudioEngine->SetPosListener(
				pCamTransform->transform.GetWorldPosition(),
				pCamTransform->transform.GetForward(),
				pCamTransform->transform.GetUp());
	}
	
	for (System* pSystem : m_pCurrentScene->m_vSystems)
	{
		if (pSystem == m_pCurrentScene->pForceSystem)		continue;
		if (pSystem == m_pCurrentScene->pPhysicSystem)		continue;
		if (pSystem == m_pCurrentScene->pCollisionSystem)	continue;
		if (pSystem == m_pCurrentScene->pTransformSystem)	continue;
		pSystem->Update(_dt);
	}

	m_pCurrentScene->OnUpdate(_dt);
	m_pCurrentScene->DestroyComponents();
	m_pCurrentScene->DeleteEntities();
}