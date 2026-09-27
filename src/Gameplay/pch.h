#ifndef PCH_H_DEFINED
#define PCH_H_DEFINED

#include <fstream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

//#include "Render/nlohmann.hpp"
//using json = nlohmann::json;

#include "Core/Console.h"

// Engine
#include "components.h"
#include "systems.h"
#include "Entity.h"
#include "SceneManager.h"
#include "Scene.h"
#include "RessourceManager.h"
#include "Tween.h"

//Components
#include "Components/CameraComponent.h"
#include "Components/MeshRenderer.h"
#include "Components/TransformComponent.h"


#include "Rooms/RoomManager.h"

//System
#include "Systems/CameraSystem.h"
#include "Systems/RenderSystem.h"
#include "Systems/InputSystem.h"

#include "Systems/LifeTimeSystem.h"

// Render
#include "Render/Generic/Render.h"

//StateMachine
#include "StateMachine/PlayerStateMachine.h"
#include "StateMachine/DoorStateMachine.h"
#include "StateMachine/TrapStateMachine.h"
#include "StateMachine/LeverStateMachine.h"

// Anomalies
#include "Anomalies/AnomalyBase.h"

#endif
