#ifndef MATH_UTILS_HPP_INCLUDED
#define MATH_UTILS_HPP_INCLUDED

/*
 * MathUtils.hpp
 * Utilitaires mathématiques pour DirectX.
 * Toutes les fonctions travaillent en XMVECTOR en interne (SIMD)
 * et exposent des XMFLOAT2 / XMFLOAT3 / XMFLOAT4 en entrée/sortie.
 *
 * Requiert : DirectXMath (Windows SDK ou vcpkg directxmath)
 */
#include "define.h"
 
using namespace DirectX;
 
namespace MathUtils
{
    // =========================================================================
    // CONSTANTES
    // =========================================================================
 
    constexpr float PI         = 3.14159265358979323846f;
    constexpr float TWO_PI     = 6.28318530717958647692f;
    constexpr float HALF_PI    = 1.57079632679489661923f;
    constexpr float DEG_TO_RAD = PI / 180.0f;
    constexpr float RAD_TO_DEG = 180.0f / PI;
    constexpr float EPSILON    = 1e-6f;
 
 
    // =========================================================================
    // HELPERS LOAD / STORE  (usage interne)
    // =========================================================================
 
    // XMFLOAT2
    inline XMVECTOR Load(const XMFLOAT2& v)  { return XMLoadFloat2(&v); }
    inline XMFLOAT2 Store2(FXMVECTOR v)       { XMFLOAT2 r; XMStoreFloat2(&r, v); return r; }
 
    // XMFLOAT3
    inline XMVECTOR Load(const XMFLOAT3& v)  { return XMLoadFloat3(&v); }
    inline XMFLOAT3 Store3(FXMVECTOR v)       { XMFLOAT3 r; XMStoreFloat3(&r, v); return r; }
 
    // XMFLOAT4
    inline XMVECTOR Load(const XMFLOAT4& v)  { return XMLoadFloat4(&v); }
    inline XMFLOAT4 Store4(FXMVECTOR v)       { XMFLOAT4 r; XMStoreFloat4(&r, v); return r; }
 
 
    // =========================================================================
    // CONVERSIONS ANGLES
    // =========================================================================
 
    inline float ToRadians(float deg) { return XMConvertToRadians(deg); }
    inline float ToDegrees(float rad) { return XMConvertToDegrees(rad); }
 
    inline XMFLOAT3 ToRadians(const XMFLOAT3& deg)
    {
        return Store3(XMVectorScale(Load(deg), DEG_TO_RAD));
    }
 
    inline XMFLOAT3 ToDegrees(const XMFLOAT3& rad)
    {
        return Store3(XMVectorScale(Load(rad), RAD_TO_DEG));
    }
 
 
    // =========================================================================
    // SCALAIRE
    // =========================================================================
 
    inline float Clamp(float v, float lo, float hi)   { return max(lo, min(v, hi)); }
    inline float Lerp(float a, float b, float t)      { return a + t * (b - a); }
    inline float SmoothStep(float a, float b, float t)
    {
        t = Clamp((t - a) / (b - a), 0.f, 1.f);
        return t * t * (3.f - 2.f * t);
    }
    inline float InverseLerp(float a, float b, float v)
    {
        return (abs(b - a) > EPSILON) ? (v - a) / (b - a) : 0.f;
    }
    inline bool NearlyEqual(float a, float b, float eps = EPSILON)
    {
        return abs(a - b) < eps;
    }
 
 
    // =========================================================================
    // XMFLOAT2
    // =========================================================================
 
