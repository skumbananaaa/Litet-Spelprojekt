#pragma once
#include "SceneInternal.h"
#include "../GUI/UICrew.h"
#include "../GUI/UILog.h"
#include "../GUI/UICrewMember.h"
#include "../GUI/UIPause.h"
#include "../GUI/UIEndScreen.h"
#include "../GUI/UICrewRequest.h"
#include <Audio/Listeners/AudioListener.h>
#include <Audio/Sources/AudioSource.h>
#include <Audio/SoundEffect.h>
#include <Audio/Music.h>
#include <IO/WorldSerializer.h>
#include <IO/ResourceHandler.h>
#include "../Crew.h"
#include "../Path.h"
#include <Graphics/Materials/WallMaterial.h>
#include <Graphics/Materials/WaterOutdoorMaterial.h>
#include <Graphics/Materials/WaterIndoorMaterial.h>
#include <World/Logger.h>
#include <System/Random.h>
#include "../GUI/UINotification.h"

#define MIN_CREWHEALTH 0.4f
#define MAX_SHIPDAMAGE 0.3f
#define MAX_WATERLEAKAGE 0.5f

class SceneGame : public SceneInternal
{
	friend class Game;
	friend class SceneInstructions;

public:
	virtual ~SceneGame();

	virtual void OnActivated(SceneInternal* lastScene, IRenderer* m_pRenderer) noexcept override;
	virtual void OnDeactivated(SceneInternal* newScene) noexcept override;
	virtual void OnUpdate(float dtS) noexcept override;
	virtual void OnRender(float dtS) noexcept override;
	virtual void OnMouseMove(const glm::vec2& lastPosition, const glm::vec2& position) override;
	virtual void OnMouseScroll(const glm::vec2& offset, const glm::vec2& position) override;
	virtual void OnMousePressed(MouseButton mousebutton, const glm::vec2& position) override;
	virtual void OnMouseReleased(MouseButton mousebutton, const glm::vec2& position) override;
	virtual void OnKeyUp(KEY keycode) override;
	virtual void OnKeyDown(KEY keycode) override;
	virtual void OnResize(uint32 width, uint32 height) override;
	virtual void OnSceneExtensionComplete() noexcept override;
	void OnGameOver() noexcept;

	void PickPosition();
	void RequestDoorClosed(uint32 doorColor);
	void SetUIVisible(bool visible) noexcept;

	void Pick(bool hover, int32 positionX, int32 positionY);
	glm::vec3 GetRay(const glm::vec2& mousepos, uint32 windowWidth, uint32 windowHeight);
	void ShowCrewmember(uint32 crewmember);
	GameObject* RayTestGameObjects();

	Crewmember* GetCrewmember(uint32 shipNumber);
	Crew* GetCrew() noexcept;
	UICrewMember* GetUICrewMember() noexcept;
	UINotification* GetUINotification() noexcept;
	World* GetWorld() noexcept;

	void SetPaused(bool paused) noexcept;
	bool IsPaused() const noexcept;

protected:
	SceneGame(World* pWorld);

	void CreateAudio() noexcept;
	void CreateGameObjects() noexcept;
	void CreateWorld() noexcept;
	void CreateCrew() noexcept;

	void UpdateCamera(float dtS) noexcept;
	void UpdateMaterialClipPlanes() noexcept;

private:
	bool m_IsPaused;
	bool m_IsGameOver;

	uint32 m_CurrentLight = 0;

	UICrewMember* m_pUICrewMember;
	UICrew* m_pUICrew;
	UILog* m_pUILog;
	UIPause* m_pUIPause;
	UICrewRequest* m_pUIRequest;
	UINotification* m_pUINotification;
	UIEndScreen* m_pUIEndScreen;

	AudioSource* m_pTestAudioSource;
	World* m_pWorld;

	Crew m_Crew;

	GameObject* m_pLookAt;
};

