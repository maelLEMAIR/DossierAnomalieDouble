#ifndef MAP_LOADER_TEST_CPP_INCLUDED
#define MAP_LOADER_TEST_CPP_INCLUDED

#include "MapLoader.h"

#include "Rooms/Room.h"

#include "Anomalies/Scaling.h"
#include "Anomalies/Gravity.h"
#include "Anomalies/AnomalyBase.h"
#include "Anomalies/FloatingObject.h"
#include "Anomalies/LookAtThePlayer.h"
#include "Anomalies/FollowPlayer.h"

UnorderedMap<String, MapLoader::MaterialFactory> MapLoader::s_materialFactories;
UnorderedMap<String, MapLoader::ComponentLoader> MapLoader::s_componentLoaders;


void MapLoader::RegisterDefaults()
{
    s_materialFactories["texture"] = [](Device* dev, const json& j) -> Material*
    {
        Shader* s = RessourceManager::GetShader("Texture");
        Material* mat = s->CreateMaterial();

        auto TryLoadTex = [&](const char* key, const char* slot)
        {
            if (!j["color"][key].contains("file")) return;
            String str = j["color"][key]["file"].get<String>();
            WString wstr(str.size(), L'\0');
            MultiByteToWideChar(CP_UTF8, 0, str.c_str(), -1, wstr.data(), (int)wstr.size());
            Texture* tex = dev->CreateTexture(L"../../res/Textures/" + wstr);
            mat->SetTexture(slot, tex);
        };

        TryLoadTex("base_color", "Albedo");
        TryLoadTex("roughness",  "Roughness");
        TryLoadTex("normal",     "Normal");
        return mat;
    };

    s_materialFactories["color"] = [](Device* dev, const json& j) -> Material*
    {
        Shader* s = RessourceManager::GetShader("Color");
        Material* mat = s->CreateMaterial();
        mat->SetFloat4("DiffuseAlbedo", {
            j["color"]["base_color"]["r"].get<float>(),
            j["color"]["base_color"]["g"].get<float>(),
            j["color"]["base_color"]["b"].get<float>(),
            1.0f
        });
        return mat;
    };

    s_componentLoaders["collider"] = [](Entity* e, Scene& scene, const json& j, Room* pRoom)
    {
        TransformComponent* tr = scene.GetComponentType<TransformComponent>(e);
        Collider* col = scene.AddComponent<Collider>(e);
        col->colliderType = ColliderType::BOX;
        col->scale = {2.0f, 2.0f, 2.0f};
        col->isStatic     = true;
    };

    s_componentLoaders["typeOfAnomaly"] = [](Entity* e, Scene& scene, const json& j, Room* pRoom)
    {
        std::unordered_map<std::string, AnomalyType> toEnum = {
            {"LOOK",     AnomalyType::LookAtThePlayer},
            {"FLOATING", AnomalyType::FloatingObject},
            {"FOLLOW", AnomalyType::FollowPlayer}
        };

        for (const auto& item : j["typeOfAnomaly"])
        {
            String type = item.get<String>();

            if (!toEnum.contains(type))
                continue;

            switch (toEnum.at(type))
            {
            case AnomalyType::LookAtThePlayer:
                {
                    AnomalyConfig cfg;
                    cfg.duration = 10.0f;
                    cfg.loops = true;

                    LookAtThePlayer* pLookAnomaly = new LookAtThePlayer(cfg, e);
                    pRoom->AddAnomaly(AnomalyType::LookAtThePlayer, pLookAnomaly);
                    pLookAnomaly->OnInit();
                    break;
                }
            case AnomalyType::FloatingObject:
                {
                    AnomalyConfig cfg;
                    cfg.activeTime   = 2.0f;    // cycles très rapides
                    cfg.cooldownTime = 10.0f;   // micro-pause entre chaque
                    cfg.intensity    = 0.25f;   // amplitude plus grande
                    cfg.loops        = true;
                    cfg.startDelay   = 0.5f;

                    FloatingObject* pFloatingAnomaly = new FloatingObject(cfg, e);
                    pRoom->AddAnomaly(AnomalyType::FloatingObject, pFloatingAnomaly);
                    pFloatingAnomaly->OnInit();
                    break;
                }
            case AnomalyType::FollowPlayer:
                {
                    AnomalyConfig cfg;
                    cfg.duration = 10.0f;
                    cfg.loops = true;
                    cfg.startDelay = 5.0f;

                    FollowPlayer* pFollowAnomaly = new FollowPlayer(cfg, e);
                    pRoom->AddAnomaly(AnomalyType::FollowPlayer, pFollowAnomaly);
                    pFollowAnomaly->OnInit();
                    break;
                }
            default:
                break;
            }
        }
    };
    
    s_componentLoaders["door"] = [](Entity* e, Scene& scene, const json& j, Room* pRoom)
    {
        Entity* pTriggerDoor = scene.CreateEntity();
        if ( pRoom ) pRoom->AddEntity(pTriggerDoor);
        
        Collider* pTriggerDoorCollider = scene.AddComponent<Collider>(pTriggerDoor);
        pTriggerDoorCollider->colliderType = ColliderType::BOX;
        pTriggerDoorCollider->isTrigger = true;

        TransformComponent* pTransformEntity = scene.GetComponentType<TransformComponent>(e);
        DoorStateGlobal* doorStateGlobal = new DoorStateGlobal();
        if (pRoom) doorStateGlobal->m_pRoom = pRoom;
        doorStateGlobal->pTransformDoor = pTransformEntity;
        doorStateGlobal->positionStart = pTransformEntity->transform.GetWorldPosition();
        doorStateGlobal->positionEnd = pTransformEntity->transform.GetWorldPosition() + XMFLOAT3(0.0f, 2.0f, 0.0f);
        StateMachineComponent* doorState = scene.AddComponent<StateMachineComponent>(pTriggerDoor);
        doorState->SetStateGlobal(doorStateGlobal);
    };
}


