#pragma once
#include <EnginePch.h>
#include <Graphics/Scene.h>
#include <World/Scenarios/Water/WaterObject.h>

#define GAMEOBJECT_CONST_INDEX_WATER 0
#define GAMEOBJECT_CONST_INDEX_DOOR 1

struct TileData 
{
	bool HasStairs;

	float Temp;
	float BurnsAt;
	float SmokeAmount;
	float SmokeLimit;

	float WaterLevel;
	float WaterLevelChange;
	float WaterLevelLastUpdated;
	float WaterLevelAge;
	bool AlreadyFlooded;
	bool Burning;

	uint32 nrOfBaseGameObjects;
	
	std::vector<GameObject*> GameObjects;

	TileData()
	{
		GameObjects.push_back(nullptr);
		GameObjects.push_back(nullptr);
		GameObjects.push_back(nullptr);
		GameObjects.push_back(nullptr);

		nrOfBaseGameObjects = GameObjects.size();
	}

	void RemoveGameObject(GameObject* gameObject) noexcept
	{
		for (int i = 2; i < GameObjects.size(); i++)
		{
			if (GameObjects[i] == gameObject)
			{
				GameObjects.erase(GameObjects.begin() + i);
				return;
			}
		}
	}

	bool HasDoor() const noexcept
	{
		return GameObjects[GAMEOBJECT_CONST_INDEX_DOOR];
	}
};

class API WorldLevel
{
	friend class WorldSerializer;
	friend class Editor;

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
	const glm::vec4& GetBulkhead(uint32 index) const noexcept;
	uint32 GetNrOfBulkheads() const noexcept;
	const std::vector<glm::uvec4>& GetRooms() const noexcept;

	void GenerateRooms(uint32 tilesBetweenBulkheads = 8);
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
	std::vector<glm::vec4> m_Bulkheads;

	std::vector<glm::uvec4> m_RoomBounds;
};