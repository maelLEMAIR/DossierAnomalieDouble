#ifndef TEST_INPUT_HPP_DEFINED
#define TEST_INPUT_HPP_DEFINED

#include "Test.h"
#include "Engine/Systems/InputSystem.h"
#include <iostream>

class TestInput : public Test
{
public:
    static void Run()
    {
        while (true)
        {
            InputSystem::HandleInput();
            
            if (InputSystem::IsKeyDown(A))
            {
                std::cout << "A : Down" << '\n';
            }
            if (InputSystem::IsKeyUp(Q))
            {
                std::cout << "Q : Up" << '\n';
            }
        }
    }
};

#endif