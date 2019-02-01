#pragma once

#include <System/Application.h>
#include <Graphics/GameObject.h>
#include <Graphics/Buffers/UniformBuffer.h>
#include <Graphics/Camera.h>
#include <Graphics/Scene.h>
#include <Graphics/Renderers/IRenderer.h>
#include <Graphics/Textures/Framebuffer.h>
#include <Graphics/Textures/Texture2D.h>
#include <Graphics/GUI/TextView.h>
#include <Graphics/Materials/Decal.h>
#include <Audio/Listeners/AudioListener.h>
#include <Audio/Sources/AudioSource.h>
#include <Audio/SoundEffect.h>
#include <Audio/Music.h>
#include <IO/WorldSerializer.h>

#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <math.h>

class Game : public Application
{
public:
	Game() noexcept;
	~Game();

	void OnKeyUp(KEY keycode) override;
	void OnKeyDown(KEY keycode) override;
	void OnMouseMove(const glm::vec2& position) override;
	void OnUpdate(float dtS) override;
	void OnRender(float dtS) override;

private:
	FontRenderer* m_pFontRenderer;
	IRenderer* m_pRenderer;
	DebugRenderer* m_pDebugRenderer;
	Scene* m_pScene;
	Scene* m_pInstancingTestScene;

	World* m_pWorld;

	IndexedMesh* m_pTestMesh;
	IndexedMesh* m_pSphereMesh;
	IndexedMesh* m_pWaterMesh;
	IndexedMesh* m_pGroundTestMesh;
	IndexedMesh* m_pWallMesh;

	Texture2D* m_pBoatTexture;
	Texture2D* m_pBoatNormalMap;
	Texture2D* m_pBloodTexture;
	Texture2D* m_pBloodNormal;

	Decal* m_pDecal;

	Material* m_pBoatMaterial;
	Material* m_pGroundMaterial;
	Material* m_pRedMaterial;
	Material* m_pGreenMaterial;
	Material* m_pBlueMaterial;

	TextView* m_pTextViewFPS;
	TextView* m_pTextViewUPS;
	
	bool cartesianCamera;

	//Sound
	SoundEffect* m_pSoundEffect;
	Music* m_pMusic;
	AudioSource* m_pTestAudioSource;
};