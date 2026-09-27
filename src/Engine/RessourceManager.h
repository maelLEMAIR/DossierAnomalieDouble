#ifndef RESSOURCE_MANAGER_H_DEFINED
#define RESSOURCE_MANAGER_H_DEFINED

#include "../Render/Generic/Render.h"

class RessourceManager
{
public:
	RessourceManager();
	~RessourceManager();

	static void AddGeometry(std::string _name, Geometry* _pGeo);
	static Geometry* GetGeometry(std::string _name);

	static void AddMaterial(std::string _name, Material* pMat);
	static Material* GetMaterial(std::string _name);

	static void AddShader(std::string _name, Shader* _pShader);
	static Shader* GetShader(std::string _name);

	static void AddUiShader(std::string _name, UiShader* _pShader);
	static UiShader* GetUiShader(std::string _name);

	static void AddFont(std::string _name, RenderFont* _pFont);
	static RenderFont* GetFont(std::string _name);

	static void AddBrush(std::string _name, Brush* _pBrush);
	static Brush* GetBrush(std::string _name);

	static void AddTexture(std::string _name, Texture* _pTexture);
	static Texture* GetTexture(std::string _name);

private:
	static RessourceManager* s_pInstance;

	std::unordered_map<std::string, Geometry*> m_mGeometries;
	std::unordered_map<std::string, Shader*> m_mShaders;
	std::unordered_map<std::string, UiShader*> m_mUiShaders;
	std::unordered_map<std::string, Material*> m_mMaterials;
	std::unordered_map<std::string, RenderFont*> m_mFonts;
	std::unordered_map<std::string, Brush*> m_mBrushs;
	std::unordered_map<std::string, Texture*> m_mTextures;

	//Cash ressource
};

#endif