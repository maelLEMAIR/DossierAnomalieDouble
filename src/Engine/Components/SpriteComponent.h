#ifndef SPRITE_COMPONENT_H_DEFINED
#define SPRITE_COMPONENT_H_DEFINED 

#include "Component.h"
#include "Render/Generic/Render.h"
#include "../RessourceManager.h"

class SpriteComponent : public Component
{
public:
    Sprite* pRect = nullptr;
    UiMaterial* pMaterial = nullptr;

    int width = 0;
    int height = 0;

    XMFLOAT2 offsetCenter = { 0.0f, 0.0f };
    XMFLOAT2 ratioScreen = { 0.0f, 0.0f };

    void SetTexture(Texture* _pTexture)
    {
        pMaterial = RessourceManager::GetUiShader("Default")->CreateMaterial();
        pMaterial->SetTexture("Image", _pTexture);
    }

    bool isToDraw = true;
};

#endif