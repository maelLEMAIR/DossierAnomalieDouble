#ifndef ANOMALIES_MANAGER_CPP_INCLUDED
#define ANOMALIES_MANAGER_CPP_INCLUDED

#include "Rooms/Room.h"
#include "AnomaliesManager.h"
#include "AnomalyBase.h"

void AnomaliesManager::Update(float _dt)
{
    if  (m_pCurrentAnomaly == nullptr)
        return;

    m_pCurrentAnomaly->OnUpdate(_dt);
}

void AnomaliesManager::ChooseAnAnomaly(Room* _pRoom)
{    
    UnorderedMap<AnomalyType, Vector<AnomalyBase*>> vAnomalies = _pRoom->GetAnomalies();

    if ( vAnomalies.empty() ) return;
    
    std::uniform_int_distribution distTypeAnomaly(0, (int)vAnomalies.size() - 1);
    int typeAnomalyRand = distTypeAnomaly(EngineManager::GetRand());
    
    Vector<AnomalyType> keys;
    keys.reserve(vAnomalies.size());
    for (auto& pair : vAnomalies)
        keys.push_back(pair.first);

    AnomalyType selectedType = keys[typeAnomalyRand];

    std::uniform_int_distribution distAnomaly(0, (int)vAnomalies[selectedType].size() - 1);
    int anomalyIdRand = distAnomaly(EngineManager::GetRand());
    
    m_pCurrentAnomaly = vAnomalies[selectedType][anomalyIdRand];
}

void AnomaliesManager::DisableAnomaly()
{
    if ( m_pCurrentAnomaly )
    {
        m_pCurrentAnomaly->OnEnd();
        m_pCurrentAnomaly = nullptr;
    }
}

#endif
