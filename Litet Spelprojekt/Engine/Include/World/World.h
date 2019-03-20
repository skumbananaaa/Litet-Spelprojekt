#pragma once
#include <EnginePch.h>
#include <World/WorldLevel.h>
#include <World/Room.h>
#include <Graphics/Scene.h>
#include <System/Input.h>
#include <System/Random.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <GLM/gtx/string_cast.hpp>

enum DOOR_COLOR : uint32
{
	DOOR_COLOR_RED,
	DOOR_COLOR_GREEN,
	DOOR_COLOR_BLUE,
	DOOR_COLOR_YELLOW
};

enum ReservedTileIndexes : uint32
{
	SICKBAY_INTERVAL_END			= MAX_NUM_ROOMS - 1,
	SICKBAY_0						= MAX_NUM_ROOMS - 1,
	SICKBAY_1						= MAX_NUM_ROOMS - 2,
	SICKBAY_INTERVAL_START			= MAX_NUM_ROOMS - 2,
	NUM_SICKBAYS					= 2,
	SICKBAY_CATEGORY_INDEX			= 6,
	SICKBAY_DOOR_COLOR				= DOOR_COLOR_YELLOW,

	TOILET_INTERVAL_END				= MAX_NUM_ROOMS - 3,
	TOILET_0						= MAX_NUM_ROOMS - 3,
	TOILET_1						= MAX_NUM_ROOMS - 4,
	TOILET_2						= MAX_NUM_ROOMS - 5,
	TOILET_3						= MAX_NUM_ROOMS - 6,
	TOILET_INTERVAL_START			= MAX_NUM_ROOMS - 6,
	NUM_TOILETS						= 4,
	TOILET_CATEGORY_INDEX			= 5,
	TOILET_DOOR_COLOR				= DOOR_COLOR_RED,

	MACHINE_ROOM_INTERVAL_END		= MAX_NUM_ROOMS - 7,
	MACHINE_ROOM_0					= MAX_NUM_ROOMS - 7,
	MACHINE_ROOM_1					= MAX_NUM_ROOMS - 8,
	MACHINE_ROOM_INTERVAL_START		= MAX_NUM_ROOMS - 8,
	NUM_MACHINE_ROOMS				= 2,
	MACHINE_ROOM_CATEGORY_INDEX		= 4,
	MACHINE_DOOR_COLOR				= DOOR_COLOR_RED,

	AMMUNITION_ROOM_INTERVAL_END	= MAX_NUM_ROOMS - 9,
	AMMUNITION_ROOM_0				= MAX_NUM_ROOMS - 9,
	AMMUNITION_ROOM_INTERVAL_START	= MAX_NUM_ROOMS - 9,
	NUM_AMMUNITION_ROOMS			= 1,
	AMMUNITION_ROOM_CATEGORY_INDEX	= 3,
	AMMUNITION_DOOR_COLOR			= DOOR_COLOR_RED,

	KITCHEN_INTERVAL_END			= MAX_NUM_ROOMS - 10,
	KITCHEN_0						= MAX_NUM_ROOMS - 10,
	KITCHEN_INTERVAL_START			= MAX_NUM_ROOMS - 10,
	NUM_KITCHENS					= 1,
	KITCHEN_CATEGORY_INDEX			= 2,
	KITCHEN_DOOR_COLOR				= DOOR_COLOR_GREEN,

	DINING_ROOM_INTERVAL_END		= MAX_NUM_ROOMS - 11,
	DINING_ROOM_0					= MAX_NUM_ROOMS - 11,
	DINING_ROOM_INTERVAL_START		= MAX_NUM_ROOMS - 11,
	NUM_DINING_ROOMS				= 1,
	DINING_ROOM_CATEGORY_INDEX		= 1,
	DINING_DOOR_COLOR				= DOOR_COLOR_BLUE,

	CABOOSE_INTERVAL_END			= MAX_NUM_ROOMS - 12,
	CABOOSE_0						= MAX_NUM_ROOMS - 12,
	CABOOSE_1						= MAX_NUM_ROOMS - 13,
	CABOOSE_2						= MAX_NUM_ROOMS - 14,
	CABOOSE_3						= MAX_NUM_ROOMS - 15,
	CABOOSE_4						= MAX_NUM_ROOMS - 16,
	CABOOSE_5						= MAX_NUM_ROOMS - 17,
	CABOOSE_6						= MAX_NUM_ROOMS - 18,
	CABOOSE_7						= MAX_NUM_ROOMS - 19,
	CABOOSE_8						= MAX_NUM_ROOMS - 20,
	CABOOSE_9						= MAX_NUM_ROOMS - 21,
	CABOOSE_INTERVAL_START			= MAX_NUM_ROOMS - 21,
	NUM_CABOOSES					= 10,
	CABOOSE_CATEGORY_INDEX			= 0,
	CABOOSE_DOOR_COLOR				= DOOR_COLOR_BLUE,

