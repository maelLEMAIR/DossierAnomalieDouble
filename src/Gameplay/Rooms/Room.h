#ifndef ROOM_H_INCLUDED
#define ROOM_H_INCLUDED

#include "pch.h"

class MainScene;
class RevealRoom;

class Room
{
public:
    virtual void OnInit(Device* _pDevice, Scene& _scene);
    virtual void OnUpdate();
    virtual void ActiveRoom(XMFLOAT3 newCenter);
    virtual void DisabledRoom(float _disableEntity = true);
    
    void AddTween(Tween* _tween)                                { m_vTweens.push_back(_tween);              }
    void AddEntity(Entity* _entity)                             { m_vEntities.push_back(_entity);           }
    void AddLightPos(XMFLOAT3& _pos)                            { m_vLightsPos.push_back(_pos);             }
    void AddAnomaly(AnomalyType _type, AnomalyBase* _entity)    { m_mAnomalies[_type].push_back(_entity);   }

    Vector<LightComponent*> GetLight() { return m_vLightsUse; }
    Vector<Entity*> GetEntity() { return m_vEntities; }
    UnorderedMap<AnomalyType, Vector<AnomalyBase*>> GetAnomalies() { return m_mAnomalies; }
    
    XMFLOAT3 centerPos = {0.0f, 0.0f, 0.0f};
    XMFLOAT3 size = {0.0f, 0.0f, 0.0f};
    float startOffsetZ = 0.0f;

    RevealRoom* pRevealRoom = nullptr;
    LeverStateMachine* m_pLeverStateMachine = nullptr;
    
    std::string name;
protected:
    Scene* m_pScene = nullptr;
    Vector<XMFLOAT3> m_vLightsPos;
    Vector<LightComponent*> m_vLightsUse;
    Vector<Entity*> m_vEntities;
    UnorderedMap<AnomalyType, Vector<AnomalyBase*>> m_mAnomalies;
    std::vector<Tween*> m_vTweens;
};

#endif