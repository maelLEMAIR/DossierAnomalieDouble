#ifndef TEST_H_DEFINED
#define TEST_H_DEFINED

#include "Engine/EngineManager.h"
#include "Render/Generic/Render.h"
#include "Engine/components.h"
#include "Engine/systems.h"

class Test
{
public:
    Test() = default;
    virtual ~Test() = default;
    
    static void Run() {}
};

#endif