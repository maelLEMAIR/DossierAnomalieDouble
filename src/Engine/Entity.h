#ifndef ENTITY_H_DEFINED
#define ENTITY_H_DEFINED 

#include <vector>
#include "Component.h"
#include "define.h"

class Scene;

class Entity
{
public:
	uint32_t id = -1;
	uint64_t mask = 0;
	String name = "Entity";
	Scene* pScene = nullptr;
	bool isActive = true;
	bool isDestroy = false;
	int tag = -1; //-1 must be the default tag
};

#endif

