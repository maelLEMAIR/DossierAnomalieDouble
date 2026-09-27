#ifndef ENGINE_MANAGER_H_DEFINED
#define ENGINE_MANAGER_H_DEFINED

#include <WinSock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "Ws2_32.lib")

// Engine
#include "SceneManager.h"
#include "define.h"
#include "Data.h"
#include "RayCast.h"

// Render
#include "Core/Chrono.h"
#include "../Render/Generic/Render.h"

//Network
#include "Socket.h"
#include "Serveur.h"

#define SERVEUR "6C02E0505069"

//Audio
#include "AudioEngine/AudioEngine.h"
#include "Components/LightComponent.h"

class Scene;
class Entity;
class Tween;

class EngineManager
{
public:
    float factorTime = 1.0f;

    EngineManager();
    ~EngineManager();

    static EngineManager& GetInstance() { return *s_pInstance; }
    
    void Initialize(UINT _width, UINT _height, WString _title, bool _isFullScreen = false);
    void Run();
    LightComponent* GetFirstAvailableLight();
    void Exit();

    Entity* CreateEntity();
    
    static float GetDeltaTime() { return s_pInstance->m_deltaTime; }
    static float GetTotalTime() { return s_pInstance->m_TotalTime; }
    static Window* GetWindow() { return s_pInstance->m_pWindow; }
    static Device* GetDevice() { return s_pInstance->m_pDevice; }
    static std::mt19937& GetRand() { return s_pInstance->m_rng; }

    static void InitServeur(PCSTR _ip, int _port);
    static void InitClient(PCSTR _ip, int _port);

    static Socket* GetSocket() { return s_pInstance->m_pSock; }
    static void CloseSocket();
    static Chrono GetChrono() { return s_pInstance->m_chrono; }

    static AudioEngine* GetAudioEngine() { return s_pInstance->m_pAudioEngine; };
    
private:
    static EngineManager* s_pInstance;
    
    Vector<LightComponent*> m_vLights;
    AudioEngine* m_pAudioEngine = nullptr;
    
    Chrono m_chrono;
    float m_deltaTime = 0.0f;
    
    Camera* m_pCamera = nullptr;
    
    Window* m_pWindow = nullptr;
    Device* m_pDevice = nullptr;
    
    SceneManager* m_pSceneManager;

    std::mt19937 m_rng;
    
    float m_TotalTime = 0.0f;

    int m_componentCount = 0;
    template <typename ComponentType>
    static void RegisterComponent();

    Socket* m_pSock = nullptr;

    friend class System;
    friend class Scene;
};

#include "EngineManager.inl"

#endif

