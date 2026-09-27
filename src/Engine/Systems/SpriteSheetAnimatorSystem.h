#ifndef SPRITESHEET_ANIMATOR_SYSTEM_H_DEFINED
#define SPRITESHEET_ANIMATOR_SYSTEM_H_DEFINED

#include "System.h" 

class SpriteSheetAnimatorComponent;

class SpriteSheetAnimatorSystem : public System
{
public:
	void OnInit() override;

private:
	std::unordered_map<int, SpriteSheetAnimatorComponent*> m_vSpriteSheet;

	void Update(float dt) override;
	void OnUpdateMapEntity(uint64_t _maskUpdate, Entity* _pEntity, bool _isNew = true) override;
};

#endif