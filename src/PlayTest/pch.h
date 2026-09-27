#ifndef PCH_H_DEFINED
#define PCH_H_DEFINED

#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>

#include "Core/Console.h"

//// Engine
#include "Engine/components.h"
#include "Engine/systems.h"
#include "Engine/Entity.h"
#include "Engine/SceneManager.h"
#include "Engine/Scene.h"
#include "Engine/RessourceManager.h"

// Render
#include "Render/Generic/Render.h"

//Components
#include "Components/CameraComponent.h"
#include "Components/MeshRenderer.h"
#include "Components/StateMachineComponent.h"
#include "Components/UIButtonComponent.h"

//System
#include "Systems/CameraSystem.h"
#include "Systems/RenderSystem.h"
#include "Systems/InputSystem.h"
#include "Systems/UIButtonSystem.h"

//Audio
#include "AudioEngine/AudioEngine.h"

#endif