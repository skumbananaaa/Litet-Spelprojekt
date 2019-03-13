#include <EnginePch.h>
#include <World/World.h>

World::World(WorldLevel* worldLevels[], uint32 numLevels, WorldObject* objects, uint32 numObjects) noexcept
	: m_Levels(),
	m_Stairs(),
	m_Objects(),
	m_Doors(),
	m_RoomLightsTimers(),
	m_ActiveRooms()
{
	for (uint32 i = 0; i < numLevels; i++)
	{
		m_Levels.push_back(*worldLevels[i]);
	}

	for (uint32 i = 0; i < numObjects; i++)
	{
		m_Objects.push_back(objects[i]);
	}
}

World::~World()
{
	//for (uint32 i = 0; i < m_NumLevels; i++)
	//{
	//	DeleteSafe(m_ppLevels[i]);
	//}

	//DeleteArrSafe(m_ppLevels);
	//DeleteArrSafe(m_pStairs);

	//for (size_t i = 0; i < m_Rooms.size(); i++)
	//{
	//	DeleteSafe(m_Rooms[i]);
	//}
}

void World::AddWorldObject(const WorldObject& object) noexcept
{
	m_Objects.push_back(object);
}

const WorldLevel& const World::GetLevel(uint32 level) const noexcept
{
	assert(level < m_Levels.size());
	return m_Levels[level];
}

WorldLevel& const World::GetLevel(uint32 level) noexcept
{
	assert(level < m_Levels.size());
	return m_Levels[level];
}

