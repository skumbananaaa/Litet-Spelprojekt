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


#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <math.h>

#define NUM_CREW 15

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
	void OnUpdate(float dtS) override;
	void OnRender(float dtS) override;
	void PickPosition();
	void PickCrew();
	glm::vec3 GetRay(const glm::vec2& mousepos, uint32 windowWidth, uint32 windowHeight);

private:
	IRenderer* m_pRenderer;
	DebugRenderer* m_pDebugRenderer;
	Scene* m_pScene;

	TextureCube* m_pSkyBoxTex;
	World* m_pWorld;

	TextView* m_pTextViewFPS;
	TextView* m_pTextViewUPS;
	TextView* m_pTextViewCrew;
	ProgressBar* m_pLoadingBar;
	TextView* m_pTextViewFile;
	
	Crew m_Crew;
	std::string m_CrewList[NUM_CREW];

	bool cartesianCamera;

	//Sound
	AudioSource* m_pTestAudioSource;
};