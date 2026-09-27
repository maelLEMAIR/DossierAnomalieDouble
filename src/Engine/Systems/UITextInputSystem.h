#ifndef UI_TEXT_INPUT_SYSTEM_H_INCLUDED
#define UI_TEXT_INPUT_SYSTEM_H_INCLUDED

#include "System.h"

class UITextInputComponent;
class TransformComponent;
class StateMachineComponent;
class SpriteComponent;
class TextComponent;

struct DataTextInput
{
    UITextInputComponent* pTextInput = nullptr;
    TransformComponent* pTransform = nullptr;
    StateMachineComponent* pSM = nullptr;
    SpriteComponent* pSprite = nullptr;
    TextComponent* pText = nullptr;
};

class UITextInputSystem : public System
{
public:
    void OnInit() override;
private:
    std::unordered_map<int, DataTextInput> m_vInputs; //pk une map ??

    void Update(float dt) override;
    void OnUpdateMapEntity(uint64_t _maskUpdate, Entity* _pEntity, bool _isNew = true) override;

    bool MouseIsInside(DataTextInput& _data, XMINT2 _mousePos);
};

#endif