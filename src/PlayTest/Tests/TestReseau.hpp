#ifndef TEST_RESEAU_H_DEFINED
#define TEST_RESEAU_H_DEFINED

#include "Test.h"
#include "Engine/EngineManager.h"
#include "Engine/Serveur.h"
#include "Data.h"

#define OWN_PC "10.10.127.1"
#define IP_PC1 "10.10.137.20"

#define SERVEUR "6C02E0505069"
void Connect()
{
    std::cout << "Connect" << std::endl;
}

class TestReseau : public Test
{
public: 
    static void Run()
    {
         EngineManager engineManager;
         engineManager.Initialize(1080, 720, L"Test");
         Scene* scene = SceneManager::GetSceneWithName("Default");
        
         Device* pDevice = engineManager.GetDevice();

         /*engineManager.InitClient(IP_PC1, 1888);
         engineManager.GetSocket()->CreateProfils((char*)SERVEUR, engineManager.GetSocket()->mAddr);

         Data data;
         data.Init(Cmd::CONNECT);

         engineManager.GetSocket()->SendSecurTo(data, (char*)SERVEUR);*/

         engineManager.InitServeur("0.0.0.0", 1888);

         engineManager.GetSocket()->LinkCmdFunc(Cmd::CONNECT, [](const char* data, std::string id, sockaddr_in* from) {Connect(); });
         engineManager.GetSocket()->StartReading();
        
         engineManager.Run();
    }
};

#endif