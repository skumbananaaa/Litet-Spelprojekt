#pragma once
#include <EnginePch.h>
#include <Graphics/Textures/StaticShadowCube.h>
#include <Audio/Sources/AudioSource.h>
#include <World/WorldLevel.h>

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
	void SetFireDetected(bool detected) noexcept;
	void SetFlooded(bool flooded) noexcept;
	void SetFloodDetected(bool detected) noexcept;
	void SetFloodUpdated(bool updated) noexcept;
	void SetPumping(bool pumping) noexcept;

	void GenerateShadows(const Scene& scene) noexcept;
	void ExtendAudioPos(float extension) noexcept;

	void SetTileOnFire(const glm::ivec3& tile) noexcept;
	void RemoveTileOnFire(const glm::ivec3& tile) noexcept;

	const StaticShadowCube* GetShadowMap() const noexcept;
	const glm::vec3& GetCenter() const noexcept;
	bool IsActive() const noexcept;
	bool IsBurning() const noexcept;
	bool IsFireDetected() const noexcept;
	bool IsFlooded() const noexcept;
	bool IsFloodDetected() const noexcept;
	bool IsFloodUpdatedThisFrame() const noexcept;
	bool IsRoomInitialized() const noexcept;
	bool IsPumping() const noexcept;

	void AddTile(glm::ivec3 tile, TileData* data) noexcept;
	const std::vector<glm::ivec3> GetRoomTiles() const noexcept;
	std::vector<TileData*>* GetTileData() noexcept;

private:
	glm::vec3 m_Center;
	bool m_Active = false;
	bool m_FireDetected = false;
	bool m_Flooded = false;
	bool m_FloodDetected = false;
	bool m_FloodUpdatedThisFrame = false;
	bool m_Initialized = false;
	bool m_Pumping = false;
	StaticShadowCube* m_pShadowMap;
	AudioSource* m_pAudioSourceFire;
	std::vector<glm::ivec3> m_Tiles;
	std::vector<TileData*> m_TileData;
	std::vector<glm::ivec3> m_OnFire;
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
	return !m_OnFire.empty();
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

inline bool Room::IsPumping() const noexcept
{
	return m_Pumping;
}

inline void Room::AddTile(glm::ivec3 tile, TileData* data) noexcept
{
	m_Tiles.push_back(tile);
	m_TileData.push_back(data);
}

inline const std::vector<glm::ivec3> Room::GetRoomTiles() const noexcept
{
	return m_Tiles;
}

inline std::vector<TileData*>* Room::GetTileData() noexcept
{
	return &m_TileData;
}

