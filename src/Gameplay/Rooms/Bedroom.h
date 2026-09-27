#ifndef BEDROOM_H_INCLUDED
#define BEDROOM_H_INCLUDED

#include "Room.h"

class MapLoader;

class Bedroom : public Room
{
public:
    void OnInit(Device* _pDevice, Scene& _scene) override;
    void OnUpdate() override { };
    void ActiveRoom(XMFLOAT3 newCenter) override;
    void DisabledRoom(float _disableEntity = true) override;
};

#endif