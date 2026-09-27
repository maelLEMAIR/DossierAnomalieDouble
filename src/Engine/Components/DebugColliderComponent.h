#ifndef DEBUG_COLLIDER_COMPONENT_H_INCLUDED
#define DEBUG_COLLIDER_COMPONENT_H_INCLUDED

#include "Component.h"

class Entity;
struct ColliderData;
class Collider;

class DebugColliderComponent : public Component
{
public:
    Geometry* pGeo;
    ColliderData* pColliderData;
};

#endif