#ifndef MESH_RENDERER_H_INLUDED
#define MESH_RENDERER_H_INLUDED

#include "Engine/Component.h"

class Scene;

class MeshRenderer : public Component
{
public:
    bool isInit = false;
    Geometry* pGeometry = nullptr;
    Material* pMaterial = nullptr;
};

#endif
