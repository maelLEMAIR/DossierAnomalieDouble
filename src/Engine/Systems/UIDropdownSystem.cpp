#include "UIDropdownSystem.h"
#include "InputSystem.h"
#include "components.h"

void UIDropdownSystem::OnInit()
{
    SetMaskLoadComponents<UIDropdownComponent, TransformComponent, StateMachineComponent, SpriteComponent>();
}

void UIDropdownSystem::Update(float dt)
{
    XMINT2 mousePos = InputSystem::GetMousePositionCenter();

    for (auto& [id, data] : m_vDropdowns)
    {
        if (data.pDropdown->GetOwner()->isActive == false) continue;
        if (data.pDropdown->isActive == false || data.pTransform->isActive == false || data.pSM->isActive == false || data.pSprite->isActive == false) continue;

        bool isInsideMain = MouseIsInsideMain(data, mousePos);
        bool isInsideList = false;
        int hoveredOptionIndex = -1;

        if (data.pDropdown->isOpen == true)
        {
            isInsideList = MouseIsInsideList(data, mousePos, hoveredOptionIndex);
        }

        if (isInsideMain == true || isInsideList == true)
        {
            if (data.pDropdown->isHovered == false)
            {
                data.pSM->GetStateGlobal()->OnHoveredEnter();
                data.pDropdown->isHovered = true;
            }
        }
        else
        {
            if (data.pDropdown->isHovered == true)
            {
                data.pSM->GetStateGlobal()->OnHoveredExit();
                data.pDropdown->isHovered = false;
            }
        }

        if (InputSystem::IsMouseButtonDown(InputMouse::LEFT_MOUSE))
        {
            if (isInsideMain == true)
            {
                if (data.pDropdown->isOpen == true)
                {
                    data.pDropdown->isOpen = false;
                }
                else
                {
                    data.pDropdown->isOpen = true;
                }
                data.pSM->GetStateGlobal()->OnButtonDown();
            }
            else if (isInsideList == true)
            {
                data.pDropdown->selectedIndex = hoveredOptionIndex;
                data.pDropdown->isOpen = false;
                data.pSM->GetStateGlobal()->OnButtonPressed();
            }
            else
            {
                if (data.pDropdown->isOpen == true)
                {
                    data.pDropdown->isOpen = false;
                }
            }
        }

        float mainX = data.pTransform->transform.GetWorldPosition().x;
        float mainY = data.pTransform->transform.GetWorldPosition().y;
        float mainZ = data.pTransform->transform.GetWorldPosition().z;

        if (data.pDropdown->pMainTextEntity != nullptr)
        {
            TransformComponent* mainTextTransform = m_pOwnerScene->GetComponentType<TransformComponent>(data.pDropdown->pMainTextEntity);
            if (mainTextTransform != nullptr)
            {
                mainTextTransform->transform.SetWorldPosition(XMFLOAT3(mainX - data.pDropdown->textOffsetX, -(mainY + data.pDropdown->textOffsetY), mainZ ));
            }
        }


        if (data.pDropdown->isOpen == true)
        {
            float startX = mainX;
            float topOfListY = mainY - (data.pSprite->height * 0.5f) - data.pDropdown->gapTop - (data.pDropdown->optionHeight * 0.5f);

            for (size_t i = 0; i < data.pDropdown->optionBackgroundEntities.size(); i++)
            {
                Entity* bgEntity = data.pDropdown->optionBackgroundEntities[i];
                Entity* textEntity = data.pDropdown->optionTextEntities[i];

                if (bgEntity != nullptr)
                {
                    TransformComponent* bgTransform = m_pOwnerScene->GetComponentType<TransformComponent>(bgEntity);
                    if (bgTransform != nullptr)
                    {
                        float currentY = topOfListY - (i * (data.pDropdown->optionHeight + data.pDropdown->gapOptions));
                        bgTransform->transform.SetWorldPosition(XMFLOAT3(startX, currentY, mainZ - 0.1f));

                        if (textEntity != nullptr)
                        {
                            TransformComponent* textTransform = m_pOwnerScene->GetComponentType<TransformComponent>(textEntity);
                            if (textTransform != nullptr)
                            {
                                textTransform->transform.SetWorldPosition(XMFLOAT3(startX - data.pDropdown->textOffsetX, -(currentY + data.pDropdown->textOffsetY), mainZ - 0.15f));
                            }
                        }
                    }
                }
            }
        }
    }
}

void UIDropdownSystem::OnUpdateMapEntity(uint64_t _maskUpdate, Entity* _pEntity, bool _isNew)
{
    if (_isNew)
    {
        if (_maskUpdate == COMPONENT_MASK(UIDropdownComponent))
        {
            DataDropdown newDropdown;

            newDropdown.pDropdown = m_pOwnerScene->GetComponentType<UIDropdownComponent>(_pEntity);
            newDropdown.pTransform = m_pOwnerScene->GetComponentType<TransformComponent>(_pEntity);
            newDropdown.pSM = m_pOwnerScene->GetComponentType<StateMachineComponent>(_pEntity);
            newDropdown.pSprite = m_pOwnerScene->GetComponentType<SpriteComponent>(_pEntity);

            m_vDropdowns[_pEntity->id] = newDropdown;
        }
    }
    else
    {
        if (m_vDropdowns.contains(_pEntity->id))
        {
            m_vDropdowns.erase(_pEntity->id);
        }
    }
}

bool UIDropdownSystem::MouseIsInsideMain(DataDropdown& _data, XMINT2 _mousePos)
{
    int buttonSizeX = _data.pSprite->width;
    int buttonSizeY = _data.pSprite->height;

    int buttonPosX = (int)(_data.pTransform->transform.GetWorldPosition().x - (buttonSizeX * 0.5f));
    int buttonPosY = (int)(_data.pTransform->transform.GetWorldPosition().y - (buttonSizeY * 0.5f));

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

bool UIDropdownSystem::MouseIsInsideList(DataDropdown& _data, XMINT2 _mousePos, int& outIndex)
{
    if (_data.pDropdown->options.empty())
    {
        return false;
    }

    float width = (float)_data.pSprite->width;
    float height = (float)_data.pSprite->height;

    float gapTop = 5.0f;
    float gapOptions = 2.0f;
     
    float startX = _data.pTransform->transform.GetWorldPosition().x - (width * 0.5f);
     
    float topOfListY = _data.pTransform->transform.GetWorldPosition().y - (height * 0.5f) - gapTop;

    float totalListHeight = _data.pDropdown->options.size() * (_data.pDropdown->optionHeight + gapOptions);
     
    float bottomOfListY = topOfListY - totalListHeight;

    float mouseX = (float)_mousePos.x;
    float mouseY = -(float)_mousePos.y;  

    if (mouseX >= startX)
    {
        if (mouseX <= startX + width)
        { 
            if (mouseY <= topOfListY)
            {
                if (mouseY >= bottomOfListY)
                { 
                    float distanceDepuisLeHaut = topOfListY - mouseY;
                    outIndex = (int)(distanceDepuisLeHaut / (_data.pDropdown->optionHeight + gapOptions));

                    if (outIndex >= _data.pDropdown->options.size())
                    {
                        outIndex = (int)_data.pDropdown->options.size() - 1;
                    }
                    if (outIndex < 0)
                    {
                        outIndex = 0;
                    }

                    return true;
                }
            }
        }
    }
    return false;
}