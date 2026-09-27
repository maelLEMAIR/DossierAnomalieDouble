#ifndef PARENT_COMPONENT_H_DEFINED
#define PARENT_COMPONENT_H_DEFINED 

#include "Component.h"

class ParentComponent : public Component
{
public:
	std::vector<Entity*> vChildrens;
};

#endif