Transform MapLoader::LoadTransform(Entity* _pEntity, Scene& _scene, const json& _obj)
{
    TransformComponent* tr = _scene.GetComponentType<TransformComponent>(_pEntity);

    XMFLOAT3 pos = {
        _obj["position"][0].get<float>(),
        _obj["position"][2].get<float>(),
        _obj["position"][1].get<float>()
    };
    tr->transform.SetWorldPosition(pos);

    XMFLOAT3 scale = {
        _obj["scale"][0].get<float>(),
        _obj["scale"][2].get<float>(),
        _obj["scale"][1].get<float>()
    };
    tr->transform.SetWorldScale(scale);

    XMFLOAT4 quat = {
        _obj["rotation"][0].get<float>(),
        _obj["rotation"][2].get<float>(),
        _obj["rotation"][1].get<float>(),
       -_obj["rotation"][3].get<float>()
    };
    tr->transform.SetWorldRotationQuaternion(quat);

    return tr->transform;
}

Geometry* MapLoader::LoadMesh(Entity* _pEntity, Scene& _scene, Device* _pDevice, Room* _pRoom, const json& _obj)
{
    MeshRenderer* mesh = _scene.AddComponent<MeshRenderer>(_pEntity);

    String geoKey = _obj["geometry"].get<String>();
    
    Geometry* pGeo = nullptr;

    if (geoKey == "NONE")
    {
        pGeo = GeometryFactory::LoadJsonGeometry(_pDevice, _obj);
        RessourceManager::AddGeometry(_pEntity->name, pGeo);
    }
    else
    {
        pGeo = RessourceManager::GetGeometry(geoKey);
        if (!pGeo)
        {
            pGeo = GeometryFactory::LoadJsonGeometry(_pDevice, _obj);
            RessourceManager::AddGeometry(geoKey, pGeo);
        }
    }
    mesh->pGeometry = pGeo;

    if (!_obj.contains("color")) return pGeo;

    String matName = _obj["color"]["material_name"].get<String>();
    Material* pMat = RessourceManager::GetMaterial(matName.c_str());

    if (!pMat)
    {
        String type = _obj["color"]["type"].get<String>();
        auto it = s_materialFactories.find(type);
        if (it != s_materialFactories.end())
            pMat = it->second(_pDevice, _obj);
        else
            std::cerr << "Unknown material type: " << type << "\n";

        if (pMat)
            RessourceManager::AddMaterial(matName, pMat);
    }
    mesh->pMaterial = pMat;

    return pGeo;
}

