#ifndef UI_BUTTON_COMPONENT_H_DEFINED
#define UI_BUTTON_COMPONENT_H_DEFINED 

#include "Component.h"
#include "../../Render/Generic/Render.h"
#include "../RessourceManager.h"
#include <functional>

class UIButtonComponent : public Component
{
public:
    Texture* textureNormal = nullptr;
    Texture* textureHover = nullptr;
    Texture* currentTexture = nullptr;

    bool isHovered = false;
    bool isPressed = false;

    //std::function<void()> onClick;
};

#endif