#ifndef SCENE_MANAGER_INL_DEFINED
#define SCENE_MANAGER_INL_DEFINED 

#include "SceneManager.h"

template <typename SceneType>
inline SceneType* SceneManager::CreateSceneType(std::string _name)
{
	SceneType* pNewScene = new SceneType;

	Scene* pScene = pNewScene;

	s_pSceneManager->m_mScenes[_name] = pScene;

	pScene->Init(_name);

	return pNewScene;
}

#endif