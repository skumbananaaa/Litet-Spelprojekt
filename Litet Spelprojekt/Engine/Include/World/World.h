#pragma once
#include <EnginePch.h>
#include <World/WorldLevel.h>
#include <World/Room.h>
#include <Graphics/Scene.h>
#include <System/Input.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <GLM/gtx/string_cast.hpp>

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

	WorldLevel& GetLevel(uint32 level) noexcept;
	const WorldLevel& GetLevel(uint32 level) const noexcept;
	const WorldObject& GetWorldObject(uint32 index) const noexcept;
	uint32 GetNumLevels() const noexcept;
	uint32 GetNumWorldObjects() const noexcept;
	Room& GetRoom(uint32 room) noexcept;
	const Room& GetRoom(uint32 room) const noexcept;
	const std::vector<uint32>& GetActiveRooms() const noexcept;
	const std::vector<float>& GetRoomLightTimers() const noexcept;
	const std::vector<glm::ivec3>& GetStairs() const noexcept;
	const std::vector<glm::ivec3>& GetDoors() const noexcept;
	const glm::ivec3& GetDoor(uint32 index) const noexcept;
	void GenerateFloor(Scene* pScene) noexcept;

	void PushRoomLightTimer(float timer);
	void RemoveRoomLightTimer(float timer);
	void PopRoomLightTimer();

	void PushActiveRoom(uint32 roomID);
	void RemoveActiveRoom(uint32 roomID);
	void PopActiveRoom();

	void Generate(Scene& scene) noexcept;
	void GenerateWater(Scene& scene) noexcept;

	//Returns true if any visibility change happend
	bool UpdateVisibility(Scene& scene, float dt);

private:
	void GenerateRooms(Scene& scene) noexcept;
	void PlaceGameObjects(Scene& scene) noexcept;
	void PlaceDoors(Scene& scene) noexcept;
	void PlaceStairs(Scene& scene) noexcept;
	void GenerateWalls(Scene& scene) noexcept;
	void GenerateRoomShadows(const Scene& scene) noexcept;

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