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
#include "GUI/UICrewMember.h"

#include <Graphics/Materials/WallMaterial.h>
#include <Graphics/Materials/WaterMaterial.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <GLM/gtx/string_cast.hpp>
#include <math.h>

#define NUM_CREW 115

class Game : public Application
{
public:
	Game() noexcept;
	~Game();

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
	void PickCrew();
	glm::vec3 GetRay(const glm::vec2& mousepos, uint32 windowWidth, uint32 windowHeight);

	void SetClipPlanes(uint32 scene);

	static Scene* GetScene();
	static Game* GetGame();

	UICrewMember* m_pUICrewMember;

private:
	IRenderer* m_pRenderer;
	DebugRenderer* m_pDebugRenderer;
	std::vector<Scene*> m_Scenes;
	uint32 m_SceneId = 0;

	TextureCube* m_pSkyBoxTex;

	TextView* m_pTextViewFPS;
	TextView* m_pTextViewUPS;
	TextView* m_pTextViewScene;
	TextView* m_pTextViewFile;
	ProgressBar* m_pLoadingBar;
	
	Crew m_Crew;
	std::string m_CrewList[NUM_CREW];

	bool cartesianCamera;
	int32 m_CurrentElevation;

	//Sound
	AudioSource* m_pTestAudioSource;
};