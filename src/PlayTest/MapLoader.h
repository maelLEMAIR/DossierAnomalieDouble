#ifndef MAP_LOADER_TEST_H_INCLUDED
#define MAP_LOADER_TEST_H_INCLUDED

#include "pch.h"

struct LoadData
{
    Vector<String> vEntityNames;
    Vector<XMFLOAT3> vWorldPos;
    Vector<XMFLOAT3> vWorldScale;
    Vector<XMFLOAT4> vWorldQuat;
    Vector<bool> vIsLights;
    Vector<Vector<Vertex>> vVertex;
    Vector<Vector<uint32>> vIndexes;
    Vector<bool> vHasCollider;
};

class MapLoader
{
public:
    static LoadData LoadMap(String _path, Device* _pDevice, Scene& _scene);
    static void LoadCashMap(String _name, Device* _pDevice, Scene& _scene);

private:
    using MaterialFactory = std::function<Material*(Device*, const json&)>;
    using ComponentLoader = std::function<void(Entity*, Scene&, const json&)>;

    static UnorderedMap<String, MaterialFactory> s_materialFactories;
    static UnorderedMap<String, ComponentLoader> s_componentLoaders;

    static void RegisterDefaults();
    static Transform LoadTransform(Entity* _pEntity, Scene& _scene, const json& _obj);
    static Geometry* LoadMesh(Entity* _pEntity, Scene& _scene, Device* _pDevice, const json& _obj);
    static bool FindTheWord(String const& _seqWord, String const& _word);

    //
    static void SaveLoadDataCache(LoadData _loadData, String _path);
    static void LoadFromCache(String _path, Device* _pDevice, Scene& _scene);
    
};

#endif