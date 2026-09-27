#ifndef UI_TEXT_INPUT_COMPONENT_H_DEFINED
#define UI_TEXT_INPUT_COMPONENT_H_DEFINED

#include "Component.h"
#include "../../Render/Generic/Render.h"
#include "../RessourceManager.h"

namespace UITextInputType
{
    enum Type
    {
        HOVERED,
        STATE
};
}

class UITextInputComponent : public Component
{
public:
    String currentText = "";

    bool isHovered = false;
    bool isFocused = false;

    UITextInputType::Type type = UITextInputType::HOVERED;
};

#endif