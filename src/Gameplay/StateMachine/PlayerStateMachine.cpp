#ifndef PLAYER_STATE_MACHINE_CPP_INCLUDED
#define PLAYER_STATE_MACHINE_CPP_INCLUDED

#include "PlayerStateMachine.h"
#include "GameManager.h"
#include "PingTriangle.h"

void PlayerStateGlobal::OnStart()
{
    Scene* current = SceneManager::GetSceneWithName("MainScene");
    
    m_pTransform = current->GetComponentType<TransformComponent>(m_pOwner);
        
    m_pCamTransform = current->GetComponentType<TransformComponent>(m_pCam);

    m_pHeadCollider = current->GetComponentType<Collider>(m_pOwner);
    m_pHeadForce = current->GetComponentType<ForceComponent>(m_pOwner);

    m_pFootStepSound = AudioEngine::LoadWav(L"../../res/Audio/footsteps.wav");
    m_pFootstepsAV = AudioEngine::PlaySoundW(m_pFootStepSound, 1.0f, true);
    m_pFootstepsAV->Pause();

    m_pPingSound = AudioEngine::LoadWav(L"../../res/Audio/ping.wav");

    m_pRayJump = new RayCast();
    m_pRayJump->origin = m_pTransform->transform.GetWorldPosition();
    m_pRayJump->maxDist = m_pHeadCollider->scale.y / 2.0f + 0.15f;
    m_pRayJump->dir = -m_pTransform->transform.GetUp();
    m_pRayJump->avoidTag.insert(PLAYER_TAG);

    m_pHit = new HitPoint();
}
    
void PlayerStateGlobal::OnUpdate(float _dt)
{
    HandleInput(_dt);
    HandleMouseMovement(_dt);

    if (m_jumpCooldown > 0.0f)
        m_jumpCooldown -= _dt;
    else if (CanJump())
        isJumping = false;
}

void PlayerStateGlobal::HandleInput(float _dt)
{
    if (InputSystem::IsKeyDown(ESCAPE))
    {
        if (InputSystem::IsMouseCursorLocked())
        {
            InputSystem::UnlockMouseCursor();
            InputSystem::ShowMouseCursor();
        }
        else
        {
            InputSystem::LockMouseCursor();
            InputSystem::HideMouseCursor();
        }
    }

    XMMATRIX rotMatrix = m_pTransform->transform.GetWorldRotMatrix();

    XMVECTOR forward = XMVector3Normalize(
        XMVectorSet(XMVectorGetX(rotMatrix.r[2]), 0.f, XMVectorGetZ(rotMatrix.r[2]), 0.f));
    XMVECTOR right = XMVector3Normalize(
        XMVectorSet(XMVectorGetX(rotMatrix.r[0]), 0.f, XMVectorGetZ(rotMatrix.r[0]), 0.f));

    const float speed = 5.0f;
    XMVECTOR move = XMVectorZero();

    bool isMoving = false;
    if (InputSystem::IsKeyPressed(Z)) { move += forward * speed; isMoving = true; }
    if (InputSystem::IsKeyPressed(S)) { move -= forward * speed; isMoving = true; }
    if (InputSystem::IsKeyPressed(Q)) { move -= right   * speed; isMoving = true; }
    if (InputSystem::IsKeyPressed(D)) { move += right   * speed; isMoving = true; }

    if (isMoving)
    {
        m_footstepTimer -= _dt;
        if (m_footstepTimer <= 0.f)
        {
            float pitch = 0.75f + static_cast<float>(rand()) / RAND_MAX * 0.30f;
            AudioEngine::PlaySoundOnce(m_pFootStepSound, 0.8f, pitch);

            m_footstepTimer = 0.45f;
        } 
    }
    else
    {
        m_footstepTimer = 0.f;
    }
        
    XMFLOAT3 vel = m_pHeadForce->velocity;
    vel.x = XMVectorGetX(move);
    vel.z = XMVectorGetZ(move);
    m_pHeadForce->velocity = vel;
    
    if (InputSystem::IsKeyDown(SPACE) && !isJumping)
    {
        m_pHeadForce->velocity.y = 4.0f;
        isJumping = true;
        m_jumpCooldown = 0.15f;
    }

    if (InputSystem::IsMouseButtonDown(
        InputMouse::MIDDLE_MOUSE))
    {
        Ping();
    }
    
    if (InputSystem::IsKeyDown(InputKeyboard::P))
    {
        std::cout << m_pTransform->transform.GetWorldPosition().z << std::endl;
    }
}

