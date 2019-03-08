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
