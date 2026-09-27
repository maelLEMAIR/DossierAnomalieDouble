#ifndef UI_SLIDER_COMPONENT_H_DEFINED
#define UI_SLIDER_COMPONENT_H_DEFINED 

#include "Component.h"
#include "../Entity.h"

class UISliderComponent : public Component
{
public:
    float minValue = 0.0f;
    float maxValue = 100.0f;
    float currentValue = 50.0f;

    bool isHovered = false;
    bool isDragging = false;

    Entity* pKnobEntity = nullptr; 
};

#endif