#ifndef RESSOURCE_MANAGER_CPP_DEFINED
#define RESSOURCE_MANAGER_CPP_DEFINED

#include "RessourceManager.h"

RessourceManager* RessourceManager::s_pInstance = nullptr;

RessourceManager::RessourceManager()
{
    s_pInstance = this;
}

RessourceManager::~RessourceManager()
{
}

void RessourceManager::AddGeometry(std::string _name, Geometry* _pGeo)
{
    if (_pGeo == nullptr) return;

    s_pInstance->m_mGeometries[_name] = _pGeo;
}

Geometry* RessourceManager::GetGeometry(std::string _name)
{
    if (s_pInstance->m_mGeometries.contains(_name) == false) return nullptr;

    return s_pInstance->m_mGeometries[_name];
}

void RessourceManager::AddShader(std::string _name, Shader* _pShader)
{
    if (_pShader == nullptr) return;

    s_pInstance->m_mShaders[_name] = _pShader;
}

Shader* RessourceManager::GetShader(std::string _name)
{
    if (s_pInstance->m_mShaders.contains(_name) == false) return nullptr;

    return s_pInstance->m_mShaders[_name];
}

void RessourceManager::AddUiShader(std::string _name, UiShader* _pShader)
{
    if (_pShader == nullptr) return;

    s_pInstance->m_mUiShaders[_name] = _pShader;
}

UiShader* RessourceManager::GetUiShader(std::string _name)
{
    if (s_pInstance->m_mUiShaders.contains(_name) == false) return nullptr;

    return s_pInstance->m_mUiShaders[_name];
}

void RessourceManager::AddFont(std::string _name, RenderFont* _pFont)
{
    if (_pFont == nullptr) return;

    s_pInstance->m_mFonts[_name] = _pFont;
}

RenderFont* RessourceManager::GetFont(std::string _name)
{
    if (s_pInstance->m_mFonts.contains(_name) == false) return nullptr;

    return s_pInstance->m_mFonts[_name];
}

void RessourceManager::AddBrush(std::string _name, Brush* _pBrush)
{
    if (_pBrush == nullptr) return;

    s_pInstance->m_mBrushs[_name] = _pBrush;
}

Brush* RessourceManager::GetBrush(std::string _name)
{
    if (s_pInstance->m_mBrushs.contains(_name) == false) return nullptr;

    return s_pInstance->m_mBrushs[_name];
}

void RessourceManager::AddTexture(std::string _name, Texture* _pTexture)
{
    if (_pTexture == nullptr) return;

    s_pInstance->m_mTextures[_name] = _pTexture;
}

Texture* RessourceManager::GetTexture(std::string _name)
{
    if (s_pInstance->m_mTextures.contains(_name) == false) return nullptr;

    return s_pInstance->m_mTextures[_name];
}

void RessourceManager::AddMaterial(std::string name, Material* pMat)
{
    if (pMat == nullptr) return;

    //if (m_mMaterials.contains(name)) return;

    s_pInstance->m_mMaterials[name] = pMat;
}

Material* RessourceManager::GetMaterial(std::string _name)
{
    if (s_pInstance->m_mMaterials.contains(_name) == false) return nullptr;

    return s_pInstance->m_mMaterials[_name];
}

#endif