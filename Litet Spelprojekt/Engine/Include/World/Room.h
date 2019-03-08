#pragma once
#include <EnginePch.h>
#include <Graphics/Textures/StaticShadowCube.h>

class API Room
{
public:
	Room(const Room& other) = delete;
	Room& operator=(Room&& other) = delete;
	Room& operator=(const Room& other) = delete;

	Room(Room&& other);
	Room() noexcept;
	~Room();

	void Init(const glm::vec3& center) noexcept;

	void SetActive(bool active) noexcept;
	void SetBurning(bool burning) noexcept;
	void SetFireDetected(bool detected) noexcept;
	void SetFlooded(bool flooded) noexcept;
	void SetFloodDetected(bool detected) noexcept;
	void SetFloodUpdated(bool updated) noexcept;
	void GenerateShadows(const Scene& scene) noexcept;

	const StaticShadowCube* GetShadowMap() const noexcept;
	const glm::vec3& GetCenter() const noexcept;
	bool IsActive() const noexcept;
	bool IsBurning() const noexcept;
	bool IsFireDetected() const noexcept;
	bool IsFlooded() const noexcept;
	bool IsFloodDetected() const noexcept;
	bool IsFloodUpdatedThisFrame() const noexcept;
	bool IsRoomInitialized() const noexcept;

private:
	glm::vec3 m_Center;
	bool m_Active = false;
	bool m_Burning = false;
	bool m_FireDetected = false;
	bool m_Flooded = false;
	bool m_FloodDetected = false;
	bool m_FloodUpdatedThisFrame = false;
	bool m_Initialized = false;
	StaticShadowCube* m_pShadowMap;
};

inline const StaticShadowCube* Room::GetShadowMap() const noexcept
{
	return m_pShadowMap;
}

inline const glm::vec3& Room::GetCenter() const noexcept
{
	return m_Center;
}

inline bool Room::IsActive() const noexcept
{
	return m_Active;
}

inline bool Room::IsBurning() const noexcept
{
	return m_Burning;
}

inline bool Room::IsFireDetected() const noexcept
{
	return m_FireDetected;
}

inline bool Room::IsFlooded() const noexcept
{
	return m_Flooded;
}

inline bool Room::IsFloodDetected() const noexcept
{
	return m_FloodDetected;
}

inline bool Room::IsFloodUpdatedThisFrame() const noexcept
{
	return m_FloodUpdatedThisFrame;
}

inline bool Room::IsRoomInitialized() const noexcept
{
	return m_Initialized;
}
