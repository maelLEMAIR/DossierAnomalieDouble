 #ifndef TEXT_COMPONENT_H_DEFINED
 #define TEXT_COMPONENT_H_DEFINED 

 #include "Component.h"
#include "../../Render/Generic/Render.h"
#include "Render/Generic/FontRendering/Text.hpp"
#include "../RessourceManager.h"

 class TextComponent : public Component
 {
 public:
	 void SetText(const String& _text)
	 {
		 if (m_pText == nullptr)
			 m_pText = EngineManager::GetDevice()->CreateText(RessourceManager::GetFont("Default"));

		 m_pText->SetString(_text);
	 }

	 void SetColor(float _r, float _g, float _b)
	 {
		 if (m_pText == nullptr)
			 m_pText = EngineManager::GetDevice()->CreateText(RessourceManager::GetFont("Default"));

		 m_pText->SetColor(_r, _g, _b);
	 }

	 XMFLOAT2 offsetCenter = { 0.0f, 0.0f };

 private:
	 Text* m_pText = nullptr;

	 friend class RenderSystem;
 };

 #endif