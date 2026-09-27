#include "UISliderSystem.h"  
#include "Engine/RessourceManager.h"
#include "InputSystem.h"
#include "components.h"

void UISliderSystem::OnInit()
{
    SetMaskLoadComponents<UISliderComponent, TransformComponent, StateMachineComponent, SpriteComponent>();
}

void UISliderSystem::Update(float dt)
{
    XMINT2 mousePos = InputSystem::GetMousePositionCenter();

    for (auto& [id, data] : m_vSliders)
    {
        if (data.pSlider->GetOwner()->isActive == false) continue;
        if (data.pSlider->isActive == false || data.pTransform->isActive == false || data.pSM->isActive == false || data.pSprite->isActive == false) continue;

        bool isInside = MouseIsInside(data, mousePos);
         
        if (isInside == true)
        {
            if (data.pSlider->isHovered == false)
            {
                data.pSM->GetStateGlobal()->OnHoveredEnter();
                data.pSlider->isHovered = true;
            }

            if (InputSystem::IsMouseButtonDown(InputMouse::LEFT_MOUSE))
            {
                data.pSlider->isDragging = true;
                data.pSM->GetStateGlobal()->OnButtonDown();
            }
        }
        else
        {
            if (data.pSlider->isHovered == true)
            {

                if (data.pSlider->isDragging == false)
                {
                    data.pSM->GetStateGlobal()->OnHoveredExit();
                    data.pSlider->isHovered = false;
                }
            }
        }


        if (InputSystem::IsMouseButtonUp(InputMouse::LEFT_MOUSE))
        {
            if (data.pSlider->isDragging == true)
            {
                data.pSlider->isDragging = false;
                data.pSM->GetStateGlobal()->OnButtonUp();

                if (isInside == false)
                {
                    data.pSM->GetStateGlobal()->OnHoveredExit();
                    data.pSlider->isHovered = false;
                }
            }
        }


        if (data.pSlider->isDragging == true)
        {
            int bgWidth = data.pSprite->width;
            float bgStartX = data.pTransform->transform.GetWorldPosition().x - ((float)bgWidth * 0.5f);
            float localMouseX = (float)mousePos.x - bgStartX;

            float percentage = localMouseX / bgWidth;


            if (percentage < 0.0f)
            {
                percentage = 0.0f;
            }
            if (percentage > 1.0f)
            {
                percentage = 1.0f;
            }

            data.pSlider->currentValue = data.pSlider->minValue + (percentage * (data.pSlider->maxValue - data.pSlider->minValue));


            data.pSM->GetStateGlobal()->OnButtonPressed();
        }


        if (data.pSlider->pKnobEntity != nullptr)
        {
            TransformComponent* knobTransform = m_pOwnerScene->GetComponentType<TransformComponent>(data.pSlider->pKnobEntity);

            if (knobTransform != nullptr)
            {
                float range = data.pSlider->maxValue - data.pSlider->minValue;
                float currentPercentage = 0.0f;

                if (range > 0.0f)
                {
                    currentPercentage = (data.pSlider->currentValue - data.pSlider->minValue) / range;
                }

                int bgWidth = data.pSprite->width;
                float bgStartX = data.pTransform->transform.GetWorldPosition().x - ((float)bgWidth * 0.5f);
                float newX = bgStartX + (bgWidth * currentPercentage);


                knobTransform->transform.SetWorldPosition({ newX, data.pTransform->transform.GetWorldPosition().y, data.pTransform->transform.GetWorldPosition().z - 1.0f });
            }
        }

        //data.pSprite->pTransform.SetWorldPosition(data.pTransform->transform.GetWorldPosition());
    }
}

void UISliderSystem::OnUpdateMapEntity(uint64_t _maskUpdate, Entity* _pEntity, bool _isNew)
{
    if (_isNew)
    {
        if (_maskUpdate == COMPONENT_MASK(UISliderComponent))
        {
            DataSlider newSlider;

            newSlider.pSlider = m_pOwnerScene->GetComponentType<UISliderComponent>(_pEntity);
            newSlider.pTransform = m_pOwnerScene->GetComponentType<TransformComponent>(_pEntity);
            newSlider.pSM = m_pOwnerScene->GetComponentType<StateMachineComponent>(_pEntity);
            newSlider.pSprite = m_pOwnerScene->GetComponentType<SpriteComponent>(_pEntity);

            m_vSliders[_pEntity->id] = newSlider;
        }
    }
    else
    {
        if (m_vSliders.contains(_pEntity->id))
        {
            m_vSliders.erase(_pEntity->id);
        }
    }
}

bool UISliderSystem::MouseIsInside(DataSlider& _data, XMINT2 _mousePos)
{
    int buttonSizeX = _data.pSprite->width;
    int buttonSizeY = _data.pSprite->height;

    float buttonPosX = _data.pTransform->transform.GetWorldPosition().x - ((float)buttonSizeX * 0.5f);
    float buttonPosY = _data.pTransform->transform.GetWorldPosition().y - ((float)buttonSizeY * 0.5f);

    float mouseX = (float)_mousePos.x;

    float mouseY = (float)-_mousePos.y;

    if (mouseX >= buttonPosX && mouseX <= buttonPosX + buttonSizeX && mouseY >= buttonPosY && mouseY <= buttonPosY + buttonSizeY)
    {
        return true;
    }
    else
    {
        return false;
    }
}