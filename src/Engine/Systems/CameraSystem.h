#ifndef CAMERA_SYSTEM_H_INCLUDED
#define CAMERA_SYSTEM_H_INCLUDED

#include "System.h"

class CameraComponent;
class TransformComponent;

class CameraSystem : public System
{
public:
    void OnInit() override;
private:
    void Update(float _dt) override;
};

#endif