    inline XMFLOAT2 Add(const XMFLOAT2& a, const XMFLOAT2& b)
    {
        return Store2(XMVectorAdd(Load(a), Load(b)));
    }
    inline XMFLOAT2 Sub(const XMFLOAT2& a, const XMFLOAT2& b)
    {
        return Store2(XMVectorSubtract(Load(a), Load(b)));
    }
    inline XMFLOAT2 Scale(const XMFLOAT2& v, float s)
    {
        return Store2(XMVectorScale(Load(v), s));
    }
    inline XMFLOAT2 Negate(const XMFLOAT2& v)
    {
        return Store2(XMVectorNegate(Load(v)));
    }
    inline float Dot(const XMFLOAT2& a, const XMFLOAT2& b)
    {
        return XMVectorGetX(XMVector2Dot(Load(a), Load(b)));
    }
    // Produit croisé 2D (scalaire Z)
    inline float Cross(const XMFLOAT2& a, const XMFLOAT2& b)
    {
        return XMVectorGetX(XMVector2Cross(Load(a), Load(b)));
    }
    inline float Length(const XMFLOAT2& v)
    {
        return XMVectorGetX(XMVector2Length(Load(v)));
    }
    inline float LengthSq(const XMFLOAT2& v)
    {
        return XMVectorGetX(XMVector2LengthSq(Load(v)));
    }
    inline XMFLOAT2 Normalize(const XMFLOAT2& v)
    {
        return Store2(XMVector2Normalize(Load(v)));
    }
    inline float Distance(const XMFLOAT2& a, const XMFLOAT2& b)
    {
        return XMVectorGetX(XMVector2Length(XMVectorSubtract(Load(b), Load(a))));
    }
    inline float DistanceSq(const XMFLOAT2& a, const XMFLOAT2& b)
    {
        return XMVectorGetX(XMVector2LengthSq(XMVectorSubtract(Load(b), Load(a))));
    }
    inline XMFLOAT2 Lerp(const XMFLOAT2& a, const XMFLOAT2& b, float t)
    {
        return Store2(XMVectorLerp(Load(a), Load(b), t));
    }
    inline XMFLOAT2 Clamp(const XMFLOAT2& v, const XMFLOAT2& lo, const XMFLOAT2& hi)
    {
        return Store2(XMVectorClamp(Load(v), Load(lo), Load(hi)));
    }
    inline XMFLOAT2 Reflect(const XMFLOAT2& incident, const XMFLOAT2& normal)
    {
        return Store2(XMVector2Reflect(Load(incident), Load(normal)));
    }
    inline XMFLOAT2 Refract(const XMFLOAT2& incident, const XMFLOAT2& normal, float eta)
    {
        return Store2(XMVector2Refract(Load(incident), Load(normal), eta));
    }
    // Perpendiculaire (rotation 90° anti-horaire)
    inline XMFLOAT2 Perpendicular(const XMFLOAT2& v)
    {
        return Store2(XMVector2Orthogonal(Load(v)));
    }
    inline float Angle(const XMFLOAT2& v)
    {
        return atan2(v.y, v.x);
    }
    inline float AngleBetween(const XMFLOAT2& a, const XMFLOAT2& b)
    {
        return XMVectorGetX(XMVector2AngleBetweenNormals(
            XMVector2Normalize(Load(a)),
            XMVector2Normalize(Load(b))));
    }
    inline XMFLOAT2 FromAngle(float radians)
    {
        return XMFLOAT2(cos(radians), sin(radians));
    }
 
 
    // =========================================================================
    // XMFLOAT3
    // =========================================================================
 
