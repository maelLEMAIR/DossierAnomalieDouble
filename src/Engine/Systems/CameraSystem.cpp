#ifndef CAMERA_SYSTEM_CPP_INCLUDED
#define CAMERA_SYSTEM_CPP_INCLUDED

#include "CameraSystem.h"
#include "Components/CameraComponent.h"
#include "Components/TransformComponent.h"

void CameraSystem::OnInit()
{
    SetMaskLoadComponents<CameraComponent, TransformComponent>();
}

void CameraSystem::Update(float dt)
{
    for (auto [id, vComponents] : m_mComponents)
    {
        if (vComponents[0]->GetOwner()->isActive == false) continue;

        if (vComponents[0]->isActive == false || vComponents[1]->isActive == false) continue;

        CameraComponent* pCameraComponent = reinterpret_cast<CameraComponent*>(vComponents[1]);
        TransformComponent* transform = reinterpret_cast<TransformComponent*>(vComponents[0]);
        
        Camera* pCam = &pCameraComponent->camera;
        pCam->SetWorld(transform->transform.GetWorldMatrix());
    }
}

#endif
