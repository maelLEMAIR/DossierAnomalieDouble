#ifndef SCENE_MANAGER_H_DEFINED
#define SCENE_MANAGER_H_DEFINED

#include <unordered_map>
#include <string>

class TransformComponent;
class AudioEngine;
class Scene;

class SceneManager
{
public:
	SceneManager();
	static SceneManager& GetInstance() { return *s_pSceneManager; }

	static Scene* GetCurrentScene() { return s_pSceneManager->m_pCurrentScene; }

	static Scene* GetSceneWithName(std::string _name);

	static Scene* CreateScene(std::string _name);

	template <typename SceneType>
	static SceneType* CreateSceneType(std::string _name);

	static void ChangeCurrentScene(Scene* _pScene);
	static void ChangeCurrentScene(std::string _name);

private:
	inline static SceneManager* s_pSceneManager = nullptr;

	std::unordered_map<std::string, Scene*> m_mScenes;
	Scene* m_pCurrentScene = nullptr;
	float m_accumulator = 0.0f;
	
	void Update(float _dt, AudioEngine* _pAudioEngine);

	friend class EngineManager;
};

#include "SceneManager.inl"

#endif