    inline XMFLOAT3 Add(const XMFLOAT3& a, const XMFLOAT3& b)
    {
        return Store3(XMVectorAdd(Load(a), Load(b)));
    }
    inline XMFLOAT3 Sub(const XMFLOAT3& a, const XMFLOAT3& b)
    {
        return Store3(XMVectorSubtract(Load(a), Load(b)));
    }
    inline XMFLOAT3 Scale(const XMFLOAT3& v, float s)
    {
        return Store3(XMVectorScale(Load(v), s));
    }
    inline XMFLOAT3 Mul(const XMFLOAT3& a, const XMFLOAT3& b)
    {
        return Store3(XMVectorMultiply(Load(a), Load(b)));
    }
    inline XMFLOAT3 Negate(const XMFLOAT3& v)
    {
        return Store3(XMVectorNegate(Load(v)));
    }
    inline float Dot(const XMFLOAT3& a, const XMFLOAT3& b)
    {
        return XMVectorGetX(XMVector3Dot(Load(a), Load(b)));
    }
    inline XMFLOAT3 Cross(const XMFLOAT3& a, const XMFLOAT3& b)
    {
        return Store3(XMVector3Cross(Load(a), Load(b)));
    }
    inline float Length(const XMFLOAT3& v)
    {
        return XMVectorGetX(XMVector3Length(Load(v)));
    }
    inline float LengthSq(const XMFLOAT3& v)
    {
        return XMVectorGetX(XMVector3LengthSq(Load(v)));
    }
    inline XMFLOAT3 Normalize(const XMFLOAT3& v)
    {
        return Store3(XMVector3Normalize(Load(v)));
    }
    inline float Distance(const XMFLOAT3& a, const XMFLOAT3& b)
    {
        return XMVectorGetX(XMVector3Length(XMVectorSubtract(Load(b), Load(a))));
    }
    inline float DistanceSq(const XMFLOAT3& a, const XMFLOAT3& b)
    {
        return XMVectorGetX(XMVector3LengthSq(XMVectorSubtract(Load(b), Load(a))));
    }
    inline XMFLOAT3 Lerp(const XMFLOAT3& a, const XMFLOAT3& b, float t)
    {
        return Store3(XMVectorLerp(Load(a), Load(b), t));
    }
    inline XMFLOAT3 Clamp(const XMFLOAT3& v, const XMFLOAT3& lo, const XMFLOAT3& hi)
    {
        return Store3(XMVectorClamp(Load(v), Load(lo), Load(hi)));
    }
    inline XMFLOAT3 Reflect(const XMFLOAT3& incident, const XMFLOAT3& normal)
    {
        return Store3(XMVector3Reflect(Load(incident), Load(normal)));
    }
    inline XMFLOAT3 Refract(const XMFLOAT3& incident, const XMFLOAT3& normal, float eta)
    {
        return Store3(XMVector3Refract(Load(incident), Load(normal), eta));
    }
    inline float AngleBetween(const XMFLOAT3& a, const XMFLOAT3& b)
    {
        return XMVectorGetX(XMVector3AngleBetweenNormals(
            XMVector3Normalize(Load(a)),
            XMVector3Normalize(Load(b))));
    }
    // Projection de v sur dir
    inline XMFLOAT3 Project(const XMFLOAT3& v, const XMFLOAT3& dir)
    {
        XMVECTOR vDir = Load(dir);
        float d = XMVectorGetX(XMVector3Dot(Load(v), vDir))
                / XMVectorGetX(XMVector3LengthSq(vDir));
        return Store3(XMVectorScale(vDir, d));
    }
    // Composante de v perpendiculaire à dir
    inline XMFLOAT3 Reject(const XMFLOAT3& v, const XMFLOAT3& dir)
    {
        return Store3(XMVectorSubtract(Load(v), Load(Project(v, dir))));
    }
    // Interpolation sphérique
    inline XMFLOAT3 Slerp(const XMFLOAT3& a, const XMFLOAT3& b, float t)
    {
        XMVECTOR na = XMVector3Normalize(Load(a));
        XMVECTOR nb = XMVector3Normalize(Load(b));
        float dot = Clamp(XMVectorGetX(XMVector3Dot(na, nb)), -1.f, 1.f);
        float theta = acos(dot) * t;
        XMVECTOR rel = XMVector3Normalize(XMVectorSubtract(nb, XMVectorScale(na, dot)));
        return Store3(XMVectorAdd(
            XMVectorScale(na, cos(theta)),
            XMVectorScale(rel, sin(theta))));
    }
    // Normale d'une face (sens anti-horaire)
    inline XMFLOAT3 FaceNormal(const XMFLOAT3& p0, const XMFLOAT3& p1, const XMFLOAT3& p2)
    {
        XMVECTOR e1 = XMVectorSubtract(Load(p1), Load(p0));
        XMVECTOR e2 = XMVectorSubtract(Load(p2), Load(p0));
        return Store3(XMVector3Normalize(XMVector3Cross(e1, e2)));
    }
    // Centroïde d'un triangle
    inline XMFLOAT3 TriangleCentroid(const XMFLOAT3& p0, const XMFLOAT3& p1, const XMFLOAT3& p2)
    {
        return Store3(XMVectorScale(
            XMVectorAdd(XMVectorAdd(Load(p0), Load(p1)), Load(p2)),
            1.f / 3.f));
    }
    // Min / Max composante par composante
    inline XMFLOAT3 Min(const XMFLOAT3& a, const XMFLOAT3& b)
    {
        return Store3(XMVectorMin(Load(a), Load(b)));
    }
    inline XMFLOAT3 Max(const XMFLOAT3& a, const XMFLOAT3& b)
    {
        return Store3(XMVectorMax(Load(a), Load(b)));
    }
    inline XMFLOAT3 Abs(const XMFLOAT3& v)
    {
        return Store3(XMVectorAbs(Load(v)));
    }
 
    inline bool NearlyEqual(const XMFLOAT3& a, const XMFLOAT3& b, float eps = EPSILON)
    {
        return XMVector3NearEqual(Load(a), Load(b), XMVectorReplicate(eps));
    }
 
    // Vecteurs cardinaux
    inline XMFLOAT3 Zero3()    { return XMFLOAT3( 0.f,  0.f,  0.f); }
    inline XMFLOAT3 One3()     { return XMFLOAT3( 1.f,  1.f,  1.f); }
    inline XMFLOAT3 Right()    { return XMFLOAT3( 1.f,  0.f,  0.f); }
    inline XMFLOAT3 Left()     { return XMFLOAT3(-1.f,  0.f,  0.f); }
    inline XMFLOAT3 Up()       { return XMFLOAT3( 0.f,  1.f,  0.f); }
    inline XMFLOAT3 Down()     { return XMFLOAT3( 0.f, -1.f,  0.f); }
    inline XMFLOAT3 Forward()  { return XMFLOAT3( 0.f,  0.f,  1.f); }
    inline XMFLOAT3 Backward() { return XMFLOAT3( 0.f,  0.f, -1.f); }
 
 
    // =========================================================================
    // XMFLOAT4
    // =========================================================================
 
