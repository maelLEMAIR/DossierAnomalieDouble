#ifndef MESH_RENDERER_SYSTEM_H_INLUDED
#define MESH_RENDERER_SYSTEM_H_INLUDED

#include "System.h"

class MeshRenderer;
class TextComponent;
class SpriteComponent;
class DebugColliderComponent;

class RenderSystem : public System
{
public:
    void OnInit() override;
private:
    void Update(float dt) override;
    void OnUpdateMapEntity(uint64_t _maskUpdate, Entity* _pEntity, bool _isNew = true) override;

    bool m_isWireframe = false;
    std::vector<MeshRenderer*> m_v3D;
    std::vector<TextComponent*> m_v2DText;
    std::vector<SpriteComponent*> m_v2DSprite;
    std::vector<DebugColliderComponent*> m_vDebugCollider;
};

#endif