	SMALLEST_RESERVED				= MAX_NUM_ROOMS - 21,
	NUM_RESERVED_ROOM_CATEGORIES	= 7
};

struct WorldObject
{
	glm::uvec3 TileId = glm::uvec3(0);
	int32 GameObject = -1;
	float Rotation;
};

class API World
{
	friend class WorldSerializer;

public:
	World(World&& other) = delete;
	World(const World& other) = delete;
	World& operator=(World&& other) = delete;
	World& operator=(const World& other) = delete;

	///<summary>Summary: Each level is a ship level, not a grid level. This object deletes the worldlevel and worldobject objects on deletion.</summary>
	World(WorldLevel* worldLevels[], uint32 numLevels, WorldObject* objects = nullptr, uint32 numObjects = 0) noexcept;
	~World();

	void AddWorldObject(const WorldObject& object) noexcept;
	void SetStairs(const glm::ivec3* stairs, uint32 nrOfStairs);
	void SetDoors(const glm::ivec3* doors, uint32 nrOfDoors);
	void SetRoomActive(uint32 roomID, bool isActive, uint32 lastRoom = 0, bool keepTimer = false) noexcept;

	WorldLevel& GetLevel(uint32 level) noexcept;
	const WorldLevel& GetLevel(uint32 level) const noexcept;
	const WorldObject& GetWorldObject(uint32 index) const noexcept;
	uint32 GetNumLevels() const noexcept;
	uint32 GetNumWorldObjects() const noexcept;
	Room& GetRoom(uint32 room) noexcept;
	const Room& GetRoom(uint32 room) const noexcept;
	const glm::ivec3& GetDoor(uint32 index) const noexcept;

	const std::vector<uint32>& GetActiveRooms() const noexcept;
	const std::vector<float>& GetRoomLightTimers() const noexcept;
	const std::vector<glm::ivec3>& GetStairs() const noexcept;
	const std::vector<glm::ivec3>& GetDoors() const noexcept;

	void Generate(Scene& scene) noexcept;
	void GenerateRoomShadows(const Scene& scene) noexcept;
	void CopyRoomShadows(const Scene& scene, World* pWorld) noexcept;

	//Returns true if any visibility change happend
	bool UpdateVisibility(Scene& scene, float dt);

	glm::ivec3 FindClosestRoomCenterInInterval(uint32 startInterval, uint32 endInterval, const glm::ivec3& currentTile, bool checkForFire = false) const noexcept;
	glm::ivec3 GetRandomTileInClosestRoomInInterval(uint32 startInterval, uint32 endInterval, const glm::ivec3& currentTile, bool checkForFire = false) const noexcept;
	glm::ivec3 GetRandomTileInRoom(uint32 roomIndex) const noexcept;

public:
	static std::string GetNameFromGlobal(uint32 globalIndex) noexcept;
	static uint32 GetDoorMaterialFromColor(DOOR_COLOR color) noexcept;
	static DOOR_COLOR GetDoorColorFromGlobal(uint32 globalIndex) noexcept;
	static glm::uvec3 GetReservedTileLocalIntervalAndCategoryFromGlobal(uint32 globalIndex) noexcept;
	static glm::uvec3 GetReservedTileLocalIntervalAndCategoryFromLocal(uint32 localIndex) noexcept;
	static uint32 GetReservedTileFloorMaterialFromGlobal(uint32 globalIndex) noexcept;
	static uint32 ConvertExtToNonExtFloorMaterial(const Material* pMaterial) noexcept;
	static uint32 ConvertNonExtToExtFloorMaterial(const Material* pMaterial) noexcept;

private:
	void GenerateRooms(Scene& scene) noexcept;
	void GenerateFloor(Scene& scene) noexcept;
	void GenerateLevelObject(Scene& scene) noexcept;
	void PlaceGameObjects(Scene& scene) noexcept;
	void PlaceDoors(Scene& scene) noexcept;
	void PlaceStairs(Scene& scene) noexcept;

private:
	std::vector<WorldLevel> m_Levels;
	std::vector<WorldObject> m_Objects;
	std::vector<glm::ivec3> m_Stairs;
	std::vector<glm::ivec3> m_Doors;
	Room m_Rooms[MAX_NUM_ROOMS];
	std::vector<float> m_RoomLightsTimers;
	std::vector<uint32> m_ActiveRooms;
	std::vector<PointLight*> m_RoomLights;
};

