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
		delete m_ppLevels[i];
		m_ppLevels[i] = nullptr;
	}

	delete[] m_ppLevels;
	m_ppLevels = nullptr;
	delete[] m_pStairs;
	m_pStairs = nullptr;
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

void World::GenerateRooms()
{
	std::vector<glm::uvec4> roomBounds;
	std::vector<glm::uvec4> temp;

	std::vector<glm::vec3> center;

	for (int level = 0; level < m_NumLevels; level += 2)
	{
		m_ppLevels[level]->GenerateRooms();
		temp = m_ppLevels[level]->GetRooms();
		for (size_t i = 0; i < temp.size(); i++)
		{
			if (i >= roomBounds.size())
			{
				roomBounds.push_back(temp[i]);
				center.push_back(glm::vec3((float)temp[i].x + (temp[i].y - temp[i].x) / 2.0f, (float)level + 1.9, (float)temp[i].z + (temp[i].w - temp[i].z) / 2.0f));
			}
			else if (temp[i].x != 11)
			{
				center[i] = glm::vec3((float)temp[i].x + (temp[i].y - temp[i].x) / 2.0f, (float)level + 1.9, (float)temp[i].z + (temp[i].w - temp[i].z) / 2.0f);
			}
		}
	}
	
	for (size_t i = 0; i < center.size(); i++)
	{
		m_Rooms.push_back(Room(center[i]));
	}
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

const glm::vec3 & World::GetRoomCenter(uint32 room) const noexcept
{
	assert(room < m_Rooms.size());
	return m_Rooms[room].GetCenter();
}

void World::Update(float dt)
{
	m_ppLevels[2]->UpdateFire(dt);
}