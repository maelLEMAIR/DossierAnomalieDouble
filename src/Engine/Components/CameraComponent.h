#ifndef CAMERA_COMPONENT_H_INCLUDED
#define CAMERA_COMPONENT_H_INCLUDED

#include "Engine/Component.h"
#include "../Render/Generic/Base/Camera.h"

class CameraComponent : public Component
{
public:
    Camera camera;
};

#endif