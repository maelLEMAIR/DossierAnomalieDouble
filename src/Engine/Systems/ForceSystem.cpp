#include "ForceSystem.h"
#include "../components.h"
#include <iostream>

struct ForceData
{
    Transform* transform = nullptr;
    ForceComponent* force = nullptr;
};

void ForceSystem::OnInit()
{
    SetMaskLoadComponents<TransformComponent, ForceComponent>();
}

void ForceSystem::AddForce(ForceComponent* _forceComponent, XMFLOAT3 const& _force)
{
    _forceComponent->appliedForce.push_back(_force);
}

void ForceSystem::Update(float dt)
{
    Vector<ForceData> vForceDatas;

    for (auto [id, vComponents] : m_mComponents)
    {
        if (vComponents.size() < 2) continue;

        if (vComponents[0]->GetOwner()->isActive == false) continue;
        if (vComponents[0]->isActive == false || vComponents[1]->isActive == false) continue;

        ForceData newForceData;

        int64_t mask = COMPONENT_MASK(ForceComponent);

        if (vComponents[0]->mask == mask)
        {
            newForceData.force     = reinterpret_cast<ForceComponent*>(vComponents[0]);
            newForceData.transform = &reinterpret_cast<TransformComponent*>(vComponents[1])->transform;
        }
        else
        {
            newForceData.force     = reinterpret_cast<ForceComponent*>(vComponents[1]);
            newForceData.transform = &reinterpret_cast<TransformComponent*>(vComponents[0])->transform;
        }

        vForceDatas.push_back(newForceData);
    }

    for (ForceData& forceData : vForceDatas)
    {
        bool wasGrounded = forceData.force->wasGroundedLastFrame;

        if (forceData.force->useGravity && dt < 0.1f)
        {
            forceData.force->velocity.y += m_Gravity * dt;

            if (wasGrounded && forceData.force->velocity.y < 0.0f)
                forceData.force->velocity.y = 0.0f;
        }

        for (XMFLOAT3 const& force : forceData.force->appliedForce)
        {
            XMVECTOR velVector      = XMLoadFloat3(&forceData.force->velocity);
            XMVECTOR velFinalVector = XMVectorAdd(velVector, XMLoadFloat3(&force));
            XMStoreFloat3(&forceData.force->velocity, velFinalVector);
        }
        forceData.force->appliedForce.clear();

        XMVECTOR move = XMVectorScale(XMLoadFloat3(&forceData.force->velocity), dt);
        XMFLOAT3 fMove;
        XMStoreFloat3(&fMove, move);

        forceData.transform->MoveWorld(fMove);

        forceData.force->velocity.x *= powf(forceData.force->drag, dt);
        forceData.force->velocity.z *= powf(forceData.force->drag, dt);
    }
}

void ForceSystem::ResetGrounded()
{
    for (auto& [id, vComponents] : m_mComponents)
    {
        int64_t mask = COMPONENT_MASK(ForceComponent);
        for (Component* c : vComponents)
        {
            if (c->mask == mask)
            {
                ForceComponent* f = reinterpret_cast<ForceComponent*>(c);
                f->wasGroundedLastFrame = f->isGrounded;
                f->isGrounded = false;
                break;
            }
        }
    }
}
