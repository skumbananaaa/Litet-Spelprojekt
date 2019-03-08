#pragma once
#include <EnginePch.h>
#include <World/WorldLevel.h>
#include <World/Room.h>
#include <Graphics/Scene.h>
#include <System/Input.h>
#include <System/Random.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <GLM/gtx/string_cast.hpp>

#define MAX_NUM_ROOMS 128

enum DOOR_COLOR : uint32
{
	RED,
	GREEN,
	BLUE,
	YELLOW
};

enum ReservedTileIndexes : uint32
{
	SICKBAY_INTERVAL_END			= MAX_NUM_ROOMS - 1,
	SICKBAY_0						= MAX_NUM_ROOMS - 1,
	SICKBAY_1						= MAX_NUM_ROOMS - 2,
	SICKBAY_INTERVAL_START			= MAX_NUM_ROOMS - 2,
	NUM_SICKBAYS					= 2,
	SICKBAY_CATEGORY_INDEX			= 6,
	SICKBAY_DOOR_COLOR				= YELLOW,

	TOILET_INTERVAL_END				= MAX_NUM_ROOMS - 3,
	TOILET_0						= MAX_NUM_ROOMS - 3,
	TOILET_1						= MAX_NUM_ROOMS - 4,
	TOILET_2						= MAX_NUM_ROOMS - 5,
	TOILET_3						= MAX_NUM_ROOMS - 6,
	TOILET_INTERVAL_START			= MAX_NUM_ROOMS - 6,
	NUM_TOILETS						= 4,
	TOILET_CATEGORY_INDEX			= 5,
	TOILET_DOOR_COLOR				= RED,

	MACHINE_ROOM_INTERVAL_END		= MAX_NUM_ROOMS - 7,
	MACHINE_ROOM_0					= MAX_NUM_ROOMS - 7,
	MACHINE_ROOM_1					= MAX_NUM_ROOMS - 8,
	MACHINE_ROOM_INTERVAL_START		= MAX_NUM_ROOMS - 8,
	NUM_MACHINE_ROOMS				= 2,
	MACHINE_ROOM_CATEGORY_INDEX		= 4,
	MACHINE_DOOR_COLOR				= RED,

	AMMUNITION_ROOM_INTERVAL_END	= MAX_NUM_ROOMS - 9,
	AMMUNITION_ROOM_0				= MAX_NUM_ROOMS - 9,
	AMMUNITION_ROOM_INTERVAL_START	= MAX_NUM_ROOMS - 9,
	NUM_AMMUNITION_ROOMS			= 1,
	AMMUNITION_ROOM_CATEGORY_INDEX	= 3,
	AMMUNITION_DOOR_COLOR			= RED,

	KITCHEN_INTERVAL_END			= MAX_NUM_ROOMS - 10,
	KITCHEN_0						= MAX_NUM_ROOMS - 10,
	KITCHEN_INTERVAL_START			= MAX_NUM_ROOMS - 10,
	NUM_KITCHENS					= 1,
	KITCHEN_CATEGORY_INDEX			= 2,
	KITCHEN_DOOR_COLOR				= GREEN,

	DINING_ROOM_INTERVAL_END		= MAX_NUM_ROOMS - 11,
	DINING_ROOM_0					= MAX_NUM_ROOMS - 11,
	DINING_ROOM_INTERVAL_START		= MAX_NUM_ROOMS - 11,
	NUM_DINING_ROOMS				= 1,
	DINING_ROOM_CATEGORY_INDEX		= 1,
	DINING_DOOR_COLOR				= BLUE,

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
	CABOOSE_DOOR_COLOR				= BLUE,

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
	void SetRoomActive(uint32 roomID, bool isActive) noexcept;

	WorldLevel& GetLevel(uint32 level) noexcept;
	const WorldLevel& GetLevel(uint32 level) const noexcept;
	const WorldObject& GetWorldObject(uint32 index) const noexcept;
	uint32 GetNumRooms() const noexcept;
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

	//Returns true if any visibility change happend
	bool UpdateVisibility(Scene& scene, float dt);

public:
	static uint32 GetDoorMaterialFromColor(DOOR_COLOR color) noexcept;
	static DOOR_COLOR GetDoorColorFromGlobal(uint32 globalIndex) noexcept;
	static glm::uvec3 GetReservedTileLocalIntervalAndCategoryFromGlobal(uint32 globalIndex) noexcept;
	static glm::uvec3 GetReservedTileLocalIntervalAndCategoryFromLocal(uint32 localIndex) noexcept;
	static uint32 GetReservedTileFloorMaterialFromGlobal(uint32 globalIndex) noexcept;

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
	std::vector<Room> m_Rooms;
	std::vector<float> m_RoomLightsTimers;
	std::vector<uint32> m_ActiveRooms;
	std::vector<PointLight*> m_RoomLights;
};

inline uint32 World::GetDoorMaterialFromColor(DOOR_COLOR color) noexcept
{
	switch (color)
	{
	case RED:
		return MATERIAL::RED;
	case GREEN:
		return MATERIAL::GREEN;
	case BLUE:
		return MATERIAL::BLUE;
	case YELLOW:
		return MATERIAL::YELLOW;
	default:
		return MATERIAL::YELLOW;
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
	return YELLOW;
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
}

inline glm::uvec3 World::GetReservedTileLocalIntervalAndCategoryFromLocal(uint32 localIndex) noexcept
{
	return GetReservedTileLocalIntervalAndCategoryFromGlobal(localIndex + SMALLEST_RESERVED);
}

inline uint32 World::GetReservedTileFloorMaterialFromGlobal(uint32 globalIndex) noexcept
{
	if (globalIndex >= SICKBAY_INTERVAL_START && globalIndex <= SICKBAY_INTERVAL_END)
	{
		return MATERIAL::FLOOR_SICKBAY;
	}
	else if (globalIndex >= TOILET_INTERVAL_START && globalIndex <= TOILET_INTERVAL_END)
	{
		return MATERIAL::FLOOR_NORMAL;
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
		return MATERIAL::FLOOR_NORMAL;
	}
	else if (globalIndex >= DINING_ROOM_INTERVAL_START && globalIndex <= DINING_ROOM_INTERVAL_END)
	{
		return MATERIAL::FLOOR_NORMAL;
	}
	else if (globalIndex >= CABOOSE_INTERVAL_START && globalIndex <= CABOOSE_INTERVAL_END)
	{
		return MATERIAL::FLOOR_NORMAL;
	}

	return MATERIAL::FLOOR_NORMAL;
}