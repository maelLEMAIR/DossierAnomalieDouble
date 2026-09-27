#ifndef TRANSFORM_CPP_DEFINED
#define TRANSFORM_CPP_DEFINED

#include "Transform.h"


Transform::Transform()
{
    dirty = 0;
    SetIdentity();
    ResetLocalRotation();
}

Transform::Transform(const Transform& other)
    :     localPos(other.localPos),
          worldPos(other.worldPos),
          localScale(other.localScale),
          worldScale(other.worldScale),
          mForward(other.mForward),
          mUp(other.mUp),
          mRight(other.mRight),
          localQuat(other.localQuat),
          worldQuat(other.worldQuat),
          localRot(other.localRot),
          worldRot(other.worldRot),
          worldMatrix(other.worldMatrix),
          invMatrix(other.invMatrix),
          dirty(other.dirty)
{
}

Transform::Transform(Transform&& other) noexcept
    : localPos(std::move(other.localPos)),
      localScale(std::move(other.localScale)),
      mForward(std::move(other.mForward)),
      mUp(std::move(other.mUp)),
      mRight(std::move(other.mRight)),
      localQuat(std::move(other.localQuat)),
      localRot(std::move(other.localRot)),
      worldMatrix(std::move(other.worldMatrix)),
      invMatrix(std::move(other.invMatrix)),
      dirty(other.dirty)
{
}

Transform& Transform::operator=(const Transform& other)
{
    if (this == &other)
        return *this;
    localPos = other.localPos;
    worldPos = other.worldPos;
    localScale = other.localScale;
    worldScale = other.worldScale;
    mForward = other.mForward;
    mUp = other.mUp;
    mRight = other.mRight;
    localQuat = other.localQuat;
    worldQuat = other.worldQuat;
    localRot = other.localRot;
    worldRot = other.worldRot;
    worldMatrix = other.worldMatrix;
    invMatrix = other.invMatrix;
    dirty = other.dirty;
    return *this;
}

Transform& Transform::operator=(Transform&& other) noexcept
{
    if (this == &other)
        return *this;
    localPos = std::move(other.localPos);
    localScale = std::move(other.localScale);
    mForward = std::move(other.mForward);
    mUp = std::move(other.mUp);
    mRight = std::move(other.mRight);
    localQuat = std::move(other.localQuat);
    localRot = std::move(other.localRot);
    worldMatrix = std::move(other.worldMatrix);
    invMatrix = std::move(other.invMatrix);
    dirty = other.dirty;
    return *this;
}

XMFLOAT4X4& Transform::GetWorldMatrix()
{
    if (dirty & WORLD)
        UpdateWorldMatrix();
    
    return worldMatrix;
}

XMFLOAT4X4& Transform::GetInvMatrix()
{
    if (dirty & INVERSE)
        UpdateInvMatrix();
    
    return invMatrix;
}

void Transform::SetIdentity()
{
    localPos    = XMFLOAT3(0.0f, 0.0f, 0.0f);
    localScale  = XMFLOAT3(1.0f, 1.0f, 1.0f);
    worldPos    = XMFLOAT3(0.0f, 0.0f, 0.0f);
    worldScale  = XMFLOAT3(1.0f, 1.0f, 1.0f);
    XMStoreFloat4x4(&worldMatrix, XMMatrixIdentity());
    ResetLocalRotation();
}

void Transform::UpdateWorldMatrix()
{
    /*XMVECTOR p = XMLoadFloat3(&worldPos);
    XMVECTOR s = XMLoadFloat3(&worldScale);

    XMVECTOR sx = XMVectorSplatX(s);
    XMVECTOR sy = XMVectorSplatY(s);
    XMVECTOR sz = XMVectorSplatZ(s);

    XMMATRIX m = XMLoadFloat4x4(&worldMatrix);
    m.r[0] = XMVectorMultiply(m.r[0], sx);
    m.r[1] = XMVectorMultiply(m.r[1], sy);
    m.r[2] = XMVectorMultiply(m.r[2], sz);
    m.r[3] = XMVectorSetW(p, 1.0f);

    XMStoreFloat4x4(&worldMatrix, m);

    dirty &= ~WORLD;*/

    XMVECTOR p = XMLoadFloat3(&worldPos);
    XMVECTOR s = XMLoadFloat3(&worldScale);
    XMVECTOR r = XMLoadFloat4(&worldQuat);

    XMMATRIX m = XMMatrixAffineTransformation(s, XMVectorZero(), r, p);
    XMStoreFloat4x4(&worldMatrix, m);

    dirty &= ~WORLD;
}

