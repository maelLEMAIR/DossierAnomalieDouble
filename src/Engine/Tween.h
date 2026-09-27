#ifndef TWEEN_H_DEFINED
#define TWEEN_H_DEFINED

#include "define.h"

enum Function
{
    Position,
    Rotation,
    Scale
};

class TransformComponent;
class Transform;

class Tween
{
public:
    void StartDuration(float duration, Function typeFunction, Transform* pOwnerTransform, bool reverse);
    void SetStart(XMFLOAT3 const& pStart) { start = pStart; }
    void SetEnd(XMFLOAT3 const& pEnd)     { end = pEnd; }
    XMFLOAT3 GetStart() const { return start; }
    XMFLOAT3 GetEnd()   const { return end; }
    
    void StartLoop(float duration, Function typeFunction, Transform* pOwnerTransform);
    void StopLoop();
    void ResumeLoop();
    void Restart();

private:
    Tween(XMFLOAT3 const& start, XMFLOAT3 const& end, float(* const pFunction)(float));
    void Update(float const deltaTime);
    XMFLOAT3 start;
    XMFLOAT3 end;
    float(* function)(float)  = nullptr;

    struct infoTween
    {
        float mDuration;
        float mElapsed = 0.f;
        Function mTypeFunction;
        Transform* mOwnerTransform;
        bool mReverse = true;
        bool m_pause = false;
        bool mLoop = false;
    };
    infoTween* mTween;
    
    friend class TweenSystem;
};

class TweenSystem
{

public:
    static Tween* Create(XMFLOAT3 const& pStart, XMFLOAT3 const& pEnd, float(* const pFunction)(float) );
    static void Update(float deltaTime);

private:
    inline static Vector<Tween*> tweens;
};

struct Interpolation 
{
public:
    static float easingIn_linear(float x) { return x; }
    static float easingOut_linear(float x) { return x; }
    static float easingInAndOut_linear(float x) { return x; }
    
    static float easingIn_Quad(float const x) { return x * x; }
    static float easingOut_Quad(float const x) { return 1.0f - (1.0f - x) * (1.0f - x); }
    static float easingInOut_Quad(float const x) { return x < 0.5f ? 2.0f * x * x : (4.0f - 2.0f * x) * x - 1.0f; }
    
    static float easingIn_cubic(float x) { return x * x * x; }
    static float easingOut_cubic(float x) { return x * x * x; }
    static float easingInAndOut_cubic(float x) { return x * x * x * x; }

    static float easingIn_quad(float x) { return x * x * x * x; }
    static float easingOut_quad(float x) { return x * x * x * x; }
    static float easingInAndOut_quad(float x) { return x * x * x * x; }
};

#endif