#ifndef UI_TOGGLE_SYSTEM_H_DEFINED
#define UI_TOGGLE_SYSTEM_H_DEFINED

#include "../System.h"
#include "../Entity.h"
#include "../Scene.h"
#include "components.h"
#include <unordered_map>

class UIToggleComponent;

struct DataToggle
{
    UIToggleComponent* pToggle = nullptr;
    TransformComponent* pTransform = nullptr;
    StateMachineComponent* pSM = nullptr;
    SpriteComponent* pSprite = nullptr;
};

class UIToggleSystem : public System
{
public:
    void OnInit() override;
    void Update(float dt) override;
    void OnUpdateMapEntity(uint64_t _maskUpdate, Entity* _pEntity, bool _isNew) override;

private:
    bool MouseIsInside(DataToggle& _data, XMINT2 _mousePos);

    std::unordered_map<int, DataToggle> m_vToggles;
};

#endif