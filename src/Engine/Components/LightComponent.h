#ifndef LIGHT_COMPONENT_H_DEFINED
#define LIGHT_COMPONENT_H_DEFINED 

#include "Component.h"
#include "Render/Generic/Render.h"

class LightComponent : public Component
{
public:
    void SetStrength(float _strength) { m_strength = _strength; m_isUpdate = true;}
    void SetFalloffStart(float _fallOffStart) { m_falloffStart = _fallOffStart; m_isUpdate = true;}
    void SetDirection(XMFLOAT3 _direction) { m_direction = _direction; m_isUpdate = true;}
    void SetFalloffEnd(float _fallOffEnd) { m_falloffEnd = _fallOffEnd; m_isUpdate = true;}
    void SetPosition(XMFLOAT3 _pos) { m_position = _pos; m_isUpdate = true;}
    void SetSpotPower(float _spotPower) { m_spotPower = _spotPower; m_isUpdate = true;}
    void SetColor(XMFLOAT4 _color) { m_color = _color; m_isUpdate = true;}
    void SetType(LightType _type) { m_type = _type; m_isUpdate = true;}

    void SetLight(LightType _type = LightType::Point, float _strength = 1.0f, XMFLOAT3 _direction = { 1.0f, 0.0f, 0.0f }, XMFLOAT4 _color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f), XMFLOAT3 _pos = XMFLOAT3(0.0f, 0.0f, 0.0f), float _fallOffStart = 0.1f, float _fallOffEnd = 10.0f, float _spotPower = 2.0f)
    {
        m_type = _type;
        m_strength = _strength;
        m_direction = _direction;
        m_color = _color;
        m_position = _pos;
        m_falloffStart = _fallOffStart;
        m_falloffEnd = _fallOffEnd;
        m_spotPower = _spotPower;

        m_isUpdate = true;
    }

    XMFLOAT3 GetPosition() { return m_position; }

    void CallUpdate() { m_isUpdate = true; }

private:
    float m_strength = 1.0f;
    float m_falloffStart = 0.1f;
    XMFLOAT3 m_direction = { 1.0f, 0.0f, 0.0f };
    float m_falloffEnd = 10.0f;
    XMFLOAT3 m_position = { 0.0f, 0.0f, 0.0f }; //faire bouger avec l'entity
    float m_spotPower = 2.0f;
    XMFLOAT4 m_color = { 1.0f, 1.0f, 1.0f, 1.0f };

    LightType m_type = LightType::Point;

    bool m_isUpdate = false;

    friend class LightSystem;
};

#endif