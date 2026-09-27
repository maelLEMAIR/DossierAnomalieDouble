#ifndef LIFE_TIME_SYSTEM_H_INCLUDED
#define LIFE_TIME_SYSTEM_H_INCLUDED

#include "pch.h"

class LifeTimeComponent;

class LifeTimeSystem : public System
{
public:
    void OnInit() override;
private:
    void Update(float dt) override;
};

#endif