void Transform::UpdateInvMatrix()
{
    if (dirty & WORLD)
        UpdateWorldMatrix();
    
    XMMATRIX m = XMLoadFloat4x4(&worldMatrix);
    XMStoreFloat4x4(&invMatrix, XMMatrixInverse(nullptr, m));

    dirty &= ~INVERSE;
}


///////////////////////////////////////////////////////////////////////////////////////
/// POSITION ///
///////////////////////////////////////////////////////////////////////////////////////

void Transform::SetLocalPosition(XMFLOAT3 const& position)
{
    localPos = position;
    dirty |= LOCAL_POS;
}

void Transform::SetWorldPosition(XMFLOAT3 const& position)
{
    worldPos = position;
    dirty |= WORLD_POS;
}

void Transform::MoveLocal(XMFLOAT3 const& delta)
{
    XMVECTOR p = XMLoadFloat3(&localPos);

    XMVECTOR offset = XMVectorZero();
    offset = XMVectorAdd(offset, XMVectorScale(XMLoadFloat3(&mRight),   delta.x));
    offset = XMVectorAdd(offset, XMVectorScale(XMLoadFloat3(&mUp),      delta.y));
    offset = XMVectorAdd(offset, XMVectorScale(XMLoadFloat3(&mForward), delta.z));

    p = XMVectorAdd(p, offset);
    XMStoreFloat3(&localPos, p);
    
    dirty |= LOCAL_POS;
}

void Transform::MoveLocal(XMFLOAT3 const& dir, float distance)
{
    XMVECTOR p      = XMLoadFloat3(&localPos);
    XMVECTOR d      = XMLoadFloat3(&dir);
    XMVECTOR scalar = XMVectorReplicate(distance);

    p = XMVectorAdd(p, XMVectorMultiply(d, scalar));
    XMStoreFloat3(&localPos, p);

    dirty |= LOCAL_POS;
}

void Transform::MoveWorld(XMFLOAT3 const& delta)
{
    XMVECTOR p = XMLoadFloat3(&worldPos);
    p = XMVectorAdd(p, XMLoadFloat3(&delta));
    XMStoreFloat3(&worldPos, p);
    dirty |= WORLD_POS;

    MoveLocal(delta);
}

void Transform::MoveWorld(XMFLOAT3 const& dir, float distance)
{
    XMVECTOR p      = XMLoadFloat3(&worldPos);
    XMVECTOR d      = XMLoadFloat3(&dir);
    XMVECTOR scalar = XMVectorReplicate(distance);

    p = XMVectorAdd(p, XMVectorMultiply(d, scalar));
    XMStoreFloat3(&worldPos, p);
    dirty |= WORLD_POS;

    MoveLocal(dir, distance);
}


///////////////////////////////////////////////////////////////////////////////////////
/// SCALE ///
///////////////////////////////////////////////////////////////////////////////////////

void Transform::SetLocalScale(XMFLOAT3 const& _scale)
{
    localScale = _scale;
    worldScale = _scale;
    dirty |= LOCAL_SCALE;
}

void Transform::SetLocalScale(float _scale)
{
    XMStoreFloat3(&localScale, XMVectorReplicate(_scale));
    worldScale = localScale;
    dirty |= LOCAL_SCALE;
}

void Transform::ScaleLocal(XMFLOAT3 const& _scale)
{
    XMVECTOR s = XMVectorMultiply(XMLoadFloat3(&localScale), XMLoadFloat3(&_scale));
    XMStoreFloat3(&localScale, s);
    worldScale = localScale;
    dirty |= LOCAL_SCALE;
}

void Transform::ScaleLocal(float _scale)
{
    XMVECTOR s = XMVectorMultiply(XMLoadFloat3(&localScale), XMVectorReplicate(_scale));
    XMStoreFloat3(&localScale, s);
    worldScale = localScale;
    dirty |= LOCAL_SCALE;
}

void Transform::SetWorldScale(XMFLOAT3 const& _scale)
{
    worldScale = _scale;
    dirty |= WORLD_SCALE;
}

void Transform::SetWorldScale(float _scale)
{
    XMStoreFloat3(&worldScale, XMVectorReplicate(_scale));
    dirty |= WORLD_SCALE;
}

