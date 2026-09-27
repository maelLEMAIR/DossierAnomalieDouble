#ifndef OPTIONS_MENU_SCENE_H_DEFINED
#define OPTIONS_MENU_SCENE_H_DEFINED

#include "Engine/Scene.h"
#include <vector>

class Entity;

class OptionsMenuScene : public Scene
{
public:
    void OnInit() override;
    void OnUpdate(float dt) override;

private:
    Entity* m_pBtnRetour = nullptr;
    Entity* m_pSliderVolumeBg = nullptr;
    Entity* m_pSliderVolumeKnob = nullptr;
    Entity* m_pToggleFullscreen = nullptr;
    Entity* m_pDropdownResolution = nullptr;

    std::vector<Entity*> m_dropdownOptions;
    std::vector<Entity*> m_dropdownTexts;


    Entity* m_pCamera = nullptr;
};

#endif