inline glm::ivec3 World::FindClosestRoomCenterInInterval(uint32 startInterval, uint32 endInterval, const glm::ivec3& currentTile, bool checkForFire) const noexcept
{
	uint32 minDistSqrd = UINT32_MAX;
	glm::ivec3 closestRoomCenter(-1);

	for (uint32 i = startInterval; i <= endInterval; i++)
	{
		const Room& room = m_Rooms[i];

		if (room.IsRoomInitialized())
		{
			if (checkForFire && room.IsBurning())
			{
				continue;
			}

			glm::ivec3 currentRoomCenter = glm::ivec3(room.GetCenter() - glm::vec3(0.0f, 1.0f, 0.0f));
			glm::ivec3 toVector = currentTile - currentRoomCenter;
			uint32 currentDistanceSqrd = (uint32)(toVector.x * toVector.x + (10.0f * toVector.y * toVector.y) + toVector.z * toVector.z);

			if (currentDistanceSqrd < minDistSqrd)
			{
				minDistSqrd = currentDistanceSqrd;
				closestRoomCenter = currentRoomCenter;
			}
		}
	}

	return closestRoomCenter;
}

inline glm::ivec3 World::GetRandomTileInClosestRoomInInterval(uint32 startInterval, uint32 endInterval, const glm::ivec3 & currentTile, bool checkForFire) const noexcept
{
	uint32 minDistSqrd = UINT32_MAX;
	glm::ivec3 closestRoomCenter(-1);
	int32 roomIndex = UINT32_MAX;

	for (uint32 i = startInterval; i <= endInterval; i++)
	{
		const Room& room = m_Rooms[i];

		if (room.IsRoomInitialized())
		{
			if (checkForFire && room.IsBurning())
			{
				continue;
			}

			glm::ivec3 currentRoomCenter = glm::ivec3(room.GetCenter() - glm::vec3(0.0f, 1.0f, 0.0f));
			glm::ivec3 toVector = currentTile - currentRoomCenter;
			uint32 currentDistanceSqrd = (uint32)(toVector.x * toVector.x + (10.0f * toVector.y * toVector.y) + toVector.z * toVector.z);

			if (currentDistanceSqrd < minDistSqrd)
			{
				minDistSqrd = currentDistanceSqrd;
				closestRoomCenter = currentRoomCenter;
				roomIndex = i;
			}
		}
	}

	if (roomIndex != UINT32_MAX)
	{
		const Room& room = m_Rooms[roomIndex];
		const std::vector<glm::ivec3>& roomTiles = room.GetRoomTiles();
		uint32 randomTile = Random::GenerateInt(0, (int32)roomTiles.size() - 1);
		return roomTiles[randomTile];
	}

	return glm::ivec3(-1);
}

inline glm::ivec3 World::GetRandomTileInRoom(uint32 roomIndex) const noexcept
{
	const Room& room = m_Rooms[roomIndex];
	const std::vector<glm::ivec3>& roomTiles = room.GetRoomTiles();
	uint32 randomTile = Random::GenerateInt(0, (int32)roomTiles.size() - 1);
	return roomTiles[randomTile];
}

