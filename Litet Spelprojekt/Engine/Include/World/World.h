#pragma once
#include <EnginePch.h>
#include <World/WorldLevel.h>
#include <World/Room.h>

struct WorldObject
{
	glm::uvec3 TileId = glm::uvec3(0);
	int32 GameObject = -1;
	float Rotation;
};

class API World
{
public:
	World(World&& other) = delete;
	World(const World& other) = delete;
	World& operator=(World&& other) = delete;
	World& operator=(const World& other) = delete;

	///<summary>Summary: Each level is a ship level, not a grid level. This object deletes the worldlevel and worldobject objects on deletion.</summary>
	World(WorldLevel* worldLevels[], uint32 numLevels, WorldObject* objects = nullptr, uint32 numObjects = 0) noexcept;
	~World();

	void AddWorldObject(const WorldObject& object) noexcept;

	const WorldLevel* const GetLevel(uint32 level) const noexcept;
	uint32 GetNumLevels() const noexcept;
	const WorldObject& GetWorldObject(uint32 index) const noexcept;
	uint32 GetNumWorldObjects() const noexcept;
	void GenerateRooms();
	void SetStairs(const glm::ivec3* stairs, uint32 nrOfStairs);
	const glm::ivec3* GetStairs() const noexcept;
	uint32 GetNumStairs() const noexcept;
	const glm::vec3& GetRoomCenter(uint32 room) const noexcept;
	void Update(float dt);

private:
	WorldLevel** m_ppLevels;
	uint32 m_NumLevels;
	std::vector<WorldObject> m_Objects;

	glm::ivec3* m_pStairs;
	uint32 m_NumStairs;

	std::vector<Room> m_Rooms;
};