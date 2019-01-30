#include <EnginePch.h>
#include <World/WorldLevel.h>

WorldLevel::WorldLevel(const uint32* const levelIndexes, uint32 sizeX, uint32 sizeZ) noexcept
{
	m_SizeX = sizeX;
	m_SizeZ = sizeZ;
	m_ppLevel = new uint32*[sizeX];
	for (uint32 x = 0; x < m_SizeX; x++)
	{
		m_ppLevel[x] = new uint32[sizeZ];

		for (uint32 z = 0; z < m_SizeZ; z++)
		{
			m_ppLevel[x][z] = levelIndexes[x * m_SizeZ + z];
		}
	}
}

WorldLevel::~WorldLevel()
{
	for (uint32 x = 0; x < m_SizeX; x++)
	{
		delete[] m_ppLevel[x];
		m_ppLevel[x] = nullptr;
	}

	delete[] m_ppLevel;
	m_ppLevel = nullptr;
}

const uint32* const* const WorldLevel::GetLevel() const noexcept
{
	return m_ppLevel;
}

uint32 WorldLevel::GetSizeX() const noexcept
{
	return m_SizeX;
}

uint32 WorldLevel::GetSizeZ() const noexcept
{
	return m_SizeZ;
}
