#include "UIButtonSystem.h"   
#include "Engine/RessourceManager.h"
#include "InputSystem.h"
 
#include "components.h"
#include <iostream>

void UIButtonSystem::OnInit()
{
	SetMaskLoadComponents<UIButtonComponent, TransformComponent, StateMachineComponent, SpriteComponent>();
}

void UIButtonSystem::Update(float dt)
{
	XMINT2 mousePos = InputSystem::GetMousePositionCenter();

	for (auto& [id, data] : m_vButtons)
	{
		if (data.pButton->GetOwner()->isActive == false) continue;
		if (data.pButton->isActive == false || data.pTransform->isActive == false || data.pSM->isActive == false) continue;

		if (MouseIsInside(data, mousePos))
		{
			if (data.pButton->isHovered == false)
			{
				data.pSM->GetStateGlobal()->OnHoveredEnter();
				data.pSprite->SetTexture(data.pButton->textureHover);
			}

			data.pButton->isHovered = true;

			if (InputSystem::IsMouseButtonDown(InputMouse::LEFT_MOUSE))
			{
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
			if (data.pButton->isHovered)
			{
				data.pSM->GetStateGlobal()->OnHoveredExit();
				data.pSprite->SetTexture(data.pButton->textureNormal);
			}

			data.pButton->isHovered = false;
		}

		/*data.pSprite->pTransform.SetWorldPosition(data.pTransform->transform.GetWorldPosition());*/
	}	
}

void UIButtonSystem::OnUpdateMapEntity(uint64_t _maskUpdate, Entity* _pEntity, bool _isNew)
{
	if (_isNew)
	{
		if (_maskUpdate == COMPONENT_MASK(UIButtonComponent))
		{
			DataButton newButton;

			newButton.pButton = m_pOwnerScene->GetComponentType<UIButtonComponent>(_pEntity);
			newButton.pTransform = m_pOwnerScene->GetComponentType<TransformComponent>(_pEntity);
			newButton.pSM = m_pOwnerScene->GetComponentType<StateMachineComponent>(_pEntity);
			newButton.pSprite = m_pOwnerScene->GetComponentType<SpriteComponent>(_pEntity); 

			m_vButtons[_pEntity->id] = newButton;
		}
	}
	else
	{
		if (m_vButtons.contains(_pEntity->id))
		{
			m_vButtons.erase(_pEntity->id);
		}
	}
}
bool UIButtonSystem::MouseIsInside(DataButton& _data, XMINT2 _mousePos)
{
	int buttonSizeX = _data.pSprite->width;
	int buttonSizeY = _data.pSprite->height;

	int buttonPosX = (int)(_data.pTransform->transform.GetWorldPosition().x - ((float)buttonSizeX * 0.5f));
	int buttonPosY = (int)(_data.pTransform->transform.GetWorldPosition().y - ((float)buttonSizeY * 0.5f));

	int mouseX = _mousePos.x;
	int mouseY = -_mousePos.y;

	if (mouseX >= buttonPosX && mouseX <= buttonPosX + buttonSizeX && mouseY >= buttonPosY && mouseY <= buttonPosY + buttonSizeY)
	{
		return true;
	}
	else
	{
		return false;
	}
}