void Transform::ScaleWorld(XMFLOAT3 const& _scale)
{
    XMVECTOR s = XMVectorMultiply(XMLoadFloat3(&worldScale), XMLoadFloat3(&_scale));
    XMStoreFloat3(&worldScale, s);
    dirty |= WORLD_SCALE;
}

void Transform::ScaleWorld(float _scale)
{
    XMVECTOR s = XMVectorMultiply(XMLoadFloat3(&worldScale), XMVectorReplicate(_scale));
    XMStoreFloat3(&worldScale, s);
    dirty |= WORLD_SCALE;
}


///////////////////////////////////////////////////////////////////////////////////////
/// ROTATION ///
///////////////////////////////////////////////////////////////////////////////////////

static void ExtractAxesFromMatrix(const XMFLOAT4X4& m,
                                  XMFLOAT3& right,
                                  XMFLOAT3& up,
                                  XMFLOAT3& forward)
{
    right   = XMFLOAT3(m._11, m._12, m._13);
    up      = XMFLOAT3(m._21, m._22, m._23);
    forward = XMFLOAT3(m._31, m._32, m._33);
}


void Transform::LookTo(XMFLOAT3 const& dir)
{
    XMVECTOR forward = XMVector3Normalize(XMLoadFloat3(&dir));
    XMVECTOR up      = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    
    XMVECTOR right  = XMVector3Normalize(XMVector3Cross(up, forward));
    XMVECTOR newUp  = XMVector3Cross(forward, right);
    
    XMMATRIX rotationMatrix;
    rotationMatrix.r[0] = XMVectorSetW(right,   0.0f);
    rotationMatrix.r[1] = XMVectorSetW(newUp,   0.0f);
    rotationMatrix.r[2] = XMVectorSetW(forward, 0.0f);
    rotationMatrix.r[3] = XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
    
    XMVECTOR quat = XMQuaternionRotationMatrix(rotationMatrix);
    
    XMStoreFloat4(&worldQuat, quat);
    XMStoreFloat4(&localQuat, quat);
    
    XMVECTOR scaleVec = XMLoadFloat3(&worldScale);
    XMVECTOR posVec   = XMLoadFloat3(&worldPos);
    XMMATRIX worldMat = XMMatrixAffineTransformation(scaleVec, XMVectorZero(), quat, posVec);
    XMStoreFloat4x4(&worldMatrix, worldMat);
    
    XMStoreFloat4x4(&worldRot, rotationMatrix);
    XMStoreFloat4x4(&localRot, rotationMatrix);
    
    ExtractAxesFromMatrix(worldRot, mRight, mUp, mForward);/*
    ExtractAxesFromMatrix(worldRot, worldRight, worldUp, worldForward);*/
    
    dirty |= WORLD_ROTATE;
}

void Transform::LookToCamera(XMFLOAT3 const& dir)
{
    XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
    XMMATRIX m  = XMMatrixLookToLH(XMLoadFloat3(&worldPos), XMLoadFloat3(&dir), up);
    XMStoreFloat4x4(&worldMatrix, m);
    UpdateLocalRotationFromMatrix();

    dirty |= WORLD_ROTATE;
}

void Transform::LookAt(XMFLOAT3 const& target)
{
    XMVECTOR pos = XMLoadFloat3(&worldPos);
    XMVECTOR tgt = XMLoadFloat3(&target);
    XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

    if (XMVector3Equal(pos, tgt))
        return;

    XMVECTOR dir = XMVector3Normalize(XMVectorSubtract(tgt, pos));
    if (XMVector3NearEqual(XMVectorAbs(dir), up, XMVectorReplicate(0.0001f)))
        up = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

    XMMATRIX view = XMMatrixLookAtLH(pos, tgt, up);
    XMMATRIX world = XMMatrixInverse(nullptr, view);

    XMStoreFloat4x4(&worldMatrix, world);
    UpdateLocalRotationFromMatrix();
    dirty |= WORLD_ROTATE;
}

