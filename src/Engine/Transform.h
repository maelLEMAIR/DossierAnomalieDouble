#ifndef TRANSFORM_H_DEFINED
#define TRANSFORM_H_DEFINED

#include <DirectXMath.h>
#include <type_traits>
using namespace DirectX;

#include "define.h"

enum DIRTY_FLAG : uint32_t
{
    WORLD =             0b00000001,
    INVERSE =           0b00000010,
    LOCAL_POS =         0b00000100,
    WORLD_POS =         0b00001000,
    LOCAL_SCALE =       0b00010000,
    WORLD_SCALE =       0b00100000,
    LOCAL_ROTATE =      0b01000000,
    WORLD_ROTATE =      0b10000000,

    ALL =               0b11111111
};

class Transform
{
public:
    Transform();
    ~Transform() = default;

    Transform(const Transform& other);
    Transform(Transform&& other) noexcept;
    Transform& operator=(const Transform& other);
    Transform& operator=(Transform&& other) noexcept;
    
    XMFLOAT4X4& GetWorldMatrix();
    XMFLOAT4X4& GetInvMatrix();

    void SetIdentity();
    void UpdateWorldMatrix();
    void UpdateInvMatrix();

    uint32_t GetDirty() { return dirty; }
    
    // Pos
    
    XMFLOAT3 GetLocalPosition() { return localPos; };
    XMFLOAT3 GetWorldPosition() { return worldPos; };
    
    void SetLocalPosition(XMFLOAT3 const& position);
    void SetWorldPosition(XMFLOAT3 const& position);
    void MoveLocal(XMFLOAT3 const& delta);
    void MoveLocal(XMFLOAT3 const& dir, float distance);
    void MoveWorld(XMFLOAT3 const& delta);
    void MoveWorld(XMFLOAT3 const& dir, float distance);

    //void UpdateWorldPos();

    // Scale
   
    const XMFLOAT3& GetLocalScale() { return localScale; };
    const XMFLOAT3 GetWorldScale() { return worldScale; };
    
    void SetLocalScale(XMFLOAT3 const& scale);
    void SetLocalScale(float scale);
    void ScaleLocal(XMFLOAT3 const& scale);
    void ScaleLocal(float scale);
    
    void SetWorldScale(XMFLOAT3 const& scale);
    void SetWorldScale(float scale);
    void ScaleWorld(XMFLOAT3 const& scale);
    void ScaleWorld(float scale);

    // Rotate

    const XMFLOAT3& GetForward()    { return mForward; }
    const XMFLOAT3& GetRight()      { return mRight; }
    const XMFLOAT3& GetUp()         { return mUp; }

    XMFLOAT4& GetLocalRotation()   { return localQuat; }
    XMFLOAT4& GetWorldRotation()   { return worldQuat; }

    const XMMATRIX GetWorldRotMatrix()  { return XMLoadFloat4x4(&localRot); }

    void LookAt(XMFLOAT3 const& target);
    
    void OrbitAroundAxis(const XMVECTOR& center, const XMFLOAT3& axis, float angle);
    void OrbitAroundAxis(const XMVECTOR& center, const XMFLOAT4& rotation);
    
    void LookTo(XMFLOAT3 const& dir);
    void LookToCamera(XMFLOAT3 const& dir);
    
    void SetLocalRotationMatrix(XMFLOAT4X4 const& rotation);
    void SetLocalRotationQuaternion(XMFLOAT4 const& quat);
    
    void ResetLocalRotation();
    
    void SetLocalRotation(XMFLOAT3 const& ypr);
    void AddLocalYPR(XMFLOAT3 const& ypr);

    void UpdateLocalRotationFromAxes();
    void UpdateLocalRotationFromQuaternion();
    void UpdateLocalRotationFromMatrix();

    void SetWorldRotationMatrix(XMFLOAT4X4 const& rotation);
    void SetWorldRotationQuaternion(XMFLOAT4 const& _quat);
    
    void UpdateWorldRotationFromAxes();
    void UpdateWorldRotationFromQuaternion();
    void UpdateWorldRotationFromMatrix();

protected:
    XMFLOAT3 localPos;
    XMFLOAT3 localScale = { 1.0f, 1.0f, 1.0f };
    XMFLOAT3 worldPos;
    XMFLOAT3 worldScale = { 1.0f, 1.0f, 1.0f };

    XMFLOAT3 mForward;
    XMFLOAT3 mUp;
    XMFLOAT3 mRight;

    XMFLOAT4 localQuat = { 0.0f, 0.0f, 0.0f, 1.0f };
    XMFLOAT4X4 localRot;
    
    XMFLOAT4 worldQuat = { 0.0f, 0.0f, 0.0f, 1.0f };
    XMFLOAT4X4 worldRot;

    XMFLOAT4X4 worldMatrix;
    XMFLOAT4X4 invMatrix;

    uint32_t dirty = 0;

private:
    friend class TransformSystem;
    friend class ChildrenComponent;
};

#endif