uint32 World::GetNumLevels() const noexcept
{
	return m_Levels.size();
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

void World::GenerateRooms(Scene& scene) noexcept
{
	//Room generation
	std::vector<glm::uvec4> roomBounds;
	std::vector<glm::vec3> centers;
	std::vector<uint32> centerIDs;

	glm::uvec4 emptyBounds = glm::uvec4(11, 0, 41, 0);

	constexpr float centreOffsetY = 1.85f;
	for (uint32 level = 0; level < m_Levels.size(); level += 2)
	{
		std::vector<glm::uvec4> roomBoundsLevel = m_Levels[level].GenerateRooms();
		uint32 numRoomsInLevel = 0;

		for (uint32 i = 2; i < roomBoundsLevel.size(); i++)
		{
			glm::uvec4& bounds = roomBoundsLevel[i];

			if (bounds != emptyBounds)
			{
				numRoomsInLevel++;

				roomBounds.push_back(roomBoundsLevel[i]);
				centers.push_back(glm::vec3((float)bounds.x + (bounds.y - bounds.x) / 2.0f, (float)level + centreOffsetY, (float)bounds.z + (bounds.w - bounds.z) / 2.0f));
				centerIDs.push_back(i);
			}
		}

		std::cout << "Level " << std::to_string(level) << " generated " << std::to_string(numRoomsInLevel) << " number of rooms!" << std::endl;

	}
	
	//Rooms
	std::vector<uint32> roomsAdded;

	for (uint32 i = 0; i < centers.size(); i++)
	{
		uint32 roomID = centerIDs[i];
		
		std::cout << "Room " << std::to_string(roomID) << " added!" << std::endl;

		if (std::find(roomsAdded.begin(), roomsAdded.end(), roomID) == roomsAdded.end())
		{
			roomsAdded.push_back(roomID);
			m_Rooms[roomID].Init(centers[i]);
		}
		else
		{
			std::cout << "Room " << std::to_string(roomID) << " already added!" << std::endl;
		}
	}

	std::cout << "Generated " << std::to_string(roomsAdded.size()) << " number of rooms!" << std::endl;
}

void World::GenerateFloor(Scene& scene) noexcept
{
	GameObject* pGameObject = nullptr;

	for (size_t level = 0; level < m_Levels.size(); level += 2)
	{
		for (uint32 x = 1; x < m_Levels[level].GetSizeX() - 1; x++)
		{
			for (uint32 z = 1; z < m_Levels[level].GetSizeZ() - 1; z++)
			{
				if (level > 0)
				{
					if (m_Levels[level - 2].GetLevelData()[x][z].HasStairs)
					{
						continue;
					}
				}

				pGameObject = ResourceHandler::CreateGameObject(GAMEOBJECT::FLOOR);
				pGameObject->SetMaterial(m_Levels[level].GetLevel()[x][z] < SMALLEST_RESERVED ?
					MATERIAL::FLOOR_NORMAL : World::GetReservedTileFloorMaterialFromGlobal(m_Levels[level].GetLevel()[x][z]));
				pGameObject->SetPosition(glm::vec3(x, level, z));
				pGameObject->SetScale(glm::vec3(1.0f, 0.1f, 1.0f));
				pGameObject->UpdateTransform();
				scene.AddGameObject(pGameObject);
			}
		}
	}
}

void World::PlaceGameObjects(Scene& scene) noexcept
{
	//Place objects in scene
	for (size_t i = 0; i < m_Objects.size(); i++)
	{
	    const WorldObject& worldObject = m_Objects[i];
		int32 width = m_Levels[worldObject.TileId.y].GetSizeX();
		int32 height = m_Levels[worldObject.TileId.y].GetSizeZ();
		int floorLevel = worldObject.TileId.y / 2;

		GameObject* pGameObject = ResourceHandler::CreateGameObject(worldObject.GameObject);
		glm::uvec3 pos = worldObject.TileId;
		pos.x += 1;
		pos.z += 1;
		pGameObject->SetPosition(pos);
		pGameObject->SetRotation(glm::vec4(0, 1, 0, worldObject.Rotation));
		pGameObject->SetRoom(m_Levels[pos.y].GetLevel()[pos.x][pos.z]);
		pGameObject->UpdateTransform();
		
		scene.AddGameObject(pGameObject);
		m_Levels[pos.y].GetLevelData()[pos.x][pos.z].GameObjects.push_back(pGameObject);
	}
}

void World::PlaceDoors(Scene& scene) noexcept
{
	//Generate Door GameObjects
	for (uint32 i = 0; i < m_Doors.size(); i++)
	{
		const glm::vec3& door1 = m_Doors[i];
		WorldLevel& level = m_Levels[door1.y];
		
		float halfWidth = level.GetSizeX() / 2;
		float halfHeight = level.GetSizeZ() / 2;

		for (uint32 j = i + 1; j < m_Doors.size(); j++)
		{
			const glm::vec3& door2 = m_Doors[j];
			glm::vec3 delta = door1 - door2;

			if (glm::length(delta) <= 1.0)
			{

				if (level.GetLevel()[(uint32)door1.x][(uint32)door1.z] != level.GetLevel()[(uint32)door2.x][(uint32)door2.z])
				{
					glm::vec3 position = (door1 + door2) / 2.0F;

					GameObject* pGameObject = new GameObject();
					pGameObject->SetMaterial(MATERIAL::DOOR_FRAME);
					pGameObject->SetMesh(MESH::DOOR_FRAME);
					pGameObject->SetPosition(position);
					pGameObject->SetRotation(glm::vec4(0, 1, 0, delta.z * glm::half_pi<float>()));
					pGameObject->SetDirection(glm::vec3((int)delta.z == 0, 0, -((int)delta.z == -1)));
					pGameObject->UpdateTransform();
					scene.AddGameObject(pGameObject);

					pGameObject = ResourceHandler::CreateGameObject(GAMEOBJECT::DOOR);
					pGameObject->SetPosition(position);
					pGameObject->SetRotation(glm::vec4(0, 1, 0, delta.z * glm::half_pi<float>()));
					pGameObject->SetDirection(glm::vec3((int)delta.z == 0, 0, -((int)delta.z == -1)));
					pGameObject->UpdateTransform();
					scene.AddGameObject(pGameObject);

					if (door1.y == 0 || (int32)position.z % 8 == 0)
					{
						pGameObject->SetMaterial(MATERIAL::DOOR_RED);
					}
					else
					{
						DOOR_COLOR color1 = GetDoorColorFromGlobal(level.GetLevel()[(int32)door1.x][(int32)door1.z]);
						DOOR_COLOR color2 = GetDoorColorFromGlobal(level.GetLevel()[(int32)door2.x][(int32)door2.z]);

						if (color1 < color2)
						{
							pGameObject->SetMaterial(GetDoorMaterialFromColor(color1));
						}
						else
						{
							pGameObject->SetMaterial(GetDoorMaterialFromColor(color2));
						}

					}

					level.GetLevelData()[(int32)door1.x][(int32)door1.z].GameObjects[GAMEOBJECT_CONST_INDEX_DOOR] = pGameObject;
					level.GetLevelData()[(int32)door2.x][(int32)door2.z].GameObjects[GAMEOBJECT_CONST_INDEX_DOOR] = pGameObject;

					break;
				}
			}
		}
	}
}

void World::PlaceStairs(Scene& scene) noexcept
{
	//Generate Ladder GameObjects
	for (uint32 i = 0; i < m_Stairs.size(); i++)
	{
		const glm::ivec3& stair = m_Stairs[i];
		WorldLevel& level = m_Levels[stair.y];
		
		float halfWidth = level.GetSizeX() / 2;
		float halfHeight = level.GetSizeZ() / 2;

		glm::vec3 position = ((glm::vec3)stair);

		const uint32* const* grid = level.GetLevel();
		uint32 myId = grid[stair.x][stair.z];
		float rotation = 0;

		if (grid[stair.x + 1][stair.z] != myId)
		{
			rotation = glm::half_pi<float>() * 2.0F;
		}
		else if (grid[stair.x - 1][stair.z] != myId)
		{
			rotation = 0.0F;
		}
		else if (grid[stair.x][stair.z + 1] != myId)
		{
			rotation = glm::half_pi<float>();
		}
		else if (grid[stair.x][stair.z - 1] != myId)
		{
			rotation = glm::half_pi<float>() * 3.0F;
		}

		GameObject* pGameObject = new GameObject();
		pGameObject->SetMaterial(MATERIAL::LADDER);
		pGameObject->SetMesh(MESH::LADDER);
		pGameObject->SetPosition(position);
		pGameObject->SetRotation(glm::vec4(0, 1, 0, rotation));
		pGameObject->UpdateTransform();
		scene.AddGameObject(pGameObject);
	}
}

void World::GenerateRoomShadows(const Scene& scene) noexcept
{
	for (size_t i = 0; i < MAX_NUM_ROOMS; i++)
	{
		if (m_Rooms[i].IsRoomInitialized())
		{
			m_Rooms[i].GenerateShadows(scene);
		}
	}
}

void World::Generate(Scene& scene) noexcept
{
	PlaceDoors(scene);
	GenerateRooms(scene);
	GenerateFloor(scene);
	GenerateLevelObject(scene);

	//Place game objects, doors and stairs
	PlaceGameObjects(scene);
	PlaceStairs(scene);

	//Generate lights for rooms
	for (size_t i = 0; i < MAX_NUM_ROOMS; i++)
	{
		PointLight* pLight = new PointLight(m_Rooms[i].GetCenter());
		pLight->SetIsVisible(false);
		scene.AddPointLight(pLight);

		m_RoomLights.push_back(pLight);
		m_RoomLightsTimers.push_back(0.0f);
	}
}

void World::SetStairs(const glm::ivec3* stairs, uint32 nrOfStairs)
{
	m_Stairs.resize(nrOfStairs);
	for (int i = 0; i < m_Stairs.size(); i++)
	{
		m_Stairs[i] = stairs[i];
		m_Levels[m_Stairs[i].y].GetLevelData()[m_Stairs[i].x][m_Stairs[i].z].HasStairs = true;
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

void World::SetRoomActive(uint32 roomID, bool isActive, uint32 lastRoom, bool keepTimer) noexcept
{
	if (isActive)
	{
		if (!m_Rooms[roomID].IsActive())
		{
			if (m_ActiveRooms.size() >= MAX_ROOMS_VISIBLE)
			{
				m_Rooms[m_ActiveRooms[0]].SetActive(false);
				m_RoomLights[m_ActiveRooms[0]]->SetIsVisible(false);
				m_RoomLightsTimers[m_ActiveRooms[0]] = 0.0f;
				m_ActiveRooms.erase(m_ActiveRooms.begin());
			}

			m_Rooms[roomID].SetActive(true);
			m_RoomLights[roomID]->SetIsVisible(true);
			if (keepTimer)
			{
				m_RoomLightsTimers[roomID] = m_RoomLightsTimers[lastRoom];
			}
			else
			{
				m_RoomLightsTimers[roomID] = 0.0f;
			}
			m_ActiveRooms.emplace_back(roomID);
		}
	}
	else
	{
		if (m_Rooms[roomID].IsActive())
		{
			for (size_t i = 0; i < m_ActiveRooms.size(); i++)
			{
				if (roomID == m_ActiveRooms[i])
				{
					m_Rooms[m_ActiveRooms[i]].SetActive(false);
					m_RoomLights[m_ActiveRooms[i]]->SetIsVisible(false);
					m_RoomLightsTimers[m_ActiveRooms[i]] = 0.0f;
					m_ActiveRooms.erase(m_ActiveRooms.begin() + i);
					break;
				}
			}
		}
	}
}

const std::vector<glm::ivec3>& World::GetStairs() const noexcept
{
	return m_Stairs;
}

const std::vector<glm::ivec3>& World::GetDoors() const noexcept
{
	return m_Doors;
}

Room& World::GetRoom(uint32 room) noexcept
{
	return m_Rooms[room];
}

const Room& World::GetRoom(uint32 room) const noexcept
{
	return m_Rooms[room];
}

const std::vector<uint32>& World::GetActiveRooms() const noexcept
{
	return m_ActiveRooms;
}

const std::vector<float>& World::GetRoomLightTimers() const noexcept
{
	return m_RoomLightsTimers;
}

const glm::ivec3& World::GetDoor(uint32 index) const noexcept
{
	assert(index < m_Doors.size());
	return m_Doors[index];
}

bool World::UpdateVisibility(Scene& pScene, float dt)
{
	bool result = false;
	for (uint32 i = 0; i < m_ActiveRooms.size(); i++)
	{
		m_RoomLightsTimers[m_ActiveRooms[i]] += dt;
		if (m_RoomLightsTimers[m_ActiveRooms[i]] >= 20.0f)
		{
			m_RoomLights[m_ActiveRooms[i]]->SetIsVisible(false);
			m_Rooms[m_ActiveRooms[i]].SetActive(false);
			m_RoomLightsTimers[m_ActiveRooms[i]] = 0.0f;
			
			m_ActiveRooms.erase(m_ActiveRooms.begin() + i);
			i--;

			result = true;
		}
	}

	return result;
}

void World::GenerateLevelObject(Scene& scene) noexcept
{
	for (int32 level = 0; level < m_Levels.size(); level++)
	{
		if (level % 2 == 0)
		{
			glm::vec4 wall;
			GameObject* pGameObject = nullptr;

			for (int i = 0; i < m_Levels[level].GetNrOfWalls(); i++)
			{
				wall = m_Levels[level].GetWall(i);

				pGameObject = new GameObject();
				pGameObject->SetMesh(MESH::CUBE);
				pGameObject->SetMaterial(MATERIAL::WALL_STANDARD);
				pGameObject->SetPosition(glm::vec3(wall.x, 1.0f + level, wall.y));
				pGameObject->SetScale(glm::vec3(wall.z + 0.1f, 2.0f, wall.w + 0.1f));
				pGameObject->UpdateTransform();

				scene.AddGameObject(pGameObject);
			}

			glm::vec4 bulkhead;
			for (int i = 0; i < m_Levels[level].GetNrOfBulkheads(); i++)
			{
				bulkhead = m_Levels[level].GetBulkhead(i);

				pGameObject = new GameObject();
				pGameObject->SetMesh(MESH::CUBE);
				pGameObject->SetMaterial(MATERIAL::BULKHEADS_STANDARD);
				pGameObject->SetPosition(glm::vec3(bulkhead.x, 1.0f + level, bulkhead.y));
				pGameObject->SetScale(glm::vec3(bulkhead.z + 0.1f, 2.01f, bulkhead.w + 0.19f));
				pGameObject->UpdateTransform();

				scene.AddGameObject(pGameObject);
			}
		}

		m_Levels[level].GenerateScenarioObjects(&scene, level);
	}
}