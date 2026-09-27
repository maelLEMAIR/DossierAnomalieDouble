#ifndef TRAP_STATE_MACHINE_H_INCLUDED
#define TRAP_STATE_MACHINE_H_INCLUDED

#include "pch.h"

class RevealRoom;

class TrapStateMachine : public StateGlobal
{
public:
    void OnStart() override;
    void OnCollisionEnter(Entity* other) override;
    void OnCollisionStay(Entity* other) override;
    void OnCollisionExit(Entity* other) override;
    void SetAnswer(bool hasAnomaly) { m_answerOfHasAnomaly = hasAnomaly; }
    void StartCooldown();
    void Reset();
    
    TransformComponent* pDoorTransform = nullptr;
    TransformComponent* pTrapTransform = nullptr;
    
    RevealRoom* m_pRoom = nullptr;
    
    bool openTrap = false;
private:
    void OpenNextDoor();
    void CloseNextDoor();
    void OpenTrap();
    void CloseTrap();
    
    Text* m_info = nullptr;
    
    Sound* m_pOpenDoorSound = nullptr;
    Sound* m_pCloseSound = nullptr;
    Sound* m_pOpenTrapdoorSound = nullptr;
    
    Tween* m_pOpenTrapTween = nullptr;
    Tween* m_pOpenDoorTween = nullptr;
    bool m_answerOfHasAnomaly = false;

    bool m_trapOpen = false;
    bool m_doorOpen = false;
    
    Chrono m_chrono;
    float m_delay = 5.0f;
    bool start_chrono = false;
};

#endif