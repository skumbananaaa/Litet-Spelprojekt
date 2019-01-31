#include <EnginePch.h>
#include <World/World.h>

World::World(WorldLevel* worldLevels[], uint32 numLevels, WorldObject* objects, uint32 numObjects) noexcept
{
	m_NumLevels = numLevels;
	m_pLevels = new WorldLevel*[m_NumLevels];

	for (uint32 i = 0; i < m_NumLevels; i++)
	{
		m_pLevels[i] = worldLevels[i];
	}

	for (uint32 i = 0; i < numObjects; i++)
	{
		m_Objects.push_back(objects[i]);
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

void World::AddWorldObject(const WorldObject& object) noexcept
{
	m_Objects.push_back(object);
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

const WorldObject& World::GetWorldObject(uint32 index) const noexcept
{
	assert(index < m_Objects.size());
	return m_Objects[index];
}

uint32 World::GetNumWorldObjects() const noexcept
{
	return m_Objects.size();
}

const glm::vec4 & World::GetWall(uint32 index) const noexcept
{
	assert(index < m_Walls.size());
	return m_Walls[index];
}

uint32 World::GetNrOfWalls() const noexcept
{
	return m_Walls.size();
}

void World::GenerateWalls(uint32 level)
{
	bool wallH = false, wallV = false;
	glm::vec2 startWallH(0, 0), endWallH(0, 0);
	glm::vec2 startWallV(0, 0), endWallV(0, 0);
	const uint32* const* map = m_pLevels[level]->GetLevel();

	for (int i = 0; i < m_pLevels[0]->GetSizeX() - 1; i++) {
		for (int j = 0; j < m_pLevels[0]->GetSizeZ(); j++) {

			wallH = (map[i][j] != map[i + 1][j]);
			if (wallH && startWallH == glm::vec2(0, 0)) {
				startWallH = glm::vec2(i + 0.5, j - 0.5);
			}
			else if (!wallH && startWallH != glm::vec2(0, 0)) {
				endWallH = glm::vec2(i + 0.5, j - 0.5);
				m_Walls.push_back(glm::vec4((startWallH + endWallH) / 2.0f, endWallH - startWallH));
				startWallH = glm::vec2(0, 0);
			}
			
			wallV = (map[j][i] != map[j][i + 1]);
			if (wallV && startWallV == glm::vec2(0, 0)) {
				startWallV = glm::vec2(j - 0.5, i + 0.5);
			}
			else if (!wallV && startWallV != glm::vec2(0, 0)) {
				endWallV = glm::vec2(j - 0.5, i + 0.5);
				m_Walls.push_back(glm::vec4((startWallV + endWallV) / 2.0f, endWallV - startWallV));
				startWallV = glm::vec2(0, 0);
			}
		}
	}
}

