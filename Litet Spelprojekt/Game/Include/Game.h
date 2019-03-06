#pragma once
#include <System/Application.h>
#include "Scenes/SceneInternal.h"
#include "Scenes/SceneGame.h"
#include "Scenes/SceneMenu.h"
#include "Scenes/SceneLoading.h"
#include "Scenes/SceneCredits.h"
#include "Scenes/SceneOptions.h"
#include "Scenes/SceneScenario.h"
#include <Graphics/Renderers/ForwardRenderer.h>
#include <Audio/Sources/AudioSource.h>
#include <IO/ResourceHandler.h>

#if defined(PRINT_CPU_DEBUG_DATA)
#include <System/CPUProfiler.h>
#endif

#define NUM_CREW 15

class Game : public Application, public IGameObjectCreator
{
public:
	Game() noexcept;
	~Game();

	virtual GameObject* CreateGameObject(uint32 gameobject) noexcept override;

	void OnResourceLoading(const std::string&, float percentage) override;
	void OnResourcesLoaded() override;

	void OnKeyUp(KEY keycode) override;
	void OnKeyDown(KEY keycode) override;
	void OnMouseMove(const glm::vec2& lastPosition, const glm::vec2& position) override;
	void OnMouseReleased(MouseButton mousebutton, const glm::vec2& position) override;
	void OnMouseScroll(const glm::vec2& offset, const glm::vec2& position) override;
	void OnUpdate(float dtS) override;
	void OnRender(float dtS) override;
	
	void SetScene(SceneInternal* scene) noexcept;

	static Game* GetGame();

public:
	SceneLoading* m_pSceneLoading;
	SceneMenu* m_pSceneMenu;
	SceneCredits* m_pSceneCredits;
	SceneOptions* m_pSceneOptions;
	SceneScenario* m_pSceneScenario;
	SceneGame* m_pSceneGame;

	int32 m_ScenarioFire;
	int32 m_ScenarioWater;
	int32 m_ScenarioMissile;
	int32 m_ScenarioTorpedo;

private:
	SceneInternal* m_pScene;
	SceneInternal* m_pSceneNext;

	TextureCube* m_pSkyBoxTex;
	SkyBox* m_pSkyBox;
	IRenderer* m_pRenderer;
	AudioSource* m_pAudioSourceMenu;
};