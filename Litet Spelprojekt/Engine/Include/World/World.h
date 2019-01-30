#pragma once
#include <EnginePch.h>
#include <World/WorldLevel.h>

class API World
{
public:
	World(World&& other) = delete;
	World(const World& other) = delete;
	World& operator=(World&& other) = delete;
	World& operator=(const World& other) = delete;

	///<summary>Summary: Each level is a ship level, not a grid level. This object deletes the level objects on deletion.</summary>
	World(WorldLevel* worldLevels[], uint32 numLevels = 3) noexcept;
	~World();

	const WorldLevel* const GetLevel(uint32 level) const noexcept;
	uint32 GetNumLevels() const noexcept;

private:
	WorldLevel** m_pLevels;
	uint32 m_NumLevels;
};