#include <EnginePch.h>
#include <World/World.h>

World::World(WorldLevel** worldLevels, uint32 numLevels) noexcept
{
	m_NumLevels = numLevels;
	m_pLevels = new WorldLevel*[m_NumLevels];

	for (uint32 i = 0; i < m_NumLevels; i++)
	{
		m_pLevels[i] = worldLevels[i];
	}
}

World::~World()
{
	for (uint32 i = 0; i < m_NumLevels; i++)
	{
		delete m_pLevels[i];
		m_pLevels[i] = nullptr;
	}

	delete[] m_pLevels;
	m_pLevels = nullptr;
}

const WorldLevel* const World::GetLevel(uint32 level) const noexcept
{
	assert(level < m_NumLevels);
	return m_pLevels[level];
}

uint32 World::GetNumLevels() const noexcept
{
	return m_NumLevels;
}
