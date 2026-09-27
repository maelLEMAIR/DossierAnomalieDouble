#ifndef TRAP_STATE_MACHINE_CPP_INCLUDED
#define TRAP_STATE_MACHINE_CPP_INCLUDED

#include "TrapStateMachine.h"
#include "Rooms/Room.h"
#include "MainScene.h"
#include "Rooms/RevealRoom.h"
#include "MainScene_Client.h"

void TrapStateMachine::OnStart()
{
    XMFLOAT3 posDoor = pDoorTransform->transform.GetWorldPosition();
    XMFLOAT3 scaleDoor = pDoorTransform->transform.GetWorldScale();
    XMFLOAT3 posTrap = pTrapTransform->transform.GetWorldPosition();
    XMFLOAT3 scaleTrap = pTrapTransform->transform.GetWorldScale();

    m_pOpenDoorSound = AudioEngine::LoadWav(L"../../res/Audio/doorOpen.wav");
    m_pCloseSound = AudioEngine::LoadWav(L"../../res/Audio/doorClose.wav");

    m_pOpenTrapdoorSound = AudioEngine::LoadWav(L"../../res/Audio/trapdoor.wav");
    
    m_pOpenDoorTween = TweenSystem::Create(
            posDoor,
            posDoor + pDoorTransform->transform.GetRight() * scaleDoor * 1.5f,
            Interpolation::easingOut_cubic
        );

    m_pOpenTrapTween = TweenSystem::Create(
            posTrap,
            posTrap + pTrapTransform->transform.GetRight() * scaleTrap * 2.0f,
            Interpolation::easingOut_cubic
        );

    if ( m_pRoom )
    {
        m_pRoom->AddTween(m_pOpenDoorTween);
        m_pRoom->AddTween(m_pOpenTrapTween);
    }
}

void TrapStateMachine::OnCollisionEnter(Entity* other)
{
    if (other->name != "Player")
        return;

    Scene* p_scene = SceneManager::GetSceneWithName("MainScene");
    MainScene* p_mainScene = reinterpret_cast<MainScene*>(p_scene);

    p_mainScene->m_inGameData.ownStateLever = m_answerOfHasAnomaly;
    p_mainScene->m_inGameData.inRevealeRoom = true;

    if (p_mainScene->isClient)
    {
        Data data;
        data.Init(Cmd::STATE_LEVER, { &m_answerOfHasAnomaly }, { Type::TYPE_BOOL });

        EngineManager::GetSocket()->SendSecurTo(data, SERVEUR);

        MainScene_Client* pMainSceneClient = reinterpret_cast<MainScene_Client*>(p_mainScene);
        pMainSceneClient->m_canLoadNextRoom = true;
    }
}

void TrapStateMachine::OnCollisionStay(Entity* other)
{
    if (other->name != "Player")
        return;
    
    if (m_chrono.GetElapsedTime() > m_delay && start_chrono)
    {
        /*if (m_answerOfHasAnomaly == HasAnomaly)
            OpenNextDoor();
        else
            OpenTrap();*/

        if (openTrap)
            OpenTrap();
        else
            OpenNextDoor();
    }
}

void TrapStateMachine::OnCollisionExit(Entity* other)
{
    if (other->name != "Player")
        return;
    
    /*if ( m_answerOfHasAnomaly == HasAnomaly )
        CloseNextDoor();
    else
        CloseTrap();*/

    if (openTrap)
        CloseTrap();
    else
        CloseNextDoor();

    Scene* p_scene = SceneManager::GetSceneWithName("MainScene");
    MainScene* p_mainScene = reinterpret_cast<MainScene*>(p_scene);
    p_mainScene->EnterNewRoom();    
    p_mainScene->m_inGameData.inRevealeRoom = false;
}

void TrapStateMachine::StartCooldown()
{
    m_chrono.Reset();
    m_chrono.Start();
    start_chrono = true;
}

void TrapStateMachine::OpenNextDoor()
{
    if ( m_doorOpen == false )
    {
        XMFLOAT3 pos = pDoorTransform->transform.GetWorldPosition();
        AudioEngine::Play3D(m_pOpenDoorSound, pos.x, pos.y, pos.z, 1.0);
        m_pOpenDoorTween->StartDuration(0.5f, Function::Position, &pDoorTransform->transform, false);
        m_doorOpen = true;

        m_pRoom->ChangeColorLight(true);
    }
}

void TrapStateMachine::CloseNextDoor()
{
    if ( m_doorOpen )
    {
        XMFLOAT3 pos = pDoorTransform->transform.GetWorldPosition();
        AudioEngine::Play3D(m_pCloseSound, pos.x, pos.y, pos.z, 1.0);
        m_pOpenDoorTween->StartDuration(1.5f, Function::Position, &pDoorTransform->transform, true);
        m_doorOpen = false;
    }
}

void TrapStateMachine::OpenTrap()
{
    if ( m_trapOpen == false )
    {
        XMFLOAT3 pos = pTrapTransform->transform.GetWorldPosition();
        AudioEngine::Play3D(m_pOpenTrapdoorSound, pos.x, pos.y, pos.z, 1.0);
        m_pOpenTrapTween->StartDuration(0.5f, Function::Position, &pTrapTransform->transform, false);
        m_trapOpen = true;

        m_pRoom->ChangeColorLight(false);
    }
}

void TrapStateMachine::CloseTrap()
{
    if ( m_trapOpen )
    {
        m_pOpenTrapTween->StartDuration(2.0f, Function::Position, &pTrapTransform->transform, true);
        m_trapOpen = false;
    }
}

void TrapStateMachine::Reset()
{
    openTrap = false;
    start_chrono = false;
    m_chrono.Reset();
}

#endif