inline void SceneGame::UpdateMaterialClipPlanes() noexcept
{
	Camera& camera = GetCamera();
	float extendedFactor = 1.0f - (float)(IsExtended() || IsExtending());
	float levelClipFactor = glm::round(glm::clamp<float>(4.0f - camera.GetLookAt().y, 0.0f, 1.0f));
	float factorProduct = extendedFactor * levelClipFactor;
	glm::vec4 standardClipPlane(0.0f, factorProduct * -1.0f, 0.0f, factorProduct * (camera.GetLookAt().y + 2.0f));
	glm::vec4 wallClipPlane(0.0f,	  factorProduct * -1.0f, 0.0f, factorProduct * (camera.GetLookAt().y + 1.99f));
	glm::vec4 floorClipPlane(0.0f,	  factorProduct * -1.0f, 0.0f, factorProduct * (camera.GetLookAt().y + 1.9f));
	glm::vec4 particleClipPlane(0.0f, factorProduct * -1.0f, 0.0f, factorProduct * (camera.GetLookAt().y + 2.0f));

	ResourceHandler::GetMaterial(MATERIAL::BLACK)				->SetLevelClipPlane(standardClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::DOOR_FRAME)			->SetLevelClipPlane(standardClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::DOOR_RED)			->SetLevelClipPlane(standardClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::DOOR_GREEN)			->SetLevelClipPlane(standardClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::DOOR_BLUE)			->SetLevelClipPlane(standardClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::DOOR_YELLOW)			->SetLevelClipPlane(standardClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::FIRE_RELATED)		->SetLevelClipPlane(standardClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::LADDER)				->SetLevelClipPlane(standardClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::OCEAN_BLUE)			->SetLevelClipPlane(standardClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::WATER_OUTDOOR)		->SetLevelClipPlane(standardClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::WATER_INDOOR)		->SetLevelClipPlane(standardClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::WALL_STANDARD)		->SetLevelClipPlane(wallClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::BULKHEADS_STANDARD)	->SetLevelClipPlane(wallClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::CREW_STANDARD)		->SetLevelClipPlane(standardClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::SINGLE_BED)			->SetLevelClipPlane(standardClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::BUNK_BED)			->SetLevelClipPlane(standardClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::INSTRUMENT_1)		->SetLevelClipPlane(floorClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::INSTRUMENT_2)		->SetLevelClipPlane(floorClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::LAMP)				->SetLevelClipPlane(standardClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::CHAIR)				->SetLevelClipPlane(standardClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::CUPBOARD)			->SetLevelClipPlane(standardClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::TABLE)				->SetLevelClipPlane(floorClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::TOILET)				->SetLevelClipPlane(standardClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::ANIMATED_MODEL)		->SetLevelClipPlane(standardClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::CREW_INJURED)		->SetLevelClipPlane(standardClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::FLOOR_NORMAL)		->SetLevelClipPlane(floorClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::FLOOR_SICKBAY1)		->SetLevelClipPlane(floorClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::FLOOR_TOILET1)		->SetLevelClipPlane(floorClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::FLOOR_MACHINE1)		->SetLevelClipPlane(floorClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::FLOOR_MACHINE2)		->SetLevelClipPlane(floorClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::FLOOR_MACHINE3)		->SetLevelClipPlane(floorClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::FLOOR_AMMUNITION1)	->SetLevelClipPlane(floorClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::FLOOR_AMMUNITION2)	->SetLevelClipPlane(floorClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::FLOOR_AMMUNITION3)	->SetLevelClipPlane(floorClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::FLOOR_KITCHEN1)		->SetLevelClipPlane(floorClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::FLOOR_DINING1)		->SetLevelClipPlane(floorClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::FLOOR_CABOOSE1)		->SetLevelClipPlane(floorClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::FLOOR_EXT_NORMAL)	->SetLevelClipPlane(floorClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::FLOOR_EXT_SICKBAY1)	->SetLevelClipPlane(floorClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::FLOOR_EXT_TOILET1)	->SetLevelClipPlane(floorClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::FLOOR_EXT_MACHINE1)	->SetLevelClipPlane(floorClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::FLOOR_EXT_MACHINE2)	->SetLevelClipPlane(floorClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::FLOOR_EXT_MACHINE3)	->SetLevelClipPlane(floorClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::FLOOR_EXT_AMMUNITION1)->SetLevelClipPlane(floorClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::FLOOR_EXT_AMMUNITION2)->SetLevelClipPlane(floorClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::FLOOR_EXT_AMMUNITION3)->SetLevelClipPlane(floorClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::FLOOR_EXT_KITCHEN1)	->SetLevelClipPlane(floorClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::FLOOR_EXT_DINING1)	->SetLevelClipPlane(floorClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::FLOOR_EXT_CABOOSE1)	->SetLevelClipPlane(floorClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::OVEN)				->SetLevelClipPlane(standardClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::SHELF_AMMUNITION)	->SetLevelClipPlane(standardClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::SHELF_EMPTY)			->SetLevelClipPlane(standardClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::GENERATOR)			->SetLevelClipPlane(standardClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::FIRE_EXTINGUISHER)	->SetLevelClipPlane(standardClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::FIRESPRINKLER)		->SetLevelClipPlane(standardClipPlane);


	IRenderer* renderer = GetRenderer();
	if (renderer != nullptr)
	{
		renderer->SetParticleClipPlane(floorClipPlane);
	}
}