    inline XMFLOAT4 Add(const XMFLOAT4& a, const XMFLOAT4& b)
    {
        return Store4(XMVectorAdd(Load(a), Load(b)));
    }
    inline XMFLOAT4 Sub(const XMFLOAT4& a, const XMFLOAT4& b)
    {
        return Store4(XMVectorSubtract(Load(a), Load(b)));
    }
    inline XMFLOAT4 Scale(const XMFLOAT4& v, float s)
    {
        return Store4(XMVectorScale(Load(v), s));
    }
    inline float Dot(const XMFLOAT4& a, const XMFLOAT4& b)
    {
        return XMVectorGetX(XMVector4Dot(Load(a), Load(b)));
    }
    inline float Length(const XMFLOAT4& v)
    {
        return XMVectorGetX(XMVector4Length(Load(v)));
    }
    inline XMFLOAT4 Normalize(const XMFLOAT4& v)
    {
        return Store4(XMVector4Normalize(Load(v)));
    }
    inline XMFLOAT4 Lerp(const XMFLOAT4& a, const XMFLOAT4& b, float t)
    {
        return Store4(XMVectorLerp(Load(a), Load(b), t));
    }
    // Conversions Float3 <-> Float4
    inline XMFLOAT4 ToFloat4(const XMFLOAT3& v, float w = 1.f)
    {
        return Store4(XMVectorSetW(Load(v), w));
    }
    inline XMFLOAT3 ToFloat3(const XMFLOAT4& v)
    {
        XMFLOAT3 r; XMStoreFloat3(&r, Load(v)); return r;
    }
    // Division perspective (w != 0)
    inline XMFLOAT3 PerspectiveDivide(const XMFLOAT4& v)
    {
        assert(abs(v.w) > EPSILON);
        return Store3(XMVectorScale(Load(v), 1.f / v.w));
    }
 
 
    // =========================================================================
    // QUATERNIONS  (XMFLOAT4 : x,y,z = imaginaire, w = réel)
    // =========================================================================
 
    inline XMFLOAT4 QuatIdentity()
    {
        return Store4(XMQuaternionIdentity());
    }
    inline XMFLOAT4 QuatNormalize(const XMFLOAT4& q)
    {
        return Store4(XMQuaternionNormalize(Load(q)));
    }
    inline XMFLOAT4 QuatConjugate(const XMFLOAT4& q)
    {
        return Store4(XMQuaternionConjugate(Load(q)));
    }
    inline XMFLOAT4 QuatInverse(const XMFLOAT4& q)
    {
        return Store4(XMQuaternionInverse(Load(q)));
    }
    inline XMFLOAT4 QuatMul(const XMFLOAT4& q1, const XMFLOAT4& q2)
    {
        return Store4(XMQuaternionMultiply(Load(q1), Load(q2)));
    }
    // Depuis axe + angle (radians)
    inline XMFLOAT4 QuatFromAxisAngle(const XMFLOAT3& axis, float angle)
    {
        return Store4(XMQuaternionRotationAxis(Load(axis), angle));
    }
    // Depuis angles d'Euler pitch/yaw/roll (radians)
    inline XMFLOAT4 QuatFromEuler(float pitch, float yaw, float roll)
    {
        return Store4(XMQuaternionRotationRollPitchYaw(pitch, yaw, roll));
    }
    inline XMFLOAT4 QuatFromEuler(const XMFLOAT3& pitchYawRoll)
    {
        return QuatFromEuler(pitchYawRoll.x, pitchYawRoll.y, pitchYawRoll.z);
    }
    // Quaternion -> angles d'Euler (pitch, yaw, roll) en radians
    // DirectXMath n'a pas de fonction directe pour cette décomposition
    inline XMFLOAT3 QuatToEuler(const XMFLOAT4& q)
    {
        float sinr = 2.f * (q.w * q.x + q.y * q.z);
        float cosr = 1.f - 2.f * (q.x * q.x + q.y * q.y);
        float pitch = atan2(sinr, cosr);
 
        float sinp = 2.f * (q.w * q.y - q.z * q.x);
        float yaw   = (abs(sinp) >= 1.f)
            ? copysign(HALF_PI, sinp)
            : asin(sinp);
 
        float siny = 2.f * (q.w * q.z + q.x * q.y);
        float cosy = 1.f - 2.f * (q.y * q.y + q.z * q.z);
        float roll  = atan2(siny, cosy);
 
        return XMFLOAT3(pitch, yaw, roll);
    }
    // Rotation d'un vecteur par un quaternion
    inline XMFLOAT3 QuatRotate(const XMFLOAT4& q, const XMFLOAT3& v)
    {
        return Store3(XMVector3Rotate(Load(v), Load(q)));
    }
    // Slerp entre deux quaternions
    inline XMFLOAT4 QuatSlerp(const XMFLOAT4& a, const XMFLOAT4& b, float t)
    {
        return Store4(XMQuaternionSlerp(Load(a), Load(b), t));
    }
    // Depuis matrice de rotation
    inline XMFLOAT4 QuatFromMatrix(const XMFLOAT4X4& m)
    {
        return Store4(XMQuaternionRotationMatrix(XMLoadFloat4x4(&m)));
    }
 
 
    // =========================================================================
    // COURBES & INTERPOLATION
    // =========================================================================
 