void Transform::OrbitAroundAxis(const XMVECTOR& center, const XMFLOAT3& axis, float angle)
{
    XMVECTOR nAxis = XMVector3Normalize(XMLoadFloat3(&axis));
    
    XMVECTOR currentPos = XMLoadFloat3(&worldPos);
    XMVECTOR toObject = XMVectorSubtract(currentPos, center);
    
    XMMATRIX rotMatrix = XMMatrixRotationAxis(nAxis, angle);
    XMVECTOR rotated = XMVector3TransformNormal(toObject, rotMatrix);
    
    XMVECTOR newPos = XMVectorAdd(center, rotated);
    XMStoreFloat3(&worldPos, newPos);
    
    XMVECTOR deltaQuat = XMQuaternionRotationAxis(nAxis, angle);
    XMVECTOR currentQuat = XMLoadFloat4(&worldQuat);
    XMVECTOR newQuat = XMQuaternionNormalize(XMQuaternionMultiply(deltaQuat, currentQuat));
    XMStoreFloat4(&worldQuat, newQuat);
    XMStoreFloat4(&localQuat, newQuat);
    
    XMStoreFloat4x4(&worldRot, XMMatrixRotationQuaternion(newQuat));
    ExtractAxesFromMatrix(worldRot, mRight, mUp, mForward);
    
    dirty |= WORLD | WORLD_POS | WORLD_ROTATE;
}

void Transform::OrbitAroundAxis(const XMVECTOR& center, const XMFLOAT4& rotation)
{
    XMVECTOR rotQuat = XMLoadFloat4(&rotation);
    rotQuat = XMQuaternionNormalize(rotQuat);

    XMVECTOR currentPos = XMLoadFloat3(&worldPos);
    XMVECTOR toObject = XMVectorSubtract(currentPos, center);

    XMMATRIX rotMatrix = XMMatrixRotationQuaternion(rotQuat);
    XMVECTOR rotated = XMVector3TransformNormal(toObject, rotMatrix);

    XMVECTOR newPos = XMVectorAdd(center, rotated);
    XMStoreFloat3(&worldPos, newPos);

    XMVECTOR currentQuat = XMLoadFloat4(&worldQuat);
    XMVECTOR newQuat = XMQuaternionNormalize(XMQuaternionMultiply(rotQuat, currentQuat));
    XMStoreFloat4(&worldQuat, newQuat);
    XMStoreFloat4(&localQuat, newQuat);

    XMStoreFloat4x4(&worldRot, XMMatrixRotationQuaternion(newQuat));
    ExtractAxesFromMatrix(worldRot, mRight, mUp, mForward);

    dirty |= WORLD | WORLD_POS | WORLD_ROTATE;
}

void Transform::SetLocalRotationMatrix(XMFLOAT4X4 const& rotation)
{
    localRot = rotation;
    UpdateLocalRotationFromMatrix();

    dirty |= LOCAL_ROTATE;
}

void Transform::SetLocalRotationQuaternion(XMFLOAT4 const& _quat)
{
    localQuat = _quat;
    UpdateLocalRotationFromQuaternion();

    dirty |= LOCAL_ROTATE;
}

void Transform::ResetLocalRotation()
{
    mRight   = XMFLOAT3(1.0f, 0.0f, 0.0f);
    mUp      = XMFLOAT3(0.0f, 1.0f, 0.0f);
    mForward = XMFLOAT3(0.0f, 0.0f, 1.0f);

    XMStoreFloat4x4(&localRot,  XMMatrixIdentity());
    XMStoreFloat4  (&localQuat, XMQuaternionIdentity());

    dirty |= LOCAL_ROTATE;
}

void Transform::SetLocalRotation(XMFLOAT3 const& ypr)
{
    ResetLocalRotation();
    AddLocalYPR(ypr);
}

void Transform::AddLocalYPR(const XMFLOAT3& ypr)
{
    XMVECTOR qRot = XMLoadFloat4(&localQuat);

    if (ypr.x != 0.0f) // yaw (Y world axis)
    {
        XMVECTOR qYaw = XMQuaternionRotationAxis(XMVectorSet(0,1,0,0), ypr.x);
        qRot = XMQuaternionMultiply(qRot, qYaw);
    }

    if (ypr.y != 0.0f) // pitch (local X axis)
    {
        XMVECTOR right  = XMVector3Rotate(XMVectorSet(1,0,0,0), qRot);
        XMVECTOR qPitch = XMQuaternionRotationAxis(right, ypr.y);
        qRot = XMQuaternionMultiply(qPitch, qRot);
    }

    if (ypr.z != 0.0f) // roll (local Z axis)
    {
        XMVECTOR qRoll = XMQuaternionRotationAxis(XMVectorSet(0,0,1,0), ypr.z);
        qRot = XMQuaternionMultiply(qRot, qRoll);
    }

    qRot = XMQuaternionNormalize(qRot);
    XMStoreFloat4(&localQuat, qRot);

    UpdateLocalRotationFromQuaternion();
}


