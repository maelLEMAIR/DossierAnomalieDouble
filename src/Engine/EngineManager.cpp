#ifndef ENGINE_MANAGER_CPP_DEFINED
#define ENGINE_MANAGER_CPP_DEFINED

#include "EngineManager.h"
#include "Scene.h"
#include "RessourceManager.h"
#include "../Render/Generic/Factories/ShaderFactory.hpp"
#include "systems.h"
#include "Tween.h"

EngineManager* EngineManager::s_pInstance = nullptr;

EngineManager::EngineManager() : m_rng(std::random_device{}()) 
{
    s_pInstance = this;
    m_chrono = Chrono();
}

EngineManager::~EngineManager()
{
    s_pInstance->Exit();
}

void EngineManager::Initialize(UINT _width, UINT _height, WString _title, bool _isFullScreen)
{
    if (m_pWindow == nullptr)
    {
        m_pWindow = new Window((int)_width, (int)_height, _title, _isFullScreen);
        m_pWindow->InitD3D12();
        m_pDevice = m_pWindow->GetDevice();
    }
    m_pAudioEngine = new AudioEngine();
    m_pAudioEngine->Init();
    
    if (m_pSceneManager == nullptr)
        m_pSceneManager = new SceneManager;

    TweenSystem* tweenSystem = new TweenSystem();
    RessourceManager* ressourceManager = new RessourceManager;

    Shader* coloredS = ShaderFactory::CreateLitColored(m_pDevice);
    RessourceManager::AddShader("Color", coloredS);

    Shader* texturedS = ShaderFactory::CreateLitTextured(m_pDevice);
    RessourceManager::AddShader("Texture", texturedS);

    Shader* wireframeS = ShaderFactory::CreateWireframe(m_pDevice);
    ressourceManager->AddShader("Wireframe", wireframeS);

    Material* wireframeM = wireframeS->CreateMaterial();
    wireframeM->SetFloat4("DiffuseAlbedo", {0.0f, 1.0f, 0.0f, 1.0f});
    ressourceManager->AddMaterial("Wireframe", wireframeM);
    
    Material* white = coloredS->CreateMaterial();
    white->SetFloat4("DiffuseAlbedo", {1.0f, 1.0f, 1.0f, 1.0f});
    RessourceManager::AddMaterial("Default", white);

    UiShader* uiColor = ShaderFactory::CreateUIBasic(m_pDevice);
    RessourceManager::AddUiShader("Default", uiColor);

    RenderFont* font = m_pDevice->CreateRenderFont(WRES("/Font/West.ttf"), 40.0f);
    RessourceManager::AddFont("Default", font);

    Geometry* pSphere = GeometryFactory::BuildIcosphere(m_pDevice, 1);
    RessourceManager::AddGeometry("SphereWireframe", pSphere);

    Geometry* pCube = GeometryFactory::BuildCube(m_pDevice);
    RessourceManager::AddGeometry("CubeWireframe", pCube);
    
    //Network
    SOCKETS::Start();
}

void EngineManager::Run()
{
    m_chrono.Start();
    
    while ( m_pWindow->IsOpen() )
    {
        m_deltaTime = m_chrono.Reset() * factorTime;
        m_TotalTime += m_deltaTime;
        
        m_pWindow->Update();

        m_pSceneManager->Update(m_deltaTime, m_pAudioEngine);

        TweenSystem::Update(m_deltaTime);
        
        if (m_pSock != nullptr)
            m_pSock->Update(m_deltaTime);
    }
}

void EngineManager::Exit()
{
    delete m_pSock;
    SOCKETS::Release();
}

Entity* EngineManager::CreateEntity()
{
    return m_pSceneManager->GetCurrentScene()->CreateEntity();
}

void EngineManager::InitServeur(PCSTR _id, int _port)
{
    s_pInstance->m_pSock = new Serveur;
    s_pInstance->m_pSock->usePing = false;
    s_pInstance->m_pSock->InitADDR(_id, _port);
    s_pInstance->m_pSock->SetId((char*)SERVEUR);
}

void EngineManager::InitClient(PCSTR _ip, int _port)
{
    s_pInstance->m_pSock = new Socket;
    s_pInstance->m_pSock->usePing = false;
    s_pInstance->m_pSock->InitADDR(_ip, _port);
    s_pInstance->m_pSock->SetId((char*)_ip);
}

void EngineManager::CloseSocket()
{
    delete s_pInstance->m_pSock;
    s_pInstance->m_pSock = nullptr;
}

#endif