void PlayerStateGlobal::HandleMouseMovement(float _dt)
{
    if (m_pCam == nullptr)
        return;

    if (InputSystem::IsMouseCursorLocked())
    {
        XMINT2 center(EngineManager::GetWindow()->GetWidth() / 2, 
                      EngineManager::GetWindow()->GetHeight() / 2);
        XMINT2 mousePos = InputSystem::GetMousePosition();
        
        constexpr float pitchLimit = 89.0f * (3.14159f / 180.0f);

        m_yawPitch.x += (float)(center.x - mousePos.x) * -m_sensitive;
        m_yawPitch.y += (float)(center.y - mousePos.y) * -m_sensitive / 2;

        m_yawPitch.x *= 3.14f / 180.0f * _dt * 15.0f;
        m_yawPitch.y *= 3.14f / 180.0f * _dt * 15.0f;
        
        XMFLOAT4 rot = m_pCamTransform->transform.GetLocalRotation();

        float sinp = 2.0f * (rot.w * rot.x - rot.z * rot.y);
        float currentPitch = std::asin(std::clamp(sinp, -1.0f, 1.0f));
        
        float newPitch = std::clamp(currentPitch + m_yawPitch.y, -pitchLimit, pitchLimit);
        float actualDelta = newPitch - currentPitch;

        m_pTransform->transform.AddLocalYPR({ m_yawPitch.x, 0.0f, 0.0f });
        m_pCamTransform->transform.AddLocalYPR({ 0.0f, actualDelta, 0.0f });

        InputSystem::SetMousePosition(center);
    }
}

void PlayerStateGlobal::Ping()
{
    std::cout << "Start raycast" << std::endl;

    RayCast ray;
    ray.origin = m_pCamTransform->transform.GetWorldPosition();
    ray.maxDist = 20.0f;
    ray.dir = m_pCamTransform->transform.GetForward();
    ray.avoidTag.insert(PLAYER_TAG);

    HitPoint hp;

    Scene* currentScene = SceneManager::GetCurrentScene();
    currentScene->GetPhysicSystem()->CheckRayCast(ray, hp);
    if (hp.pHitEntity != nullptr)
    {
        std::cout << "hit entity with name : " << hp.pHitEntity->name << " at pos : " << hp.hitPoint.x << ".x " << hp.hitPoint.y << ".y" << hp.hitPoint.z << ".z" << std::endl;
        
        PingTriangle ping;
        ping.Init(currentScene, hp.hitPoint);
        
        Data data;
        data.Init(Cmd::SHOW_PING, { &hp.hitPoint }, { Type::TYPE_XMFLOAT3 });

        XMFLOAT3 pos = { 0.0f,0.0f, 0.0f };

        memcpy(&pos, data.GetByte() + 5, 12);
        

        AudioEngine::Play3D( m_pPingSound, pos.x, pos.y, pos.z, 0.05f );
        EngineManager::GetSocket()->SendTo(data, GameManager::GetOtherPlayerId());
    }
}

bool PlayerStateGlobal::CanJump()
{
    XMFLOAT3 origin = m_pTransform->transform.GetWorldPosition();
    origin.y -= m_pHeadCollider->scale.y / 2.0f;

    m_pRayJump->origin = origin;
    m_pRayJump->dir    = { 0.0f, -1.0f, 0.0f };
    m_pRayJump->maxDist = 0.2f;
    m_pRayJump->pas    = 0.05f;

    m_pHit->pHitEntity = nullptr;
    SceneManager::GetCurrentScene()->GetPhysicSystem()->CheckRayCast(*m_pRayJump, *m_pHit);

    return m_pHit->pHitEntity != nullptr;
}

void PlayerStateGlobal::Reset()
{
    m_score = 0;
    m_pTextScore->SetText(std::to_string(m_score));
    m_yawPitch = { 0.0f, 0.0f };
}

void PlayerStateGlobal::AddScore(int _score)
{
    if (m_score + _score >= 0.0f) m_score += _score;
    else m_score = 0;
    m_pTextScore->SetText(std::to_string(m_score));
}

#endif