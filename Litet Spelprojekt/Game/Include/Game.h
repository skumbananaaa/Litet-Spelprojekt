#pragma once

#include <System/Application.h>
#include <Graphics/GameObject.h>
#include <World/GameObjectDoor.h>
#include <Graphics/Buffers/UniformBuffer.h>
#include <Graphics/Camera.h>
#include <Graphics/Renderers/IRenderer.h>
#include <Graphics/Textures/Framebuffer.h>
#include <Graphics/Textures/Texture2D.h>
#include <Graphics/Textures/TextureCube.h>
#include <Graphics/GUI/TextView.h>
#include <Graphics/Materials/Decal.h>
#include <Audio/Listeners/AudioListener.h>
#include <Audio/Sources/AudioSource.h>
#include <Audio/SoundEffect.h>
#include <Audio/Music.h>
#include <IO/WorldSerializer.h>
#include <IO/ResourceHandler.h>
#include "..\Include\Crew.h"
#include "..\Include\Path.h"
#include <Graphics/GUI/ProgressBar.h>
#include "GUI/UICrew.h"
#include "GUI/UICrewMember.h"
#include "GUI/UIScenario.h"
#include <Graphics/GUI/ListScrollable.h>

#include <Graphics/Materials/WallMaterial.h>
#include <Graphics/Materials/WaterOutdoorMaterial.h>
#include <Graphics/Materials/WaterIndoorMaterial.h>

#include "../Include/Scenarios/ScenarioManager.h"
#include "../Include/Scenarios/ScenarioWater.h"
#include "../Include/Scenarios/ScenarioFire.h"

#include "Scenes/SceneInternal.h"

#if defined(PRINT_CPU_DEBUG_DATA)
#include <System/CPUProfiler.h>
#endif

#define NUM_CREW 15
#define MAX_ROOMS_VISIBLE 3

class Game : public Application
{
public:
	Game() noexcept;
	~Game();

	void OnResourceLoading(const std::string&, float percentage) override;
	void OnResourcesLoaded() override;

	void OnKeyUp(KEY keycode) override;
	void OnKeyDown(KEY keycode) override;
	void OnMouseMove(const glm::vec2& lastPosition, const glm::vec2& position) override;
	void OnMouseReleased(MouseButton mousebutton, const glm::vec2& position) override;
	void OnMouseScroll(const glm::vec2& offset, const glm::vec2& position) override;
	void OnUpdate(float dtS) override;
	void OnRender(float dtS) override;
	
	UIScenario* GetUIScenario() noexcept;

	void SetScene(SceneInternal* scene) noexcept;

	static Game* GetGame();

private:
	SceneInternal* m_pScene;
	SceneInternal* m_pSceneLast;
	

	UIScenario* m_pUIScenario;
};