#pragma once
#include <EnginePch.h>
#include <Graphics/Textures/StaticShadowCube.h>
#include <Audio/Sources/AudioSource.h>

class API Room
{
public:
	Room(const Room& other) = delete;
	Room& operator=(Room&& other) = delete;
	Room& operator=(const Room& other) = delete;

	Room(Room&& other);
	Room() noexcept;
	Room(glm::vec3 center) noexcept;
	~Room();

	const StaticShadowCube* GetShadowMap() const noexcept;
	const glm::vec3& GetCenter() const noexcept;
	const bool IsActive() const noexcept;
	const bool IsBurning() const noexcept;
	const bool IsFireDetected() const noexcept;
	const bool IsFlooded() const noexcept;
	const bool IsFloodDetected() const noexcept;
	const bool IsFloodUpdatedThisFrame() const noexcept;

	void SetActive(bool active) noexcept;
	void SetBurning(bool burning) noexcept;
	void SetFireDetected(bool detected) noexcept;
	void SetFlooded(bool flooded) noexcept;
	void SetFloodDetected(bool detected) noexcept;
	void SetFloodUpdated(bool updated) noexcept;
	void GenerateShadows(const Scene& scene) noexcept;

private:
	glm::vec3 m_Center;
	bool m_Active = false;
	bool m_Burning = false;
	bool m_FireDetected = false;
	bool m_Flooded = false;
	bool m_FloodDetected = false;
	bool m_FloodUpdatedThisFrame = false;
	StaticShadowCube* m_pShadowMap;
	AudioSource* m_pAudioSourceFire;
};