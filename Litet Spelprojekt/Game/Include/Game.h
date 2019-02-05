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
	void OnUpdate(float dtS) override;
	void OnRender(float dtS) override;
	void PickingTest();

private:
	IRenderer* m_pRenderer;
	DebugRenderer* m_pDebugRenderer;
	Scene* m_pScene;
	Scene* m_pInstancingTestScene;

	TextureCube* m_pSkyBoxTex;
	World* m_pWorld;

	TextView* m_pTextViewFPS;
	TextView* m_pTextViewUPS;
	
	bool cartesianCamera;

	//Sound
	SoundEffect* m_pSoundEffect;
	Music* m_pMusic;
	AudioSource* m_pTestAudioSource;
};