    // Hermite cubique
    inline XMFLOAT3 HermiteInterp(
        const XMFLOAT3& p0, const XMFLOAT3& m0,
        const XMFLOAT3& p1, const XMFLOAT3& m1, float t)
    {
        return Store3(XMVectorHermite(Load(p0), Load(m0), Load(p1), Load(m1), t));
    }
    // Catmull-Rom spline
    inline XMFLOAT3 CatmullRom(
        const XMFLOAT3& p0, const XMFLOAT3& p1,
        const XMFLOAT3& p2, const XMFLOAT3& p3, float t)
    {
        return Store3(XMVectorCatmullRom(Load(p0), Load(p1), Load(p2), Load(p3), t));
    }
    // Bézier cubique (implémentation manuelle, DirectXMath n'en a pas de native)
    inline XMFLOAT3 BezierCubic(
        const XMFLOAT3& p0, const XMFLOAT3& p1,
        const XMFLOAT3& p2, const XMFLOAT3& p3, float t)
    {
        float u = 1.f - t, u2 = u*u, u3 = u2*u, t2 = t*t, t3 = t2*t;
        XMVECTOR result = XMVectorAdd(XMVectorAdd(XMVectorAdd(
            XMVectorScale(Load(p0), u3),
            XMVectorScale(Load(p1), 3.f * u2 * t)),
            XMVectorScale(Load(p2), 3.f * u  * t2)),
            XMVectorScale(Load(p3), t3));
        return Store3(result);
    }
    // Coordonnées barycentriques
    inline XMFLOAT3 Barycentric(
        const XMFLOAT3& p0, const XMFLOAT3& p1, const XMFLOAT3& p2,
        float f, float g)
    {
        return Store3(XMVectorBaryCentric(Load(p0), Load(p1), Load(p2), f, g));
    }
 
 
    // =========================================================================
    // GÉOMÉTRIE / RAY CASTING
    // =========================================================================
 
    // Distance point / plan
    inline float PointPlaneDistance(const XMFLOAT3& point, const XMFLOAT3& normal, const XMFLOAT3& planePoint)
    {
        return XMVectorGetX(XMVector3Dot(
            XMVectorSubtract(Load(point), Load(planePoint)),
            Load(normal)));
    }
    // Projection d'un point sur un plan
    inline XMFLOAT3 ProjectPointOnPlane(const XMFLOAT3& point, const XMFLOAT3& normal, const XMFLOAT3& planePoint)
    {
        float dist = PointPlaneDistance(point, normal, planePoint);
        return Store3(XMVectorSubtract(Load(point), XMVectorScale(Load(normal), dist)));
    }
    // Point le plus proche sur le segment [a,b]
    inline XMFLOAT3 ClosestPointOnSegment(const XMFLOAT3& p, const XMFLOAT3& a, const XMFLOAT3& b)
    {
        XMVECTOR ab = XMVectorSubtract(Load(b), Load(a));
        float t = XMVectorGetX(XMVector3Dot(XMVectorSubtract(Load(p), Load(a)), ab))
                / XMVectorGetX(XMVector3LengthSq(ab));
        return Store3(XMVectorAdd(Load(a), XMVectorScale(ab, Clamp(t, 0.f, 1.f))));
    }
 
    struct RayHit
    {
        bool     hit      = false;
        float    distance = 0.f;
        XMFLOAT3 point    = { 0.f, 0.f, 0.f };
    };
 
    // Rayon / plan
    inline RayHit RayPlaneIntersect(
        const XMFLOAT3& rayOrigin, const XMFLOAT3& rayDir,
        const XMFLOAT3& planeNormal, const XMFLOAT3& planePoint)
    {
        XMVECTOR n  = Load(planeNormal);
        XMVECTOR d  = Load(rayDir);
        float denom = XMVectorGetX(XMVector3Dot(d, n));
        if (abs(denom) < EPSILON) return {};
        float t = XMVectorGetX(XMVector3Dot(
            XMVectorSubtract(Load(planePoint), Load(rayOrigin)), n)) / denom;
        if (t < 0.f) return {};
        return { true, t, Store3(XMVectorAdd(Load(rayOrigin), XMVectorScale(d, t))) };
    }
 
