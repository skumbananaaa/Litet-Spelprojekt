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


#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <math.h>

class Game : public Application, public IResourceListener
{
public:
	Game() noexcept;
	~Game();

	void OnResourcesLoaded() override;

	void OnKeyUp(KEY keycode) override;
	void OnKeyDown(KEY keycode) override;
	void OnMouseMove(const glm::vec2& position) override;
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
	
	Crew m_Crew;
	uint32 m_CurrentCrewmember;

	bool cartesianCamera;

	//Sound
	SoundEffect* m_pSoundEffect;
	Music* m_pMusic;
	AudioSource* m_pTestAudioSource;
};