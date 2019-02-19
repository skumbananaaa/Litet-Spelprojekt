#include <EnginePch.h>
#include <World/World.h>

World::World(WorldLevel* worldLevels[], uint32 numLevels, WorldObject* objects, uint32 numObjects) noexcept
{
	m_NumLevels = numLevels;
	m_ppLevels = new WorldLevel*[m_NumLevels];

	for (uint32 i = 0; i < m_NumLevels; i++)
	{
		m_ppLevels[i] = worldLevels[i];
	}

	for (uint32 i = 0; i < numObjects; i++)
	{
		m_Objects.push_back(objects[i]);
	}

	m_pStairs = nullptr;
	m_NumStairs = 0;
}

World::~World()
{
	for (uint32 i = 0; i < m_NumLevels; i++)
	{
		DeleteSafe(m_ppLevels[i]);
	}

	DeleteArrSafe(m_ppLevels);
	DeleteArrSafe(m_pStairs);
}

void World::AddWorldObject(const WorldObject& object) noexcept
{
	m_Objects.push_back(object);
}

const WorldLevel* const World::GetLevel(uint32 level) const noexcept
{
	assert(level < m_NumLevels);
	return m_ppLevels[level];
}

uint32 World::GetNumLevels() const noexcept
{
	return m_NumLevels;
}

const WorldObject& World::GetWorldObject(uint32 index) const noexcept
{
	assert(index < m_Objects.size());
	return m_Objects[index];
}

uint32 World::GetNumWorldObjects() const noexcept
{
	return static_cast<uint32>(m_Objects.size());
}

void World::GenerateWalls(uint32 level)
{
	m_ppLevels[level]->GenerateWalls();
}

void World::SetStairs(const glm::ivec3* stairs, uint32 nrOfStairs)
{
	m_NumStairs = nrOfStairs;
	m_pStairs = new glm::ivec3[m_NumStairs];

	for (int i = 0; i < m_NumStairs; i++) 
	{
		m_pStairs[i] = stairs[i];
	}
}

const glm::ivec3* World::GetStairs() const noexcept
{
	return m_pStairs;
}

uint32 World::GetNumStairs() const noexcept
{
	return m_NumStairs;
}

void World::Update(float dt)
{
	m_ppLevels[2]->UpdateFire(dt);
}