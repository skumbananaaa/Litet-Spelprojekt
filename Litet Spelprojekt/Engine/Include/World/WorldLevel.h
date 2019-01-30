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

private:
	uint32** m_ppLevel;
	uint32 m_SizeX;
	uint32 m_SizeZ;
};