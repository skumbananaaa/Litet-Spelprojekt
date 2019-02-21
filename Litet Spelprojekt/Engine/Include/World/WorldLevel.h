#pragma once
#include <EnginePch.h>
#include <Graphics/Scene.h>
#include <World/Scenarios/Water/WaterObject.h>

struct TileData 
{
	bool HasStairs;
	bool HasDoor;

	float Temp;
	float BurnsAt;
	float SmokeAmount;
	float SmokeLimit;

	float WaterLevel;
	float WaterLevelChange;
	float WaterLevelLastUpdated;
	float WaterLevelAge;
	bool AlreadyFlooded;
	
	std::vector<GameObject*> GameObjects;
};

class API WorldLevel
{
public:
	WorldLevel(WorldLevel&& other) = delete;
	WorldLevel(const WorldLevel& other) = delete;
	WorldLevel& operator=(WorldLevel&& other) = delete;
	WorldLevel& operator=(const WorldLevel& other) = delete;

	WorldLevel(uint32 levelHeight, const uint32* const levelIndexes, uint32 sizeX, uint32 sizeZ) noexcept;
	~WorldLevel();

	const uint32* const* const GetLevel() const noexcept;
	const TileData* const* const GetLevelData() const noexcept;
	TileData* const* const GetLevelData() noexcept;
	std::vector<glm::ivec2>& GetBurningIDs() noexcept;
	std::vector<glm::ivec2>& GetFloodingIDs() noexcept;
	uint32 GetSizeX() const noexcept;
	uint32 GetSizeZ() const noexcept;

	///<summary>Returns a vec4. The first two values are the position of the wall (x, z), the second two are the dimensions of the wall (x, z)</summary>
	const glm::vec4& GetWall(uint32 index) const noexcept;
	uint32 GetNrOfWalls() const noexcept;
	const std::vector<glm::uvec4>& GetRooms() const noexcept;

	void GenerateRooms();
	void GenerateWater(Scene* pScene, uint32 levelHeight);
	void UpdateFire(float dt);
	void UpdateSmoke(float dt, const TileData* const* fireLevel, WorldLevel* aboveLevel);

private:
	TileData** m_ppLevelData;
	uint32** m_ppLevel;
	std::vector<glm::ivec2> m_BurningIDs;
	std::vector<glm::ivec2> m_FloodingIDs;
	uint32 m_SizeX;
	uint32 m_SizeZ;
	uint32 m_NrOfWalls;
	std::vector<glm::vec4> m_Walls;

	std::vector<glm::uvec4> m_RoomBounds;
};