#pragma once
#include "SceneInternal.h"
#include <Graphics/GUI/TextView.h>
#include <Graphics/GUI/Panel.h>
#include <Graphics/GUI/ListScrollable.h>
#include "../GUI/UICrew.h"
#include "../GUI/UICrewMember.h"
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

class SceneGame : public SceneInternal, public ILogListener
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
	virtual void OnLogged(const std::string& text) noexcept override;

	void PickPosition();
	void PickCrew(bool hover);
	glm::vec3 GetRay(const glm::vec2& mousepos, uint32 windowWidth, uint32 windowHeight);
	void ShowCrewmember(uint32 crewmember);
	Crewmember* RayTestCrewmembers();
	Crewmember* GetCrewmember(uint32 shipNumber);
	UICrewMember* GetUICrewMember() noexcept;

protected:
	SceneGame();

	void CreateAudio() noexcept;
	void CreateGameObjects() noexcept;
	void CreateWorld() noexcept;
	void CreateCrew() noexcept;

	void UpdateCamera(float dtS) noexcept;

private:
	UICrewMember* m_pUICrewMember;
	World* m_pWorld;
	UICrew* m_pUICrew;
	ListScrollable* m_ListScrollableLog;
	TextView* m_pTextViewLog;
	Panel* m_PanelLog;
	Crew m_Crew;
	bool cartesianCamera;
	int32 m_CurrentElevation;
	std::vector<uint32> m_ActiveRooms;
	std::vector<float> m_RoomLightsTimers;
	uint32 m_CurrentLight = 0;
	AudioSource* m_pTestAudioSource;
};
