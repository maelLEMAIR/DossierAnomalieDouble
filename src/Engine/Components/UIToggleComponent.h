#ifndef UI_TOGGLE_COMPONENT_H_DEFINED
#define UI_TOGGLE_COMPONENT_H_DEFINED 

#include "Component.h"
#include "../../Render/Generic/Render.h"

class UIToggleComponent : public Component
{
public:
    Texture* textureNormal = nullptr;
    Texture* textureHover = nullptr;
    Texture* textureToggled = nullptr;  
    Texture* currentTexture = nullptr;

    bool isHovered = false;
    bool isToggled = false;
};

#endif