#pragma once
#include <EnginePch.h>
#include <Graphics/Scene.h>

#define GAMEOBJECT_CONST_INDEX_WATER 0
#define GAMEOBJECT_CONST_INDEX_DOOR 1
#define GAMEOBJECT_CONST_INDEX_SMOKE 2
#define GAMEOBJECT_CONST_INDEX_FIRE 3
#define GAMEOBJECT_CONST_INDEX_FLOOR 4

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
	bool WaterInlet;
	bool AlreadyFlooded;
	bool Burning;

	uint32 NrOfBaseGameObjects;
	
	std::vector<GameObject*> GameObjects;

	TileData()
	{
		GameObjects.push_back(nullptr);
		GameObjects.push_back(nullptr);
		GameObjects.push_back(nullptr);
		GameObjects.push_back(nullptr);
		GameObjects.push_back(nullptr);

		NrOfBaseGameObjects = GameObjects.size();
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

	bool HasFloor() const noexcept
	{
		return GameObjects[GAMEOBJECT_CONST_INDEX_FLOOR];
	}
};

class API WorldLevel
{
	friend class WorldSerializer;
	friend class Editor;

public:
	WorldLevel& operator=(WorldLevel&& other) = delete;
	WorldLevel& operator=(const WorldLevel& other) = delete;

	WorldLevel(WorldLevel&& other);
	WorldLevel(const WorldLevel& other);
	WorldLevel(uint32 levelHeight, const uint32* const levelIndexes, uint32 sizeX, uint32 sizeZ) noexcept;
	~WorldLevel();

	const uint32* const* const GetLevel() const noexcept;
	const TileData* const* const GetLevelData() const noexcept;
	TileData* const* const GetLevelData() noexcept;
	uint32 GetSizeX() const noexcept;
	uint32 GetSizeZ() const noexcept;

	///<summary>Returns a vec4. The first two values are the position of the wall (x, z), the second two are the dimensions of the wall (x, z)</summary>
	const glm::vec4& GetWall(uint32 index) const noexcept;
	uint32 GetNrOfWalls() const noexcept;
	const glm::vec4& GetBulkhead(uint32 index) const noexcept;
	uint32 GetNrOfBulkheads() const noexcept;
	uint32 GetTilesBetweenBulkheads() const noexcept;

	std::vector<glm::uvec4> GenerateRooms(uint32 tilesBetweenBulkheads = 8);
	void GenerateScenarioObjects(Scene* pScene, uint32 levelHeight);

private:
	uint32 m_SizeX;
	uint32 m_SizeZ;
	uint32 m_TilesBetweenBulkheads;
	TileData** m_ppLevelData;
	uint32** m_ppLevel;
	std::vector<glm::vec4> m_Walls;
	std::vector<glm::vec4> m_Bulkheads;
};