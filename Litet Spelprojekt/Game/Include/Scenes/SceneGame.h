#pragma once
#include "SceneInternal.h"
#include "../GUI/UICrew.h"
#include "../GUI/UILog.h"
#include "../GUI/UICrewMember.h"
#include "../GUI/UIPause.h"
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

class SceneGame : public SceneInternal
{
	friend class Game;

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

	void GenerateShadows();
	void PickPosition();
	void RequestDoorClosed(uint32 doorColor);
	void Pick(bool hover, int32 positionX, int32 positionY);
	glm::vec3 GetRay(const glm::vec2& mousepos, uint32 windowWidth, uint32 windowHeight);
	void ShowCrewmember(uint32 crewmember);
	GameObject* RayTestGameObjects();

	Crewmember* GetCrewmember(uint32 shipNumber);
	Crew* GetCrew() noexcept;
	UICrewMember* GetUICrewMember() noexcept;
	World* GetWorld() noexcept;

	void SetPaused(bool paused) noexcept;
	bool IsPaused() const noexcept;

protected:
	SceneGame();

	void CreateAudio() noexcept;
	void CreateGameObjects() noexcept;
	void CreateWorld() noexcept;
	void CreateCrew() noexcept;

	void UpdateCamera(float dtS) noexcept;
	void UpdateMaterialClipPlanes() noexcept;

private:
	bool m_IsPaused;
	bool m_CartesianCamera;
	
	int32 m_CurrentElevation;
	uint32 m_CurrentLight = 0;

	UICrewMember* m_pUICrewMember;
	UICrew* m_pUICrew;
	UILog* m_pUILog;
	UIPause* m_pUIPause;

	AudioSource* m_pTestAudioSource;
	World* m_pWorld;

	Crew m_Crew;
};

inline void SceneGame::UpdateMaterialClipPlanes() noexcept
{
	Camera& camera = GetCamera();
	float extendedFactor = 1.0f - (float)(IsExtended() || IsExtending());
	glm::vec4 standardClipPlane(0.0f, extendedFactor * -1.0f, 0.0f, extendedFactor * (camera.GetLookAt().y + 2.0f));
	glm::vec4 wallClipPlane(0.0f, extendedFactor * -1.0f, 0.0f, extendedFactor * (camera.GetLookAt().y + 1.99f));
	glm::vec4 floorClipPlane(0.0f, extendedFactor * -1.0f, 0.0f, extendedFactor * (camera.GetLookAt().y + 1.9f));

	ResourceHandler::GetMaterial(MATERIAL::BLACK)				->SetLevelClipPlane(standardClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::WHITE)				->SetLevelClipPlane(standardClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::DOOR_RED)			->SetLevelClipPlane(standardClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::DOOR_GREEN)			->SetLevelClipPlane(standardClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::DOOR_BLUE)			->SetLevelClipPlane(standardClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::DOOR_YELLOW)			->SetLevelClipPlane(standardClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::OCEAN_BLUE)			->SetLevelClipPlane(standardClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::WATER_OUTDOOR)		->SetLevelClipPlane(standardClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::WATER_INDOOR)		->SetLevelClipPlane(standardClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::WALL_STANDARD)		->SetLevelClipPlane(wallClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::BULKHEADS_STANDARD)	->SetLevelClipPlane(wallClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::CREW_STANDARD)		->SetLevelClipPlane(standardClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::SINGLE_BED)			->SetLevelClipPlane(standardClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::BUNK_BED)			->SetLevelClipPlane(standardClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::INSTRUMENT_1)		->SetLevelClipPlane(standardClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::INSTRUMENT_2)		->SetLevelClipPlane(standardClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::LAMP)				->SetLevelClipPlane(standardClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::CHAIR)				->SetLevelClipPlane(standardClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::CUPBOARD)			->SetLevelClipPlane(standardClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::TABLE)				->SetLevelClipPlane(standardClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::TOILET)				->SetLevelClipPlane(standardClipPlane);
	ResourceHandler::GetMaterial(MATERIAL::ANIMATED_MODEL)		->SetLevelClipPlane(standardClipPlane);
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
	ResourceHandler::GetMaterial(MATERIAL::OVEN)				->SetLevelClipPlane(standardClipPlane);

	IRenderer* renderer = GetRenderer();
	if (renderer != nullptr)
	{
		renderer->SetParticleClipPlane(standardClipPlane);
	}
}