#include "UITextInputSystem.h"
#include "Engine/RessourceManager.h"
#include "InputSystem.h"
#include "components.h"
#include <iostream>

void UITextInputSystem::OnInit()
{
    SetMaskLoadComponents<UITextInputComponent, TransformComponent, SpriteComponent, TextComponent>();
}

void UITextInputSystem::Update(float dt)
{
    XMINT2 mousePos = InputSystem::GetMousePositionCenter();

    for (auto& [id, data] : m_vInputs)
    {
        if (data.pTextInput->GetOwner()->isActive == false) continue;
        if (data.pTextInput->isActive == false || data.pTransform->isActive == false || data.pText->isActive == false) continue;
        if (data.pSM != nullptr)
            if (data.pSM->isActive == false) continue;

        if (data.pTextInput->type == UITextInputType::HOVERED)
        {
            bool isInside = MouseIsInside(data, mousePos);

            if (isInside)
            {
                if (data.pTextInput->isHovered == false)
                {
                    data.pSM->GetStateGlobal()->OnHoveredEnter();
                    data.pTextInput->isHovered = true;
                }

                if (InputSystem::IsMouseButtonDown(InputMouse::LEFT_MOUSE))
                {
                    if (data.pTextInput->isFocused == false)
                    {
                        data.pTextInput->isFocused = true;
                        data.pSM->GetStateGlobal()->OnButtonDown();

                        InputSystem::ClearTypedChars();
                    }
                }
            }
            else
            {
                if (data.pTextInput->isHovered)
                {
                    data.pSM->GetStateGlobal()->OnHoveredExit();
                    data.pTextInput->isHovered = false;
                }

                if (InputSystem::IsMouseButtonDown(InputMouse::LEFT_MOUSE))
                {
                    if (data.pTextInput->isFocused)
                    {
                        data.pTextInput->isFocused = false;
                        data.pSM->GetStateGlobal()->OnButtonUp();
                    }
                }
            }
        }
         
        if (data.pTextInput->isFocused)
        {
            data.pTextInput->currentText = InputSystem::GetTypedChar();
             
            if (data.pTextInput->currentText == "")
            {
                data.pText->SetText("");
            }
            else
            {
                data.pText->SetText(data.pTextInput->currentText);
            }
             
            if (InputSystem::IsKeyPressed(InputKeyboard::RETURN) || InputSystem::IsKeyPressed(InputKeyboard::NUMPAD_RETURN))
            {
                data.pTextInput->isFocused = false;
                if (data.pSM != nullptr)
                    data.pSM->GetStateGlobal()->OnButtonPressed();  
            }
        }
        else
        { 
            if (data.pTextInput->currentText == "")
            {
                data.pText->SetText("Cliquez ici pour ecrire...");
            }
            else
            {
                data.pText->SetText(data.pTextInput->currentText);
            }
        }
    }
}

void UITextInputSystem::OnUpdateMapEntity(uint64_t _maskUpdate, Entity* _pEntity, bool _isNew)
{
    if (_isNew)
    {
        if (_maskUpdate == COMPONENT_MASK(UITextInputComponent))
        {
            DataTextInput newInput;

            newInput.pTextInput = m_pOwnerScene->GetComponentType<UITextInputComponent>(_pEntity);
            newInput.pTransform = m_pOwnerScene->GetComponentType<TransformComponent>(_pEntity);
            newInput.pSM = m_pOwnerScene->GetComponentType<StateMachineComponent>(_pEntity);
            newInput.pSprite = m_pOwnerScene->GetComponentType<SpriteComponent>(_pEntity);
            newInput.pText = m_pOwnerScene->GetComponentType<TextComponent>(_pEntity);

            m_vInputs[_pEntity->id] = newInput;
        }
    }
    else
    {
        if (m_vInputs.contains(_pEntity->id))
        {
            m_vInputs.erase(_pEntity->id);
        }
    }
}

bool UITextInputSystem::MouseIsInside(DataTextInput& _data, XMINT2 _mousePos)
{
    XMINT2 buttonSize;
    buttonSize.x = _data.pSprite->width;
    buttonSize.y = _data.pSprite->height;

    XMINT2 buttonPos;
    buttonPos.x = (int)(_data.pTransform->transform.GetWorldPosition().x - (float)buttonSize.x * 0.5f);
    buttonPos.y = (int)(_data.pTransform->transform.GetWorldPosition().y - (float)buttonSize.y * 0.5f);

    if (_mousePos.x >= buttonPos.x && _mousePos.y >= buttonPos.y)
    {
        if (_mousePos.x <= buttonPos.x + buttonSize.x && _mousePos.y <= buttonPos.y + buttonSize.y)
        {
            return true;
        }
    }
    return false;
}