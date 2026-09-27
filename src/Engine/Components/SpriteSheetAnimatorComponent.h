#ifndef SPRITE_SHEET_ANIMATOR_COMPONENT_H_DEFINED
#define SPRITE_SHEET_ANIMATOR_COMPONENT_H_DEFINED 

#include "Component.h"
#include "../EngineManager.h"

class Material;
class UiMaterial;

class SpriteSheetAnimatorComponent : public Component
{
public:
	int currentFrame = 0;
	int totalFrames = 1;

	float timePerFrames = 0.1f;
	float elapsedTime = 0.0f;
	bool isPlaying = true;

	Vector<Texture*> vTextures;

	Material* pTargetMaterial = nullptr;
	UiMaterial* pTargetUiMaterial = nullptr;
	String textureParameterName = "";

	void SetSpriteSheet(WString _textureName, bool _isFromVLC = false)
	{
		WString path = L"../../res/Textures/" + _textureName + L"/" + _textureName;
		for (int i = 0; i < totalFrames; i++)
		{
			std::cout << "Load une texture" << std::endl;
			if (_isFromVLC)
			{
				WString zero;

				if (i <= 9)
					zero += L"0000";
				else if (i <= 99)
					zero += L"000";
				else if (i <= 999)
					zero += L"00";
				else if (i <= 9999)
					zero += L"0";

				Texture* tempTexture = EngineManager::GetDevice()->CreateTexture(path + zero + std::to_wstring(i) + L".dds");
				vTextures.push_back(tempTexture);
			}
			else
			{
				Texture* tempTexture = EngineManager::GetDevice()->CreateTexture(path + std::to_wstring(i) + L".dds");
				vTextures.push_back(tempTexture);
			}
		}
	}
};

#endif