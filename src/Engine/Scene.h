#ifndef SCENE_H_DEFINED
#define SCENE_H_DEFINED

#include "define.h"

class Entity;
class Component;
class System;
class CameraComponent;
class LightComponent;
class LightSystem;
class TransformSystem;
class PhysicSystem;
class CollisionSystem;
class ForceSystem;


class Scene
{
public:
	CameraComponent* pCamera = nullptr;

	std::string GetName() { return m_name; }

	bool IsInit() { return m_isInit; }
	//ECS
	void UpdateEntityInArchetype(Entity* _pEntity, uint64_t _mask, bool _isPlus = true);

	void UpdateComponentInSystem(uint64_t _maskUpdate);

	Entity* CreateEntity(bool _hasTransfrom = true);
	void DestroyEntity(Entity* pEntity);

	//Component
	template <typename T>
	T* AddComponent(Entity* _pEntity);

	template <typename T>
	bool HasComponent(Entity* _pEntity);

	template <typename T>
	Component* GetComponent(Entity* _pEntity);

	Vector<Component*> GetAllComponentEntityWithMask(Entity* _pEntity, uint64_t _mask);

	template <typename T>
	T* GetComponentType(Entity* _pEntity);

	template <typename T>
	void RemoveComponent(Entity* _pEntity);

	template <typename... ComponentType>
	UnorderedMap<int, Vector<Component*>> GetAllComponentsEntities();

	UnorderedMap<int, Vector<Component*>> GetAllComponentsEntitiesWithMask(uint64_t _mask);

	UnorderedMap<int, Vector<Component*>> GetAllComponentsEntitiesWithMaskAndWithoutMask(uint64_t _withMask, uint64_t _withoutMask);

	UnorderedMap<int, Vector<Component*>> GetEntitiesWithComponentOfMask(uint64_t _mask);

	template <typename ComponentType>
	Vector<Component*> GetAllComponentOfType();

	//System
	template <typename SystemType>
	SystemType* RegisterSystem(int _priority = 0);

	PhysicSystem* GetPhysicSystem() { return pPhysicSystem; }
	ForceSystem* GetForceSystem() { return pForceSystem; }
	
	LightComponent* GetFirstAvailableLight();
	void DisableLight(LightComponent* _pLight);

protected:
	TransformSystem* pTransformSystem = nullptr;
	ForceSystem* pForceSystem = nullptr;
	PhysicSystem* pPhysicSystem = nullptr;
	LightSystem* pLightSystem = nullptr;
	CollisionSystem* pCollisionSystem = nullptr;

	float interpolationAlpha = 1.0f;
	virtual void OnInit() {};
	virtual void OnStart() {};
	virtual void OnUpdate(float _dt) {};
	
	bool m_isInit = false;
	
private:
	String m_name;
	
	Vector<LightComponent*> m_vLights;
	
	UnorderedMap<uint64_t, Vector<Entity*>> m_mArchetype;
	Vector<Entity*> m_vDestroyEntities;

	UnorderedMap<uint64_t, Vector<Component*>> m_mComponents;
	Vector<Component*> m_vCreationComponents;
	Vector<Component*> m_vDestroyComponents;

	Vector<System*> m_vSystems;

	int m_entityCount = 0;
	
	int m_systemCount = 0;

	void Init(std::string _name);
	void Start();
	void Destroy();

	void CreateComponents();
	void DestroyComponents();
	void DeleteEntities();

	friend class ECS;
	friend class SceneManager; 
};

#include "Scene.inl"

#endif