inline std::string World::GetNameFromGlobal(uint32 globalIndex) noexcept
{
	if (globalIndex >= SICKBAY_INTERVAL_START && globalIndex <= SICKBAY_INTERVAL_END)
	{
		return "sjukstuga " + std::to_string((globalIndex + 1) - SICKBAY_INTERVAL_START);
	}
	else if (globalIndex >= TOILET_INTERVAL_START && globalIndex <= TOILET_INTERVAL_END)
	{
		return "toalett " + std::to_string((globalIndex + 1) - TOILET_INTERVAL_START);
	}
	else if (globalIndex >= MACHINE_ROOM_INTERVAL_START && globalIndex <= MACHINE_ROOM_INTERVAL_END)
	{
		return "maskinrum " + std::to_string((globalIndex + 1) - MACHINE_ROOM_INTERVAL_START);
	}
	else if (globalIndex >= AMMUNITION_ROOM_INTERVAL_START && globalIndex <= AMMUNITION_ROOM_INTERVAL_END)
	{
		return "ammunitionsrum " + std::to_string((globalIndex + 1) - AMMUNITION_ROOM_INTERVAL_START);
	}
	else if (globalIndex >= KITCHEN_INTERVAL_START && globalIndex <= KITCHEN_INTERVAL_END)
	{
		return "kabyss " + std::to_string((globalIndex + 1) - KITCHEN_INTERVAL_START);
	}
	else if (globalIndex >= DINING_ROOM_INTERVAL_START && globalIndex <= DINING_ROOM_INTERVAL_END)
	{
		return "mäss " + std::to_string((globalIndex + 1) - DINING_ROOM_INTERVAL_START);
	}
	else if (globalIndex >= CABOOSE_INTERVAL_START && globalIndex <= CABOOSE_INTERVAL_END)
	{
		return "hytt " + std::to_string((globalIndex + 1) - CABOOSE_INTERVAL_START);
	}
	return "rum " + std::to_string(globalIndex);
}

inline uint32 World::GetDoorMaterialFromColor(DOOR_COLOR color) noexcept
{
	switch (color)
	{
	case DOOR_COLOR_RED:
		return MATERIAL::DOOR_RED;
	case DOOR_COLOR_GREEN:
		return MATERIAL::DOOR_GREEN;
	case DOOR_COLOR_BLUE:
		return MATERIAL::DOOR_BLUE;
	case DOOR_COLOR_YELLOW:
		return MATERIAL::DOOR_YELLOW;
	default:
		return MATERIAL::DOOR_YELLOW;
	}
}

inline DOOR_COLOR World::GetDoorColorFromGlobal(uint32 globalIndex) noexcept
{
	if (globalIndex >= SICKBAY_INTERVAL_START && globalIndex <= SICKBAY_INTERVAL_END)
	{
		return (DOOR_COLOR)SICKBAY_DOOR_COLOR;
	}
	else if (globalIndex >= TOILET_INTERVAL_START && globalIndex <= TOILET_INTERVAL_END)
	{
		return (DOOR_COLOR)TOILET_DOOR_COLOR;
	}
	else if (globalIndex >= MACHINE_ROOM_INTERVAL_START && globalIndex <= MACHINE_ROOM_INTERVAL_END)
	{
		return (DOOR_COLOR)MACHINE_DOOR_COLOR;
	}
	else if (globalIndex >= AMMUNITION_ROOM_INTERVAL_START && globalIndex <= AMMUNITION_ROOM_INTERVAL_END)
	{
		return (DOOR_COLOR)AMMUNITION_DOOR_COLOR;
	}
	else if (globalIndex >= KITCHEN_INTERVAL_START && globalIndex <= KITCHEN_INTERVAL_END)
	{
		return (DOOR_COLOR)KITCHEN_DOOR_COLOR;
	}
	else if (globalIndex >= DINING_ROOM_INTERVAL_START && globalIndex <= DINING_ROOM_INTERVAL_END)
	{
		return (DOOR_COLOR)DINING_DOOR_COLOR;
	}
	else if (globalIndex >= CABOOSE_INTERVAL_START && globalIndex <= CABOOSE_INTERVAL_END)
	{
		return (DOOR_COLOR)CABOOSE_DOOR_COLOR;
	}
	return DOOR_COLOR_YELLOW;
}

