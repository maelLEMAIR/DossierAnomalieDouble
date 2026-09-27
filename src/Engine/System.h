#ifndef SYSTEM_H_DEFINED
#define SYSTEM_H_DEFINED 

#include "define.h"

class Entity;
class Component;
class Scene;
class EngineManager;

class System
{
public:
	int priority = 0;
protected:
	template <typename ...ComponentType>
	void SetMaskLoadComponents();

	template <typename ComponentType>
	void SetAvoidMaskComponent();

	template <typename ...ComponentType>
	void SetMaskOrComponents();

	Scene* m_pOwnerScene = nullptr;

	UnorderedMap< int, Vector<Component*>> m_mComponents;

	uint64_t m_mask = 0;
	uint64_t m_avoidMask = 0;

	virtual void OnInit() {};

private:
	virtual void Update(float dt) {};

	void UpdateMap(uint64_t maskUpdate);
	void UpdateMapEntity(uint64_t maskUpdate, Entity* pEntity);

	virtual void OnUpdateMapEntity(uint64_t _maskUpdate, Entity* _pEntity, bool _isNew = true) {};

	void Init(Scene* pOwner);

	bool m_isOr = false;
	
	friend class Scene;
	friend class SceneManager;
};

#include "System.inl"

#endif