    // Rayon / sphère
    inline RayHit RaySphereIntersect(
        const XMFLOAT3& rayOrigin, const XMFLOAT3& rayDir,
        const XMFLOAT3& center, float radius)
    {
        XMVECTOR oc = XMVectorSubtract(Load(rayOrigin), Load(center));
        XMVECTOR d  = Load(rayDir);
        float a     = XMVectorGetX(XMVector3LengthSq(d));
        float b     = 2.f * XMVectorGetX(XMVector3Dot(oc, d));
        float c     = XMVectorGetX(XMVector3LengthSq(oc)) - radius * radius;
        float disc  = b * b - 4.f * a * c;
        if (disc < 0.f) return {};
        float t = (-b - sqrt(disc)) / (2.f * a);
        if (t < 0.f) t = (-b + sqrt(disc)) / (2.f * a);
        if (t < 0.f) return {};
        return { true, t, Store3(XMVectorAdd(Load(rayOrigin), XMVectorScale(d, t))) };
    }
 
    // Rayon / triangle (Möller-Trumbore)
    inline RayHit RayTriangleIntersect(
        const XMFLOAT3& rayOrigin, const XMFLOAT3& rayDir,
        const XMFLOAT3& v0, const XMFLOAT3& v1, const XMFLOAT3& v2)
    {
        XMVECTOR e1 = XMVectorSubtract(Load(v1), Load(v0));
        XMVECTOR e2 = XMVectorSubtract(Load(v2), Load(v0));
        XMVECTOR d  = Load(rayDir);
        XMVECTOR h  = XMVector3Cross(d, e2);
        float a     = XMVectorGetX(XMVector3Dot(e1, h));
        if (abs(a) < EPSILON) return {};
        float f     = 1.f / a;
        XMVECTOR s  = XMVectorSubtract(Load(rayOrigin), Load(v0));
        float u     = f * XMVectorGetX(XMVector3Dot(s, h));
        if (u < 0.f || u > 1.f) return {};
        XMVECTOR q  = XMVector3Cross(s, e1);
        float v     = f * XMVectorGetX(XMVector3Dot(d, q));
        if (v < 0.f || u + v > 1.f) return {};
        float t     = f * XMVectorGetX(XMVector3Dot(e2, q));
        if (t < EPSILON) return {};
        return { true, t, Store3(XMVectorAdd(Load(rayOrigin), XMVectorScale(d, t))) };
    }
 
 
    // =========================================================================
    // COULEURS (XMFLOAT4 R,G,B,A dans [0,1])
    // =========================================================================
 
    // HSV -> RGB
    inline XMFLOAT3 HSVToRGB(const XMFLOAT3& hsv)
    {
        float h = hsv.x, s = hsv.y, v = hsv.z;
        float c = v * s, x = c * (1.f - abs(fmod(h / 60.f, 2.f) - 1.f)), m = v - c;
        float r = 0, g = 0, b = 0;
        if      (h <  60.f) { r = c; g = x; }
        else if (h < 120.f) { r = x; g = c; }
        else if (h < 180.f) { g = c; b = x; }
        else if (h < 240.f) { g = x; b = c; }
        else if (h < 300.f) { r = x; b = c; }
        else                { r = c; b = x; }
        return XMFLOAT3(r + m, g + m, b + m);
    }
 
    // Luminance perceptuelle BT.709
    inline float Luminance(const XMFLOAT3& rgb)
    {
        return XMVectorGetX(XMVector3Dot(Load(rgb),
            XMVectorSet(0.2126f, 0.7152f, 0.0722f, 0.f)));
    }
 
    // Gamma <-> Linéaire (sRGB ≈ 2.2)
    inline XMFLOAT3 GammaToLinear(const XMFLOAT3& c)
    {
        return XMFLOAT3(pow(c.x, 2.2f), pow(c.y, 2.2f), pow(c.z, 2.2f));
    }
    inline XMFLOAT3 LinearToGamma(const XMFLOAT3& c)
    {
        constexpr float inv = 1.f / 2.2f;
        return XMFLOAT3(pow(c.x, inv), pow(c.y, inv), pow(c.z, inv));
    }
 
 
    // =========================================================================
    // BRUIT & ALÉATOIRE
    // =========================================================================
 
    inline uint32_t WangHash(uint32_t seed)
    {
        seed = (seed ^ 61u) ^ (seed >> 16u);
        seed *= 9u; seed ^= seed >> 4u;
        seed *= 0x27d4eb2du; seed ^= seed >> 15u;
        return seed;
    }
    inline float RandFloat01(uint32_t seed)
    {
        return static_cast<float>(WangHash(seed)) / static_cast<float>(0xFFFFFFFFu);
    }
    inline float ValueNoise(int x, int y)
    {
        return RandFloat01(static_cast<uint32_t>(x * 1619 + y * 31337));
    }
    inline float SmoothedNoise(float x, float y)
    {
        int ix = static_cast<int>(floor(x)), iy = static_cast<int>(floor(y));
        float fx = x - ix, fy = y - iy;
        float v00 = ValueNoise(ix,   iy  ), v10 = ValueNoise(ix+1, iy  );
        float v01 = ValueNoise(ix,   iy+1), v11 = ValueNoise(ix+1, iy+1);
        float tx  = fx * fx * (3.f - 2.f * fx), ty = fy * fy * (3.f - 2.f * fy);
        return Lerp(Lerp(v00, v10, tx), Lerp(v01, v11, tx), ty);
    }
 
} // namespace MathUtils
 
 
// =============================================================================
// OPÉRATEURS GLOBAUX — XMFLOAT2
// Permettent d'écrire : a + b, a - b, a * s, s * a, a / s, -a, a == b
// =============================================================================
 
