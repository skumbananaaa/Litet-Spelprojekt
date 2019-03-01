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

	for (size_t i = 0; i < m_Rooms.size(); i++)
	{
		DeleteSafe(m_Rooms[i]);
	}
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

WorldLevel* const World::GetLevel(uint32 level) noexcept
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
	std::vector<glm::uvec4> m_RoomBounds;
	std::vector<glm::uvec4> temp;

	std::vector<glm::vec3> center;

	for (int level = 0; level < m_NumLevels; level += 2)
	{
		m_ppLevels[level]->GenerateRooms();
		temp = m_ppLevels[level]->GetRooms();
		for (size_t i = 0; i < temp.size(); i++)
		{
			if (i >= m_RoomBounds.size())
			{
				m_RoomBounds.push_back(temp[i]);
				center.push_back(glm::vec3((float)temp[i].x + (temp[i].y - temp[i].x) / 2.0f, (float)level + 1.9, (float)temp[i].z + (temp[i].w - temp[i].z) / 2.0f));
			}
			else if (temp[i].x != 11)
			{
				center[i] = glm::vec3((float)temp[i].x + (temp[i].y - temp[i].x) / 2.0f, (float)level + 1.9, (float)temp[i].z + (temp[i].w - temp[i].z) / 2.0f);
			}
		}
	}
	
	m_Rooms.push_back(new Room());
	m_Rooms.push_back(new Room());
	for (size_t i = 2; i < center.size(); i++)
	{
		m_Rooms.push_back(new Room(center[i]));
	}
}

void World::GenerateWater(Scene* pScene) noexcept
{
	for (int level = 0; level < m_NumLevels; level++)
	{
		m_ppLevels[level]->GenerateScenarioObjects(pScene, level);
	}
}

void World::SetStairs(const glm::ivec3* stairs, uint32 nrOfStairs)
{
	m_NumStairs = nrOfStairs;
	m_pStairs = new glm::ivec3[m_NumStairs];

	for (int i = 0; i < m_NumStairs; i++) 
	{
		m_pStairs[i] = stairs[i];

		m_ppLevels[m_pStairs[i].y]->GetLevelData()[m_pStairs[i].x][m_pStairs[i].z].HasStairs = true;
	}
}

void World::SetDoors(const glm::ivec3* doors, uint32 nrOfDoors)
{
	for (int i = 0; i < nrOfDoors; i++)
	{
		m_Doors.push_back(doors[i]);
		//m_ppLevels[doors[i].y]->GetLevelData()[doors[i].x][doors[i].z].GameObjects[GAMEOBJECT_CONST_INDEX_DOOR] = 
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

Room* World::GetRoom(uint32 room) const noexcept
{
	assert(room < m_Rooms.size());
	return m_Rooms[room];
}

const glm::ivec3& World::GetDoor(uint32 index) const noexcept
{
	assert(index < m_Doors.size());
	return m_Doors[index];
}

uint32 World::GetNumDoors() const noexcept
{
	return m_Doors.size();
}

uint32 World::GetNumRooms() const noexcept
{
	return m_Rooms.size();
}

void World::Update(Scene* pScene, float dt)
{
}
