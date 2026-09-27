#ifndef MESH_RENDERER_SYSTEM_CPP_INLUDED
#define MESH_RENDERER_SYSTEM_CPP_INLUDED

#include "RenderSystem.h"

#include "InputSystem.h"
#include "Components/ColliderData.h"
#include "Components/DebugColliderComponent.h"
#include "Components/MeshRenderer.h"
#include "Components/TransformComponent.h"
#include "Engine/RessourceManager.h"
#include "Components/TextComponent.h"
#include "Components/SpriteComponent.h"


#include <iostream>
void RenderSystem::OnInit()
{
    SetMaskOrComponents<MeshRenderer, TextComponent, SpriteComponent, DebugColliderComponent>();
}

void RenderSystem::Update(float dt)
{
    Device* pDevice = EngineManager::GetDevice();

    pDevice->BeginDraw(EngineManager::GetWindow()->GetSwapChain()->GetBackBuffer(), EngineManager::GetWindow()->GetSwapChain()->GetDepthStencil());

    if ( InputSystem::IsKeyDown(F3) )
        m_isWireframe = !m_isWireframe;

    if ( m_isWireframe == false )
    {
        for (MeshRenderer* mesh : m_v3D)
        {
            if (mesh->GetOwner()->isActive == false) continue;
            if (mesh->isActive == false) continue;

            Entity* pOwner = mesh->GetOwner();
            Scene* current = SceneManager::GetCurrentScene();
            TransformComponent* transform = current->GetComponentType<TransformComponent>(pOwner);

            if (transform == nullptr) continue;

            Geometry* pGeo = mesh->pGeometry;
            if (pGeo == nullptr) continue;

            if (mesh->pMaterial == nullptr)
                mesh->pMaterial = RessourceManager::GetMaterial("Default");

            pDevice->SetMaterial(mesh->pMaterial);
            pDevice->Draw(mesh->pGeometry, transform->transform.GetWorldMatrix());
        }
    }
    
    if ( m_isWireframe )
    {
        for (DebugColliderComponent* debug : m_vDebugCollider)
        {
            if (debug->GetOwner()->isActive == false) continue;
            if (debug->isActive == false) continue;

            Entity* pOwner = debug->GetOwner();
            Scene* current = SceneManager::GetCurrentScene();
            ColliderData* pColliderData = debug->pColliderData;
        
            TransformComponent* pTransform = current->GetComponentType<TransformComponent>(pOwner);
            Transform transformCollider = Transform(pTransform->transform);

            
            XMFLOAT3 scaleCollider = pColliderData->pCollider->scale;
            XMVECTOR scaleVCollider = XMLoadFloat3(&scaleCollider);
            XMFLOAT3 scaleTransform = pTransform->transform.GetWorldScale();
            XMVECTOR scaleVTransform = XMLoadFloat3(&scaleTransform);
            XMVECTOR scaleVFinal = XMVectorMultiply(scaleVTransform, scaleVCollider);
            XMFLOAT3 scaleFinal;
            XMStoreFloat3(&scaleFinal, scaleVFinal);
            transformCollider.SetWorldScale(scaleFinal);
            transformCollider.UpdateWorldMatrix();

            XMFLOAT4X4 worldMatrix = pTransform->transform.GetWorldMatrix();
            XMFLOAT4X4 worldCollMatrix = transformCollider.GetWorldMatrix();

            Geometry* pGeo = debug->pGeo;
            if (pGeo == nullptr) continue;

            Material* pMaterial = RessourceManager::GetMaterial("Wireframe");
            pDevice->SetMaterial(pMaterial);
            pDevice->Draw(debug->pGeo, transformCollider.GetWorldMatrix());
        }
    }
    
    for (SpriteComponent* pSprite : m_v2DSprite)
    {
        if (pSprite->GetOwner()->isActive == false) continue;
        if (pSprite->isActive == false) continue;

        if (pSprite->pRect == nullptr) continue;
        if (pSprite->pMaterial == nullptr && pSprite->isToDraw) continue;

        Entity* pOwner = pSprite->GetOwner();
        Scene* current = SceneManager::GetCurrentScene();
        TransformComponent* transform = current->GetComponentType<TransformComponent>(pOwner);

        if (transform == nullptr) continue;

        int width = EngineManager::GetWindow()->GetWidth();
        int height = EngineManager::GetWindow()->GetHeight();
         
        Transform temp = transform->transform;
         
        if (pSprite->offsetCenter.x != 0.0f || pSprite->offsetCenter.y != 0.0f)
        {
            temp.MoveWorld({ width * pSprite->offsetCenter.x, height * pSprite->offsetCenter.y, 0.0f });
        }
         
        if (width != 0 && height != 0)
        {
            pSprite->ratioScreen.x = temp.GetWorldScale().x / width;
            pSprite->ratioScreen.y = temp.GetWorldScale().y / height;
        }

        temp.UpdateWorldMatrix();

        if (pSprite->isToDraw)
        {
            pDevice->SetUiMaterial(pSprite->pMaterial);
            pDevice->DrawUi(pSprite->pRect, temp.GetWorldMatrix());
        }
    }

    for (TextComponent* pText : m_v2DText)
    {
        if (pText->GetOwner()->isActive == false) continue;
        if (pText->isActive == false) continue;

        Entity* pOwner = pText->GetOwner();
        Scene* current = SceneManager::GetCurrentScene();
        TransformComponent* transform = current->GetComponentType<TransformComponent>(pOwner);

        int width = EngineManager::GetWindow()->GetWidth();
        int height = EngineManager::GetWindow()->GetHeight();

        Transform temp = transform->transform;
        temp.MoveWorld({ width * pText->offsetCenter.x, height * pText->offsetCenter.y, 0.0f });
        temp.UpdateWorldMatrix();

        pDevice->DrawRenderText(pText->m_pText, temp.GetWorldMatrix());
    }

    pDevice->EndDraw();
   
    EngineManager::GetWindow()->GetSwapChain()->Present();
}

