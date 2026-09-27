#ifndef ANOMALIES_MANAGER_H_INCLUDED
#define ANOMALIES_MANAGER_H_INCLUDED

#include "AnomalyBase.h"
#include "pch.h"

class Room;
class AnomalyBase;

class AnomaliesManager
{
public:
    void Update(float _dt);
    void ChooseAnAnomaly(Room* _pRoom);
    void DisableAnomaly();
private:

    AnomalyBase* m_pCurrentAnomaly = nullptr;
};

#endif
