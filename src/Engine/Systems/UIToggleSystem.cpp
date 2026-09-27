#include "UIToggleSystem.h"  
#include "components.h"
#include "Engine/RessourceManager.h"
#include "InputSystem.h"

void UIToggleSystem::OnInit()
{
    SetMaskLoadComponents<UIToggleComponent, TransformComponent, StateMachineComponent, SpriteComponent>();
}

void UIToggleSystem::Update(float dt)
{
    XMINT2 mousePos = InputSystem::GetMousePositionCenter();

    for (auto& [id, data] : m_vToggles)
    {
        if (data.pToggle->GetOwner()->isActive == false) continue;
        if (data.pToggle->isActive == false || data.pTransform->isActive == false || data.pSM->isActive == false || data.pSprite->isActive == false) continue;

        if (MouseIsInside(data, mousePos))
        {
            if (data.pToggle->isHovered == false)
            {
                data.pSM->GetStateGlobal()->OnHoveredEnter();
                data.pToggle->isHovered = true;
            }

            if (InputSystem::IsMouseButtonDown(InputMouse::LEFT_MOUSE))
            {
                if (data.pToggle->isToggled == true)
                {
                    data.pToggle->isToggled = false;
                }
                else
                {
                    data.pToggle->isToggled = true;
                }

                data.pSM->GetStateGlobal()->OnButtonDown();
            }
            else if (InputSystem::IsMouseButtonPressed(InputMouse::LEFT_MOUSE))
            {
                data.pSM->GetStateGlobal()->OnButtonPressed();
            }
            else if (InputSystem::IsMouseButtonUp(InputMouse::LEFT_MOUSE))
            {
                data.pSM->GetStateGlobal()->OnButtonUp();
            }
        }
        else
        {
            if (data.pToggle->isHovered == true)
            {
                data.pSM->GetStateGlobal()->OnHoveredExit();
                data.pToggle->isHovered = false;
            }
        }

        //data.pSprite->pTransform.SetWorldPosition(data.pTransform->transform.GetWorldPosition());
    }
}

void UIToggleSystem::OnUpdateMapEntity(uint64_t _maskUpdate, Entity* _pEntity, bool _isNew)
{
    if (_isNew)
    {
        if (_maskUpdate == COMPONENT_MASK(UIToggleComponent))
        {
            DataToggle newToggle;

            newToggle.pToggle = m_pOwnerScene->GetComponentType<UIToggleComponent>(_pEntity);
            newToggle.pTransform = m_pOwnerScene->GetComponentType<TransformComponent>(_pEntity);
            newToggle.pSM = m_pOwnerScene->GetComponentType<StateMachineComponent>(_pEntity);
            newToggle.pSprite = m_pOwnerScene->GetComponentType<SpriteComponent>(_pEntity);

            m_vToggles[_pEntity->id] = newToggle;
        }
    }
    else
    {
        if (m_vToggles.contains(_pEntity->id))
        {
            m_vToggles.erase(_pEntity->id);
        }
    }
}

bool UIToggleSystem::MouseIsInside(DataToggle& _data, XMINT2 _mousePos)
{
    int buttonSizeX = _data.pSprite->width;
    int buttonSizeY = _data.pSprite->height;

    float buttonPosX = _data.pTransform->transform.GetWorldPosition().x - ((float)buttonSizeX * 0.5f);
    float buttonPosY = _data.pTransform->transform.GetWorldPosition().y - ((float)buttonSizeY * 0.5f);

    float mouseX = (float)_mousePos.x;
    float mouseY = -(float)_mousePos.y;

    if (mouseX >= buttonPosX && mouseX <= buttonPosX + buttonSizeX && mouseY >= buttonPosY && mouseY <= buttonPosY + buttonSizeY)
    {
        return true;
    }
    else
    {
        return false;
    }
}