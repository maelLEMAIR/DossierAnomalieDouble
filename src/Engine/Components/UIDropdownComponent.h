#ifndef UI_DROPDOWN_COMPONENT_H_DEFINED
#define UI_DROPDOWN_COMPONENT_H_DEFINED 

#include "Component.h"
#include <vector>
#include <string>

class Entity;  

class UIDropdownComponent : public Component
{
public:
    std::vector<std::string> options;
    int selectedIndex = 0;

    bool isOpen = false;
    bool isHovered = false;

    int optionHeight = 30;
     
    Entity* pMainTextEntity = nullptr;
    std::vector<Entity*> optionBackgroundEntities;
    std::vector<Entity*> optionTextEntities;

    float textOffsetX = 40.0f;
    float textOffsetY = 25.0f;
    float gapTop = 5.0f;
    float gapOptions = 2.0f;
};

#endif