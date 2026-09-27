#ifndef ROOM_CPP_INCLUDED
#define ROOM_CPP_INCLUDED

#include "Room.h"
#include "RevealRoom.h"
#include "MainScene.h"

void Room::OnInit(Device* _pDevice, Scene& _scene)
{
    m_pScene = &_scene;
    DisabledRoom();
}

void Room::OnUpdate()
{
    
}

void Room::ActiveRoom(XMFLOAT3 newCenter)
{
    XMFLOAT3 delta = {
        newCenter.x - centerPos.x,
        newCenter.y - centerPos.y,
        newCenter.z - centerPos.z
    };

    for (Entity* entity : m_vEntities)
    {
        entity->isActive = true;
        
        TransformComponent* pTransform = m_pScene->GetComponentType<TransformComponent>(entity);
        if (!pTransform) continue;

        XMFLOAT3 pos = pTransform->transform.GetWorldPosition();
        pTransform->transform.SetWorldPosition({
            pos.x + delta.x,
            pos.y + delta.y,
            pos.z + delta.z
        });
    }

    XMFLOAT3 color = ToColor(252, 214, 109);
    MainScene* pScene = static_cast<MainScene*>(m_pScene);
    for (XMFLOAT3& pos : m_vLightsPos)
    {
        LightComponent* pLight = pScene->GetFirstAvailableLight();
        if ( pLight == nullptr)
            break;
        
        pos += delta;
        pLight->SetPosition(pos);
        pLight->SetColor({color.x, color.y, color.z, 1.0f});
        m_vLightsUse.push_back(pLight);
    }
    
    for (Tween* tween : m_vTweens)
    {
        tween->SetStart(tween->GetStart() + delta);
        tween->SetEnd(tween->GetEnd() + delta);
    }
    
    centerPos = newCenter;

}

void Room::DisabledRoom(float _disableEntity)
{
    std::cout << "Room disable" << std::endl;

    for (LightComponent* pLight : m_vLightsUse)
    {
        MainScene* pScene = static_cast<MainScene*>(m_pScene);
        pScene->DisableLight(pLight);
    }

    m_vLightsUse.clear();
    
    if (_disableEntity)
    {
        for (Entity* entity : m_vEntities)
            entity->isActive = false;
    }

    if ( m_pLeverStateMachine )
        m_pLeverStateMachine->Reset();
}

#endif
