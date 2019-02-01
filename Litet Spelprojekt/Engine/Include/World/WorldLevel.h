#pragma once
#include <EnginePch.h>

class API WorldLevel
{
public:
	WorldLevel(WorldLevel&& other) = delete;
	WorldLevel(const WorldLevel& other) = delete;
	WorldLevel& operator=(WorldLevel&& other) = delete;
	WorldLevel& operator=(const WorldLevel& other) = delete;

	WorldLevel(const uint32* const levelIndexes, uint32 sizeX, uint32 sizeZ) noexcept;
	~WorldLevel();

	const uint32* const* const GetLevel() const noexcept;
	uint32 GetSizeX() const noexcept;
	uint32 GetSizeZ() const noexcept;

	///<summary>Returns a vec4. The first two values are the position of the wall (x, z), the second two are the dimensions of the wall (x, z)</summary>
	const glm::vec4& GetWall(uint32 index) const noexcept;
	uint32 GetNrOfWalls() const noexcept;
	const glm::ivec2& GetStairsUp(uint32 index) const noexcept;
	uint32 GetNrOfStairsUp() const noexcept;
	const glm::ivec2& GetStairsDown(uint32 index) const noexcept;
	uint32 GetNrOfStairsDown() const noexcept;

	void GenerateWalls();

private:
	uint32** m_ppLevel;
	uint32 m_SizeX;
	uint32 m_SizeZ;

	uint32 m_nrOfWalls;
	std::vector<glm::vec4> m_Walls;
	std::vector<glm::ivec2> m_StairsUp;
	std::vector<glm::ivec2> m_StairsDown;
};