LoadData MapLoader::LoadMap(String _path, Device* _pDevice, Scene& _scene, Room* _pRoom, String _nameRoom)
{
    RegisterDefaults();

    LoadData loadData;

    std::ifstream file(_path);
    if (!file.is_open()) {
        std::cerr << "Impossible d'ouvrir le fichier JSON: " << _path << "\n";
        return loadData;
    }

    json j;
    try { file >> j; }
    catch (json::parse_error& e) {
        std::cerr << "Erreur de parsing: " << e.what() << "\n";
        return loadData;;
    }

    for (const json& obj : j["objects"])
    {
        Entity* pEntity    = _scene.CreateEntity();
        pEntity->name      = _nameRoom + "_" + obj["name"].get<String>();

        if ( _pRoom )
        {
            pEntity->isActive  = false;
            _pRoom->AddEntity(pEntity);
        }
        
        loadData.vEntityNames.push_back(pEntity->name);

        Transform tr = LoadTransform(pEntity, _scene, obj);
        XMFLOAT3 pos = tr.GetWorldPosition();
        loadData.vWorldPos.push_back(pos);
        loadData.vWorldScale.push_back(tr.GetWorldScale());
        loadData.vWorldQuat.push_back(tr.GetWorldRotation());

        bool hasMesh = false;
        if ( obj.contains("has_mesh"))
        {
            hasMesh = obj["has_mesh"].get<bool>();;
            loadData.vHasMesh.push_back(hasMesh);
        }
        
        bool isLight = obj["isLight"].get<bool>();
        if ( isLight ) if ( _pRoom ) _pRoom->AddLightPos(pos);
        loadData.vIsLights.push_back(isLight);

        bool hasCollider = obj["has_collider"].get<bool>();
        loadData.vHasCollider.push_back(hasCollider);
        
        if (hasMesh)
        {
            Geometry* pGeo = LoadMesh(pEntity, _scene, _pDevice, _pRoom, obj);
            loadData.vVertex.push_back(pGeo->GetVertex());
            loadData.vIndexes.push_back(pGeo->GetIndexes());
        }

        if (hasCollider)
            s_componentLoaders["collider"](pEntity, _scene, obj, _pRoom);

        if ( obj.contains("typeOfAnomaly") )
            if (obj["typeOfAnomaly"].empty() == false)
                s_componentLoaders["typeOfAnomaly"](pEntity, _scene, obj, _pRoom);
    
        if ( obj.contains("isDoor") )
            if (obj["isDoor"].get<bool>())
                s_componentLoaders["door"](pEntity, _scene, obj, _pRoom);
    }

    return loadData;
}

void MapLoader::LoadCashMap(String _name, Device* _pDevice, Scene& _scene, Room* _pRoom, String _nameRoom)
{
    String CachePath = "../../res/Cache/" + _name + ".cache";
    String JSONPath = "../../res/JSON/" + _name + ".json";
    //check if cash exist and if is more recent than the json
    if (std::filesystem::exists(CachePath))
    {
        auto casheTime = std::filesystem::last_write_time(CachePath);
        auto jsonTime = std::filesystem::last_write_time(JSONPath);

        if (casheTime > jsonTime)
        {
            std::cout << "Load from cache" << std::endl;
            float tempTime = EngineManager::GetChrono().GetTotalTime();
            LoadFromCache(CachePath, _pDevice, _scene);
            float timeLoad = EngineManager::GetChrono().GetTotalTime() - tempTime;
            std::cout << "End loading : " << timeLoad << std::endl;
            return;
        }
    }

    std::cout << "Cashe file doesnt exist or isnt updated from the json, reload" << std::endl;
    float tempTime = EngineManager::GetChrono().GetTotalTime();
    LoadData loadData = LoadMap(JSONPath, _pDevice, _scene, _pRoom);
    float timeLoad = EngineManager::GetChrono().GetTotalTime() - tempTime;
    std::cout << "End loading : " << timeLoad << std::endl;
    SaveLoadDataCache(loadData, CachePath);
}

void MapLoader::SaveLoadDataCache(LoadData _loadData, String _path)
{
    std::ofstream file(_path, std::ios::binary);


    //Count of entity
    size_t count = _loadData.vEntityNames.size();

    file.write((char*)&count, sizeof(count));


    //Name entity
    for (const String& name : _loadData.vEntityNames)
    {
        size_t strLen = name.size();
        file.write((char*)&strLen, sizeof(strLen));
        file.write(name.data(), strLen);
    }


    //Transform
    //file.write((char*)_loadData.vTransforms.data(), count * sizeof(Transform));
    file.write((char*)_loadData.vWorldPos.data(), count * sizeof(XMFLOAT3));
    file.write((char*)_loadData.vWorldScale.data(), count * sizeof(XMFLOAT3));
    file.write((char*)_loadData.vWorldQuat.data(), count * sizeof(XMFLOAT4));


    //Is light
    Vector<uint8_t> isLights;

    for (bool isLight : _loadData.vIsLights)
        isLights.push_back(isLight);

    file.write((char*)isLights.data(), count * sizeof(uint8_t));


    /// Nombre de meshes
    size_t meshCount = _loadData.vVertex.size();
    file.write((char*)&meshCount, sizeof(meshCount));

    for (size_t i = 0; i < meshCount; ++i)
    {
        // Vertices du mesh i
        size_t vertexCount = _loadData.vVertex[i].size();
        file.write((char*)&vertexCount, sizeof(vertexCount));
        file.write((char*)_loadData.vVertex[i].data(), vertexCount * sizeof(Vertex));

        // Indices du mesh i
        size_t indexCount = _loadData.vIndexes[i].size();
        file.write((char*)&indexCount, sizeof(indexCount));
        file.write((char*)_loadData.vIndexes[i].data(), indexCount * sizeof(uint32));
    }


    //Has collider
    Vector<uint8_t> hasColliders;

    for (bool hasCollider : _loadData.vHasCollider)
        hasColliders.push_back(hasCollider);

    file.write((char*)hasColliders.data(), count * sizeof(uint8_t));
}