inline glm::uvec3 World::GetReservedTileLocalIntervalAndCategoryFromGlobal(uint32 globalIndex) noexcept
{
	if (globalIndex >= SICKBAY_INTERVAL_START && globalIndex <= SICKBAY_INTERVAL_END)
	{
		return glm::uvec3(SICKBAY_INTERVAL_START - SMALLEST_RESERVED, SICKBAY_INTERVAL_END - SMALLEST_RESERVED, SICKBAY_CATEGORY_INDEX);
	}
	else if (globalIndex >= TOILET_INTERVAL_START && globalIndex <= TOILET_INTERVAL_END)
	{
		return glm::uvec3(TOILET_INTERVAL_START - SMALLEST_RESERVED, TOILET_INTERVAL_END - SMALLEST_RESERVED, TOILET_CATEGORY_INDEX);
	}
	else if (globalIndex >= MACHINE_ROOM_INTERVAL_START && globalIndex <= MACHINE_ROOM_INTERVAL_END)
	{
		return glm::uvec3(MACHINE_ROOM_INTERVAL_START - SMALLEST_RESERVED, MACHINE_ROOM_INTERVAL_END - SMALLEST_RESERVED, MACHINE_ROOM_CATEGORY_INDEX);
	}
	else if (globalIndex >= AMMUNITION_ROOM_INTERVAL_START && globalIndex <= AMMUNITION_ROOM_INTERVAL_END)
	{
		return glm::uvec3(AMMUNITION_ROOM_INTERVAL_START - SMALLEST_RESERVED, AMMUNITION_ROOM_INTERVAL_END - SMALLEST_RESERVED, AMMUNITION_ROOM_CATEGORY_INDEX);
	}
	else if (globalIndex >= KITCHEN_INTERVAL_START && globalIndex <= KITCHEN_INTERVAL_END)
	{
		return glm::uvec3(KITCHEN_INTERVAL_START - SMALLEST_RESERVED, KITCHEN_INTERVAL_END - SMALLEST_RESERVED, KITCHEN_CATEGORY_INDEX);
	}
	else if (globalIndex >= DINING_ROOM_INTERVAL_START && globalIndex <= DINING_ROOM_INTERVAL_END)
	{
		return glm::uvec3(DINING_ROOM_INTERVAL_START - SMALLEST_RESERVED, DINING_ROOM_INTERVAL_END - SMALLEST_RESERVED, DINING_ROOM_CATEGORY_INDEX);
	}
	else if (globalIndex >= CABOOSE_INTERVAL_START && globalIndex <= CABOOSE_INTERVAL_END)
	{
		return glm::uvec3(CABOOSE_INTERVAL_START - SMALLEST_RESERVED, CABOOSE_INTERVAL_END - SMALLEST_RESERVED, CABOOSE_CATEGORY_INDEX);
	}

	return glm::uvec3();
}

inline glm::uvec3 World::GetReservedTileLocalIntervalAndCategoryFromLocal(uint32 localIndex) noexcept
{
	return GetReservedTileLocalIntervalAndCategoryFromGlobal(localIndex + SMALLEST_RESERVED);
}

inline uint32 World::GetReservedTileFloorMaterialFromGlobal(uint32 globalIndex) noexcept
{
	if (globalIndex >= SICKBAY_INTERVAL_START && globalIndex <= SICKBAY_INTERVAL_END)
	{
		return MATERIAL::FLOOR_SICKBAY1;
	}
	else if (globalIndex >= TOILET_INTERVAL_START && globalIndex <= TOILET_INTERVAL_END)
	{
		return MATERIAL::FLOOR_TOILET1;
	}
	else if (globalIndex >= MACHINE_ROOM_INTERVAL_START && globalIndex <= MACHINE_ROOM_INTERVAL_END)
	{
		float randomFloat = Random::GenerateFloat(0.0f, 1.0f);

		if (randomFloat < 0.5f)
		{
			return MATERIAL::FLOOR_NORMAL;
		}
		else if (randomFloat < 0.667f)
		{
			return MATERIAL::FLOOR_MACHINE1;
		}
		else if (randomFloat < 0.833f)
		{
			return MATERIAL::FLOOR_MACHINE2;
		}
		else 
		{
			return MATERIAL::FLOOR_MACHINE3;
		}
	}
	else if (globalIndex >= AMMUNITION_ROOM_INTERVAL_START && globalIndex <= AMMUNITION_ROOM_INTERVAL_END)
	{
		float randomFloat = Random::GenerateFloat(0.0f, 1.0f);

		if (randomFloat < 0.5f)
		{
			return MATERIAL::FLOOR_NORMAL;
		}
		else if (randomFloat < 0.667f)
		{
			return MATERIAL::FLOOR_AMMUNITION1;
		}
		else if (randomFloat < 0.833f)
		{
			return MATERIAL::FLOOR_AMMUNITION2;
		}
		else
		{
			return MATERIAL::FLOOR_AMMUNITION3;
		}
	}
	else if (globalIndex >= KITCHEN_INTERVAL_START && globalIndex <= KITCHEN_INTERVAL_END)
	{
		return MATERIAL::FLOOR_KITCHEN1;
	}
	else if (globalIndex >= DINING_ROOM_INTERVAL_START && globalIndex <= DINING_ROOM_INTERVAL_END)
	{
		return MATERIAL::FLOOR_DINING1;
	}
	else if (globalIndex >= CABOOSE_INTERVAL_START && globalIndex <= CABOOSE_INTERVAL_END)
	{
		return MATERIAL::FLOOR_CABOOSE1;
	}

	return MATERIAL::FLOOR_NORMAL;
}

