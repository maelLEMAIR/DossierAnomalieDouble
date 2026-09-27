#ifndef UI_DROPDOWN_SYSTEM_H_DEFINED
#define UI_DROPDOWN_SYSTEM_H_DEFINED

#include "../System.h"
#include "../Entity.h"
#include "../Scene.h"
#include "components.h"
#include <unordered_map>

struct DataDropdown
{
    UIDropdownComponent* pDropdown = nullptr;
    TransformComponent* pTransform = nullptr;
    StateMachineComponent* pSM = nullptr;
    SpriteComponent* pSprite = nullptr;
};

class UIDropdownSystem : public System
{
public:
    void OnInit() override;
    void Update(float dt) override;
    void OnUpdateMapEntity(uint64_t _maskUpdate, Entity* _pEntity, bool _isNew) override;

private:
    bool MouseIsInsideMain(DataDropdown& _data, XMINT2 _mousePos);
    bool MouseIsInsideList(DataDropdown& _data, XMINT2 _mousePos, int& outIndex);

    std::unordered_map<int, DataDropdown> m_vDropdowns;
};

#endif