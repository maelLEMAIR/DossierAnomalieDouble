#ifndef TEST_DATA_CASH_H_DEFINED
#define TEST_DATA_CASH_H_DEFINED

#include "Test.h"
#include "Engine/EngineManager.h"

#include "MapLoader.h"

class TestDataCash : public Test
{
public: 
    static void Run()
    {
         EngineManager engineManager;
         engineManager.Initialize(1080, 720, L"Test");
         Scene* scene = SceneManager::GetSceneWithName("Default");
        
         Device* pDevice = engineManager.GetDevice();

         std::cout << "Start loading" << std::endl;
         float tempTime = EngineManager::GetChrono().GetTotalTime();
         //MapLoader::LoadMap("../../res/JSON/map_Bedroom.json", pDevice, *scene);
         MapLoader::LoadCashMap("map_Bedroom", pDevice, *scene);
         float timeLoad = EngineManager::GetChrono().GetTotalTime() - tempTime;
         std::cout << "End loading : " << timeLoad << std::endl;

        
         engineManager.Run();
    }
};

#endif