inline DirectX::XMFLOAT2 operator+(const DirectX::XMFLOAT2& a, const DirectX::XMFLOAT2& b)
{
    DirectX::XMFLOAT2 r;
    DirectX::XMStoreFloat2(&r, DirectX::XMVectorAdd(DirectX::XMLoadFloat2(&a), DirectX::XMLoadFloat2(&b)));
    return r;
}
inline DirectX::XMFLOAT2 operator-(const DirectX::XMFLOAT2& a, const DirectX::XMFLOAT2& b)
{
    DirectX::XMFLOAT2 r;
    DirectX::XMStoreFloat2(&r, DirectX::XMVectorSubtract(DirectX::XMLoadFloat2(&a), DirectX::XMLoadFloat2(&b)));
    return r;
}
inline DirectX::XMFLOAT2 operator*(const DirectX::XMFLOAT2& v, float s)
{
    DirectX::XMFLOAT2 r;
    DirectX::XMStoreFloat2(&r, DirectX::XMVectorScale(DirectX::XMLoadFloat2(&v), s));
    return r;
}
inline DirectX::XMFLOAT2 operator*(float s, const DirectX::XMFLOAT2& v)   { return v * s; }
inline DirectX::XMFLOAT2 operator/(const DirectX::XMFLOAT2& v, float s)   { return v * (1.f / s); }
inline DirectX::XMFLOAT2 operator-(const DirectX::XMFLOAT2& v)
{
    DirectX::XMFLOAT2 r;
    DirectX::XMStoreFloat2(&r, DirectX::XMVectorNegate(DirectX::XMLoadFloat2(&v)));
    return r;
}
inline DirectX::XMFLOAT2& operator+=(DirectX::XMFLOAT2& a, const DirectX::XMFLOAT2& b) { a = a + b; return a; }
inline DirectX::XMFLOAT2& operator-=(DirectX::XMFLOAT2& a, const DirectX::XMFLOAT2& b) { a = a - b; return a; }
inline DirectX::XMFLOAT2& operator*=(DirectX::XMFLOAT2& v, float s)                    { v = v * s; return v; }
inline DirectX::XMFLOAT2& operator/=(DirectX::XMFLOAT2& v, float s)                    { v = v / s; return v; }
inline bool operator==(const DirectX::XMFLOAT2& a, const DirectX::XMFLOAT2& b)
{
    return DirectX::XMVector2Equal(DirectX::XMLoadFloat2(&a), DirectX::XMLoadFloat2(&b));
}
inline bool operator!=(const DirectX::XMFLOAT2& a, const DirectX::XMFLOAT2& b) { return !(a == b); }
 
 
// =============================================================================
// OPÉRATEURS GLOBAUX — XMFLOAT3
// Permettent d'écrire : a + b, a - b, a * b (composante), a * s, s * a, a / s, -a, a == b
// =============================================================================
 