void Transform::UpdateLocalRotationFromAxes()
{
    localRot._11 = mRight.x;   localRot._12 = mRight.y;   localRot._13 = mRight.z;
    localRot._21 = mUp.x;      localRot._22 = mUp.y;      localRot._23 = mUp.z;
    localRot._31 = mForward.x; localRot._32 = mForward.y; localRot._33 = mForward.z;
    localRot._14 = 0.0f; localRot._24 = 0.0f; localRot._34 = 0.0f;
    localRot._41 = 0.0f; localRot._42 = 0.0f; localRot._43 = 0.0f; localRot._44 = 1.0f;

    XMStoreFloat4(&localQuat, XMQuaternionRotationMatrix(XMLoadFloat4x4(&localRot)));

    dirty |= LOCAL_ROTATE;
}

void Transform::UpdateLocalRotationFromQuaternion()
{
    XMVECTOR q = XMLoadFloat4(&localQuat);
    
    XMStoreFloat4x4(&localRot, XMMatrixRotationQuaternion(q));
    XMStoreFloat4x4(&worldRot, XMMatrixRotationQuaternion(q));
    
    // CRUCIAL - sans ça, UpdateWorldMatrix() utilise l'ancien worldQuat
    XMStoreFloat4(&worldQuat, q);
    
    //ExtractAxesFromMatrix(worldRot, mRight, mUp, mForward);

    dirty |= LOCAL_ROTATE;
}

void Transform::UpdateLocalRotationFromMatrix()
{
    XMMATRIX worldMat = XMLoadFloat4x4(&worldMatrix);
    
    XMVECTOR scale, rotQuat, trans;
    XMMatrixDecompose(&scale, &rotQuat, &trans, worldMat);
    
    XMStoreFloat4(&worldQuat, rotQuat);
    XMStoreFloat4(&localQuat, rotQuat);
    XMStoreFloat4x4(&localRot, XMMatrixRotationQuaternion(rotQuat));

    worldRot = localRot;

    ExtractAxesFromMatrix(worldRot, mRight, mUp, mForward);

    /*
    ExtractAxesFromMatrix(worldRot, worldRight, worldUp, worldForward);*/

    dirty |= LOCAL_ROTATE;
}

void Transform::SetWorldRotationMatrix(XMFLOAT4X4 const& rotation)
{
    worldRot = rotation;
    UpdateWorldRotationFromMatrix();

    dirty |= WORLD_ROTATE;
}

void Transform::SetWorldRotationQuaternion(XMFLOAT4 const& _quat)
{
    worldQuat = _quat;
    UpdateWorldRotationFromQuaternion();

    dirty |= WORLD_ROTATE;
}

void Transform::UpdateWorldRotationFromAxes()
{
    worldRot._11 = mRight.x;   worldRot._12 = mRight.y;   worldRot._13 = mRight.z;
    worldRot._21 = mUp.x;      worldRot._22 = mUp.y;      worldRot._23 = mUp.z;
    worldRot._31 = mForward.x; worldRot._32 = mForward.y; worldRot._33 = mForward.z;
    worldRot._14 = 0.0f; worldRot._24 = 0.0f; worldRot._34 = 0.0f;
    worldRot._41 = 0.0f; worldRot._42 = 0.0f; worldRot._43 = 0.0f; worldRot._44 = 1.0f;

    XMStoreFloat4(&worldQuat, XMQuaternionRotationMatrix(XMLoadFloat4x4(&worldRot)));

    dirty |= WORLD_ROTATE;
}

void Transform::UpdateWorldRotationFromQuaternion()
{
    XMStoreFloat4x4(&worldRot, XMMatrixRotationQuaternion(XMLoadFloat4(&worldQuat)));
    ExtractAxesFromMatrix(worldRot, mRight, mUp, mForward);

    dirty |= WORLD_ROTATE;
}

void Transform::UpdateWorldRotationFromMatrix()
{
    XMStoreFloat4(&worldQuat, XMQuaternionRotationMatrix(XMLoadFloat4x4(&worldRot)));

    ExtractAxesFromMatrix(worldRot, mRight, mUp, mForward);

    dirty |= WORLD_ROTATE;
}

///////////////////////////////////////////////////////////////////////////////////////

#endif