void RenderSystem::OnUpdateMapEntity(uint64_t _maskUpdate, Entity* _pEntity, bool _isNew)
{
    if (_isNew)
    {
        if (_maskUpdate == COMPONENT_MASK(MeshRenderer))
        {
            MeshRenderer* pMesh = m_pOwnerScene->GetComponentType<MeshRenderer>(_pEntity);
            m_v3D.push_back(pMesh);
        }
        else if (_maskUpdate == COMPONENT_MASK(TextComponent))
        {
            TextComponent* pText = m_pOwnerScene->GetComponentType<TextComponent>(_pEntity);
            m_v2DText.push_back(pText);
        }
        else if (_maskUpdate == COMPONENT_MASK(SpriteComponent))
        {
            SpriteComponent* pSprite = m_pOwnerScene->GetComponentType<SpriteComponent>(_pEntity);
            m_v2DSprite.push_back(pSprite);
        }
        else if (_maskUpdate == COMPONENT_MASK(DebugColliderComponent))
        {
            DebugColliderComponent* pDebugColl = m_pOwnerScene->GetComponentType<DebugColliderComponent>(_pEntity);
            m_vDebugCollider.push_back(pDebugColl);
        }
    }
    else
    {
        if (_maskUpdate == COMPONENT_MASK(MeshRenderer))
        {
            int i = 0;
            for (MeshRenderer* pMesh : m_v3D)
            {
                if (pMesh->GetOwner()->id == _pEntity->id)
                {
                    m_v3D.erase(m_v3D.begin() + i);
                    return;
                }
                i++;
            }
        }
        else if (_maskUpdate == COMPONENT_MASK(SpriteComponent))
        {
            int i = 0;
            for (SpriteComponent* pSprite : m_v2DSprite)
            {
                if (pSprite->GetOwner()->id == _pEntity->id)
                {
                    m_v2DSprite.erase(m_v2DSprite.begin() + i);
                    return;
                }
                i++;
            }
        }
        else if (_maskUpdate == COMPONENT_MASK(TextComponent))
        {
            int i = 0;
            for (TextComponent* pText : m_v2DText)
            {
                if (pText->GetOwner()->id == _pEntity->id)
                {
                    m_v2DText.erase(m_v2DText.begin() + i);
                    return;
                }

                i++;
            }
        }
        else if (_maskUpdate == COMPONENT_MASK(DebugColliderComponent))
        {
            int i = 0;
            for (DebugColliderComponent* pText : m_vDebugCollider)
            {
                if (pText->GetOwner()->id == _pEntity->id)
                {
                    m_vDebugCollider.erase(m_vDebugCollider.begin() + i);
                    return;
                }
                i++;
            }
        }
    }
}

#endif