inline uint32 World::ConvertNonExtToExtFloorMaterial(const Material* pMaterial) noexcept
{
	uint32 material = ResourceHandler::GetMaterial(pMaterial);
	if (material == MATERIAL::FLOOR_NORMAL)
	{
		return MATERIAL::FLOOR_EXT_NORMAL;
	}
	else if (material == MATERIAL::FLOOR_SICKBAY1)
	{
		return MATERIAL::FLOOR_EXT_SICKBAY1;
	}
	else if (material == MATERIAL::FLOOR_TOILET1)
	{
		return MATERIAL::FLOOR_EXT_TOILET1;
	}
	else if (material == MATERIAL::FLOOR_MACHINE1)
	{
		return MATERIAL::FLOOR_EXT_MACHINE1;
	}
	else if (material == MATERIAL::FLOOR_MACHINE2)
	{
		return MATERIAL::FLOOR_EXT_MACHINE2;
	}
	else if (material == MATERIAL::FLOOR_MACHINE3)
	{
		return MATERIAL::FLOOR_EXT_MACHINE3;
	}
	else if (material == MATERIAL::FLOOR_AMMUNITION1)
	{
		return MATERIAL::FLOOR_EXT_AMMUNITION1;
	}
	else if (material == MATERIAL::FLOOR_AMMUNITION2)
	{
		return MATERIAL::FLOOR_EXT_AMMUNITION2;
	}
	else if (material == MATERIAL::FLOOR_AMMUNITION3)
	{
		return MATERIAL::FLOOR_EXT_AMMUNITION3;
	}
	else if (material == MATERIAL::FLOOR_KITCHEN1)
	{
		return MATERIAL::FLOOR_EXT_KITCHEN1;
	}
	else if (material == MATERIAL::FLOOR_DINING1)
	{
		return MATERIAL::FLOOR_EXT_DINING1;
	}
	else if (material == MATERIAL::FLOOR_CABOOSE1)
	{
		return MATERIAL::FLOOR_EXT_CABOOSE1;
	}

	return material;
}

inline uint32 World::ConvertExtToNonExtFloorMaterial(const Material* pMaterial) noexcept
{
	uint32 material = ResourceHandler::GetMaterial(pMaterial);
	if (material == MATERIAL::FLOOR_EXT_NORMAL)
	{
		return MATERIAL::FLOOR_NORMAL;
	}
	else if (material == MATERIAL::FLOOR_EXT_SICKBAY1)
	{
		return MATERIAL::FLOOR_SICKBAY1;
	}
	else if (material == MATERIAL::FLOOR_EXT_TOILET1)
	{
		return MATERIAL::FLOOR_TOILET1;
	}
	else if (material == MATERIAL::FLOOR_EXT_MACHINE1)
	{
		return MATERIAL::FLOOR_MACHINE1;
	}
	else if (material == MATERIAL::FLOOR_EXT_MACHINE2)
	{
		return MATERIAL::FLOOR_MACHINE2;
	}
	else if (material == MATERIAL::FLOOR_EXT_MACHINE3)
	{
		return MATERIAL::FLOOR_MACHINE3;
	}
	else if (material == MATERIAL::FLOOR_EXT_AMMUNITION1)
	{
		return MATERIAL::FLOOR_AMMUNITION1;
	}
	else if (material == MATERIAL::FLOOR_EXT_AMMUNITION2)
	{
		return MATERIAL::FLOOR_AMMUNITION2;
	}
	else if (material == MATERIAL::FLOOR_EXT_AMMUNITION3)
	{
		return MATERIAL::FLOOR_AMMUNITION3;
	}
	else if (material == MATERIAL::FLOOR_EXT_KITCHEN1)
	{
		return MATERIAL::FLOOR_KITCHEN1;
	}
	else if (material == MATERIAL::FLOOR_EXT_DINING1)
	{
		return MATERIAL::FLOOR_DINING1;
	}
	else if (material == MATERIAL::FLOOR_EXT_CABOOSE1)
	{
		return MATERIAL::FLOOR_CABOOSE1;
	}

	return material;
}
