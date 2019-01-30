#pragma once
#include <EnginePch.h>
#include <World/WorldLevel.h>

struct WorldObject
{
	glm::uvec3 TileId = glm::uvec3(0);
	uint32 MeshId = 0;
	uint32 MaterialId = 0;
};

class API World
{
public:
	World(World&& other) = delete;
	World(const World& other) = delete;
	World& operator=(World&& other) = delete;
	World& operator=(const World& other) = delete;

	///<summry>Summary: Each level is a ship level, not a grid level. This object deletes the worldlevel and worldobject objects on deletion.</summary>
	World(WorldLevel* worldLevels[], uint32 numLevels, WorldObject* objects = nullptr, uint32 numObjects = 0) noexcept;
	~World();

	void AddWorldObject(const WorldObject& object) noexcept;

	const WorldLevel* const GetLevel(uint32 level) const noexcept;
	uint32 GetNumLevels() const noexcept;
	const WorldObject& GetWorldObject(uint32 index) const noexcept;
	uint32 GetNumWorldObjects() const noexcept;

private:
	WorldLevel** m_pLevels;
	uint32 m_NumLevels;
	std::vector<WorldObject> m_Objects;
};