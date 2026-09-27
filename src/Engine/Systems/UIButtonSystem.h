#ifndef UI_BUTTON_SYSTEM_H_INLUDED
#define UI_BUTTON_SYSTEM_H_INLUDED

#include "System.h"

class UIButtonComponent;
class TransformComponent;
class StateMachineComponent;
class SpriteComponent;

struct DataButton
{
    UIButtonComponent* pButton = nullptr;
    TransformComponent* pTransform = nullptr;
    StateMachineComponent* pSM = nullptr;
    SpriteComponent* pSprite = nullptr;
};

class UIButtonSystem : public System
{
public:
    void OnInit() override;
private:
    std::unordered_map<int, DataButton> m_vButtons;

    void Update(float dt) override;
    void OnUpdateMapEntity(uint64_t _maskUpdate, Entity* _pEntity, bool _isNew = true) override;

    bool MouseIsInside(DataButton& _data, XMINT2 _mousePos);
};

#endif