void MapLoader::LoadFromCache(String _path, Device* _pDevice, Scene& _scene)
{
    LoadData loadData;
    std::ifstream file(_path, std::ios::binary);

    if (!file.is_open()) return;

    // Count
    size_t count;
    file.read((char*)&count, sizeof(count));
    loadData.vEntityNames.resize(count);
    loadData.vWorldPos.resize(count);
    loadData.vWorldScale.resize(count);
    loadData.vWorldQuat.resize(count);

    // Names
    for (size_t i = 0; i < count; ++i)
    {
        size_t strLen;
        file.read((char*)&strLen, sizeof(strLen));
        loadData.vEntityNames[i].resize(strLen);
        file.read(loadData.vEntityNames[i].data(), strLen);
    }

    // Transforms
    //file.read((char*)loadData.vTransforms.data(), count * sizeof(Transform));
    file.read((char*)loadData.vWorldPos.data(), count * sizeof(XMFLOAT3));
    file.read((char*)loadData.vWorldScale.data(), count * sizeof(XMFLOAT3));
    file.read((char*)loadData.vWorldQuat.data(), count * sizeof(XMFLOAT4));

    // IsLights
    loadData.vIsLights.resize(count);
    for (size_t i = 0; i < count; ++i)
    {
        uint8_t val;
        file.read((char*)&val, sizeof(uint8_t));
        loadData.vIsLights[i] = static_cast<bool>(val);
    }

    // Mesh geo
    size_t meshCount;
    file.read((char*)&meshCount, sizeof(meshCount));
    loadData.vVertex.resize(meshCount);
    loadData.vIndexes.resize(meshCount);

    for (size_t i = 0; i < meshCount; ++i)
    {
        size_t vertexCount;
        file.read((char*)&vertexCount, sizeof(vertexCount));
        loadData.vVertex[i].resize(vertexCount);
        file.read((char*)loadData.vVertex[i].data(), vertexCount * sizeof(Vertex));

        size_t indexCount;
        file.read((char*)&indexCount, sizeof(indexCount));
        loadData.vIndexes[i].resize(indexCount);
        file.read((char*)loadData.vIndexes[i].data(), indexCount * sizeof(uint32)); 
    }

    // Has collider
    loadData.vHasCollider.resize(count);
    for (size_t i = 0; i < count; ++i)
    {
        uint8_t val;
        file.read((char*)&val, sizeof(uint8_t));
        loadData.vHasCollider[i] = static_cast<bool>(val);
    }

    // Create entities
    for (size_t i = 0; i < count; i++)
    {
        Entity* pNewEntity = _scene.CreateEntity();
        pNewEntity->name = loadData.vEntityNames[i];

        TransformComponent* pTr = _scene.GetComponentType<TransformComponent>(pNewEntity);
        pTr->transform.SetWorldPosition(loadData.vWorldPos[i]);
        pTr->transform.SetWorldScale(loadData.vWorldScale[i]);
        pTr->transform.SetLocalRotationQuaternion(loadData.vWorldQuat[i]);

        if (!loadData.vIsLights[i])
        {
            loadData.vVertex[i] = GeometryFactory::CalculateNormalsAndTangentsCustom(loadData.vVertex[i], loadData.vIndexes[i]);

            Geometry* pGeo = _pDevice->CreateGeometry();
            pGeo->SetVertexData(loadData.vVertex[i].data(), loadData.vVertex[i].size());
            pGeo->SetIndexData(loadData.vIndexes[i].data(), loadData.vIndexes[i].size());

            MeshRenderer* pMr = _scene.AddComponent<MeshRenderer>(pNewEntity);
            pMr->pGeometry = pGeo;

            if (loadData.vHasCollider[i])
            {
                Collider* col = _scene.AddComponent<Collider>(pNewEntity);
                col->colliderType = ColliderType::BOX;
                col->isStatic = true;
            }
        }
        else
        {
            /*LightComponent* lc = _scene.AddComponent<LightComponent>(pNewEntity);*/
            XMFLOAT3 pos = pTr->transform.GetWorldPosition();
            /*lc->SetLight(LightType::Point, 1.0f,
                { -0.5f, 0.0f, 0.5f }, { 1.0f, 1.0f, 1.0f, 0.5f },
                pos, 0.1f, 10.0f);*/
        }
    }
}

#endif