#ifndef TWEEN_CPP_DEFINED
#define TWEEN_CPP_DEFINED

#include "Tween.h"
#include "Components/TransformComponent.h"

Tween::Tween(XMFLOAT3 const&  pStart, XMFLOAT3 const& pEnd, float(* const pFunction)(float))
{
    start = pStart;
    end = pEnd;
    function = pFunction;
}

void Tween::StartDuration(float duration, Function typeFunction, Transform* pOwnerTransform, bool reverse)
{
    mTween = new infoTween();
    mTween->mDuration = duration;
    mTween->mTypeFunction = typeFunction;
    mTween->mOwnerTransform = pOwnerTransform;
    mTween->mReverse = reverse;
}

void Tween::StartLoop(float duration, Function typeFunction, Transform* pOwnerTransform)
{
    mTween = new infoTween();
    mTween->mDuration = duration;
    mTween->mTypeFunction = typeFunction;
    mTween->mOwnerTransform = pOwnerTransform;
    mTween->mLoop = true;
}

void Tween::StopLoop()
{
    mTween->m_pause = true;
}

void Tween::ResumeLoop()
{
    mTween->m_pause = false;
}

void Tween::Update(float const deltaTime)
{
    if (mTween == nullptr) return;

    if ( mTween->m_pause) return;
    
    float const dt = deltaTime;
    mTween->mElapsed += dt;
    float const progress = mTween->mReverse ? 1.0f - mTween->mElapsed / mTween->mDuration : mTween->mElapsed / mTween->mDuration;
    switch (mTween->mTypeFunction)
    {
    case Function::Position:
    {
        mTween->mOwnerTransform->SetLocalPosition(start + (end - start) * function(progress <= 1.0f ? progress : 1.0f));
        break;
    }
    case Function::Rotation:
    {
        mTween->mOwnerTransform->SetLocalRotation(start + (end - start) * function(progress <= 1.0f ? progress : 1.0f));
        break;
    }
    case Function::Scale:
    {
        mTween->mOwnerTransform->SetWorldScale(start + (end - start) * function(progress <= 1.0f ? progress : 1.0f));
        break;
    }
    };
    if (mTween->mElapsed >= mTween->mDuration) 
    {
        if (mTween->mLoop == false)
            mTween = nullptr;
        else
        {
            mTween->mReverse = !mTween->mReverse;
            mTween->mElapsed = 0.0f;
        }
    }
}    

void Tween::Restart()
{
    mTween->mElapsed = 0.0f;    
}

Tween* TweenSystem::Create(XMFLOAT3 const& start, XMFLOAT3 const& end, float(* const pFunction)(float))
{
    Tween* tween = new Tween(start,end,pFunction);
    tweens.push_back(tween);
    return tween;
}

void TweenSystem::Update(float const deltaTime)
{
    for (Tween* t : tweens) t->Update(deltaTime);
}

#endif