inline DirectX::XMFLOAT3 operator+(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b)
{
    DirectX::XMFLOAT3 r;
    DirectX::XMStoreFloat3(&r, DirectX::XMVectorAdd(DirectX::XMLoadFloat3(&a), DirectX::XMLoadFloat3(&b)));
    return r;
}
inline DirectX::XMFLOAT3 operator-(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b)
{
    DirectX::XMFLOAT3 r;
    DirectX::XMStoreFloat3(&r, DirectX::XMVectorSubtract(DirectX::XMLoadFloat3(&a), DirectX::XMLoadFloat3(&b)));
    return r;
}
// Produit composante par composante (Hadamard)
inline DirectX::XMFLOAT3 operator*(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b)
{
    DirectX::XMFLOAT3 r;
    DirectX::XMStoreFloat3(&r, DirectX::XMVectorMultiply(DirectX::XMLoadFloat3(&a), DirectX::XMLoadFloat3(&b)));
    return r;
}
inline DirectX::XMFLOAT3 operator*(const DirectX::XMFLOAT3& v, float s)
{
    DirectX::XMFLOAT3 r;
    DirectX::XMStoreFloat3(&r, DirectX::XMVectorScale(DirectX::XMLoadFloat3(&v), s));
    return r;
}
inline DirectX::XMFLOAT3 operator*(float s, const DirectX::XMFLOAT3& v)   { return v * s; }
inline DirectX::XMFLOAT3 operator/(const DirectX::XMFLOAT3& v, float s)   { return v * (1.f / s); }
inline DirectX::XMFLOAT3 operator/(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b)
{
    DirectX::XMFLOAT3 r;
    DirectX::XMStoreFloat3(&r, DirectX::XMVectorDivide(DirectX::XMLoadFloat3(&a), DirectX::XMLoadFloat3(&b)));
    return r;
}
inline DirectX::XMFLOAT3 operator-(const DirectX::XMFLOAT3& v)
{
    DirectX::XMFLOAT3 r;
    DirectX::XMStoreFloat3(&r, DirectX::XMVectorNegate(DirectX::XMLoadFloat3(&v)));
    return r;
}
inline DirectX::XMFLOAT3& operator+=(DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b) { a = a + b; return a; }
inline DirectX::XMFLOAT3& operator-=(DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b) { a = a - b; return a; }
inline DirectX::XMFLOAT3& operator*=(DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b) { a = a * b; return a; }
inline DirectX::XMFLOAT3& operator*=(DirectX::XMFLOAT3& v, float s)                    { v = v * s; return v; }
inline DirectX::XMFLOAT3& operator/=(DirectX::XMFLOAT3& v, float s)                    { v = v / s; return v; }
inline bool operator==(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b)
{
    return DirectX::XMVector3Equal(DirectX::XMLoadFloat3(&a), DirectX::XMLoadFloat3(&b));
}
inline bool operator!=(const DirectX::XMFLOAT3& a, const DirectX::XMFLOAT3& b) { return !(a == b); }
 
 
// =============================================================================
// OPÉRATEURS GLOBAUX — XMFLOAT4
// =============================================================================
 
inline DirectX::XMFLOAT4 operator+(const DirectX::XMFLOAT4& a, const DirectX::XMFLOAT4& b)
{
    DirectX::XMFLOAT4 r;
    DirectX::XMStoreFloat4(&r, DirectX::XMVectorAdd(DirectX::XMLoadFloat4(&a), DirectX::XMLoadFloat4(&b)));
    return r;
}
inline DirectX::XMFLOAT4 operator-(const DirectX::XMFLOAT4& a, const DirectX::XMFLOAT4& b)
{
    DirectX::XMFLOAT4 r;
    DirectX::XMStoreFloat4(&r, DirectX::XMVectorSubtract(DirectX::XMLoadFloat4(&a), DirectX::XMLoadFloat4(&b)));
    return r;
}
inline DirectX::XMFLOAT4 operator*(const DirectX::XMFLOAT4& a, const DirectX::XMFLOAT4& b)
{
    DirectX::XMFLOAT4 r;
    DirectX::XMStoreFloat4(&r, DirectX::XMVectorMultiply(DirectX::XMLoadFloat4(&a), DirectX::XMLoadFloat4(&b)));
    return r;
}
inline DirectX::XMFLOAT4 operator*(const DirectX::XMFLOAT4& v, float s)
{
    DirectX::XMFLOAT4 r;
    DirectX::XMStoreFloat4(&r, DirectX::XMVectorScale(DirectX::XMLoadFloat4(&v), s));
    return r;
}
inline DirectX::XMFLOAT4 operator*(float s, const DirectX::XMFLOAT4& v)   { return v * s; }
inline DirectX::XMFLOAT4 operator/(const DirectX::XMFLOAT4& v, float s)   { return v * (1.f / s); }
inline DirectX::XMFLOAT4 operator-(const DirectX::XMFLOAT4& v)
{
    DirectX::XMFLOAT4 r;
    DirectX::XMStoreFloat4(&r, DirectX::XMVectorNegate(DirectX::XMLoadFloat4(&v)));
    return r;
}
inline DirectX::XMFLOAT4& operator+=(DirectX::XMFLOAT4& a, const DirectX::XMFLOAT4& b) { a = a + b; return a; }
inline DirectX::XMFLOAT4& operator-=(DirectX::XMFLOAT4& a, const DirectX::XMFLOAT4& b) { a = a - b; return a; }
inline DirectX::XMFLOAT4& operator*=(DirectX::XMFLOAT4& v, float s)                    { v = v * s; return v; }
inline DirectX::XMFLOAT4& operator/=(DirectX::XMFLOAT4& v, float s)                    { v = v / s; return v; }
inline bool operator==(const DirectX::XMFLOAT4& a, const DirectX::XMFLOAT4& b)
{
    return DirectX::XMVector4Equal(DirectX::XMLoadFloat4(&a), DirectX::XMLoadFloat4(&b));
}
inline bool operator!=(const DirectX::XMFLOAT4& a, const DirectX::XMFLOAT4& b) { return !(a == b); }

#endif