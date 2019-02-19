#pragma once

#include <System/Application.h>
#include <Graphics/GameObject.h>
#include <Graphics/Buffers/UniformBuffer.h>
#include <Graphics/Camera.h>
#include <Graphics/Scene.h>
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
#include <Graphics/GUI/ListScrollable.h>

#include <Graphics/Materials/WallMaterial.h>
#include <Graphics/Materials/WaterMaterial.h>

#include <World/Logger.h>
#include <World/Scenarios/ScenarioManager.h>
#include "../Include/Scenarios/ScenarioWater.h"
#include "../Include/Scenarios/ScenarioFire.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtx/string_cast.hpp>
#include <math.h>

#define NUM_CREW 15
#define MAX_ROOMS_VISIBLE 3

class Game : public Application, public ILogListener
{
public:
	Game() noexcept;
	~Game();

	void OnLogged(const std::string& text) noexcept override;

	void OnResourceLoading(const std::string&, float percentage) override;
	void OnResourcesLoaded() override;
	void OnUpdateLoading(float dtS) override;
	void OnRenderLoading(float dtS) override;

	void OnKeyUp(KEY keycode) override;
	void OnKeyDown(KEY keycode) override;
	void OnMouseMove(const glm::vec2& lastPosition, const glm::vec2& position) override;
	void OnMouseReleased(MouseButton mousebutton, const glm::vec2& position) override;
	void OnMouseScroll(const glm::vec2& offset, const glm::vec2& position) override;
	void OnUpdate(float dtS) override;
	void OnRender(float dtS) override;
	void PickPosition();
	void PickCrew(bool hover);
	glm::vec3 GetRay(const glm::vec2& mousepos, uint32 windowWidth, uint32 windowHeight);

	void ShowCrewmember(uint32 crewmember);

	Crewmember* RayTestCrewmembers();

	void SetClipPlanes(uint32 scene);

	Crewmember* GetCrewmember(uint32 shipNumber);
	Scene* GetScene();
	static Game* GetGame();

	UICrewMember* m_pUICrewMember;

private:
	IRenderer* m_pRenderer;
	DebugRenderer* m_pDebugRenderer;
	std::vector<Scene*> m_Scenes;
	uint32 m_SceneId = 0;
	World* m_pWorld;

	TextureCube* m_pSkyBoxTex;
	UICrew* m_pUICrew;

	TextView* m_pTextViewScene;
	TextView* m_pTextViewFile;
	ProgressBar* m_pLoadingBar;

	ListScrollable* m_ListScrollableLog;
	TextView* m_pTextViewLog;
	Panel* m_PanelLog;
	
	Crew m_Crew;

	bool cartesianCamera;
	int32 m_CurrentElevation;

	std::vector<uint32> m_ActiveRooms;
	std::vector<float> m_RoomLightsTimers;
	float m_DoorLightTimer = 0.0f;
	uint32 m_CurrentLight = 0;

	//Sound
	AudioSource* m_pTestAudioSource;
};