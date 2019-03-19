#include "..\..\Include\Scenarios\ScenarioFire.h"
#include <System/Random.h>
#include <World/Scenarios/Fire/FireAlarm.h>
#include "../../Include/GameState.h"

ScenarioFire::ScenarioFire(bool fireAlwaysVisible)
	: m_HasStarted(false)
{
	m_FireAlwaysVisible = fireAlwaysVisible;
}

ScenarioFire::~ScenarioFire()
{
	DeleteArrSafe(m_pppMap);
}

void ScenarioFire::Init(World* pWorld) noexcept
{
	m_pWorld = pWorld;
	SetTimeOfNextOutBreak(1.0f);
	m_pppMap = new const uint32* const*[m_pWorld->GetNumLevels()];

	for (uint32 i = 0; i < m_pWorld->GetNumLevels(); i++)
	{
		m_pppMap[i] = m_pWorld->GetLevel(i).GetLevel();
	}

	for (uint32 i = 0; i < MAX_NUM_ROOMS; i++)
	{
		m_DiscoveredRooms.push_back(false);
	}
}

void ScenarioFire::Release() noexcept
{
	DeleteArrSafe(m_pppMap);
	m_DiscoveredRooms.clear();
	m_OnFire.clear();
}

void ScenarioFire::OnStart(SceneGame* scene) noexcept
{
	uint32 lvl = Random::GenerateInt(0, m_pWorld->GetNumLevels() - 1);
	lvl += lvl % 2;
	lvl = std::min(lvl, m_pWorld->GetNumLevels() - 1);
	uint32 x = Random::GenerateInt(1, m_pWorld->GetLevel(lvl).GetSizeX() - 2);
	x = m_pWorld->GetLevel(lvl).GetSizeX() / 2;
	x -= 2;
	uint32 z = Random::GenerateInt(1, m_pWorld->GetLevel(lvl).GetSizeZ() - 2);
	z = m_pWorld->GetLevel(lvl).GetSizeZ() / 2;
	glm::ivec3 pos = glm::ivec3(x, lvl, z);

	Escalate(pos);

	/*uint32 lvl = 4;
	uint32 x = 10;
	uint32 z = 1;
	glm::ivec3 pos = glm::ivec3(x, lvl, z);

	Escalate(pos);*/
}

void ScenarioFire::OnEnd(SceneGame* scene) noexcept
{

}

void ScenarioFire::Escalate(const glm::ivec3& position, float severity) noexcept
{
	TileData& tileData = m_pWorld->GetLevel(position.y).GetLevelData()[position.x][position.z];
	Room& room = m_pWorld->GetRoom(m_pWorld->GetLevel(position.y).GetLevel()[position.x][position.z]);
	tileData.Temp = tileData.BurnsAt + 0.1f;
	tileData.Burning = true;

	MeshEmitter* emitter = dynamic_cast<MeshEmitter*>(tileData.GameObjects[GAMEOBJECT_CONST_INDEX_FIRE]);
	emitter->SetIsVisible(m_FireAlwaysVisible);
	m_OnFire.push_back(position);
	room.SetTileOnFire(position);

	if (!m_HasStarted)
	{
		m_HasStarted = true;
	}

	if (tileData.GameObjects[GAMEOBJECT_CONST_INDEX_FLOOR] != nullptr)
	{
		tileData.GameObjects[GAMEOBJECT_CONST_INDEX_FLOOR]->SetMaterial(
			World::ConvertExtToNonExtFloorMaterial(tileData.GameObjects[GAMEOBJECT_CONST_INDEX_FLOOR]->GetMaterial()));
	}
}

void ScenarioFire::OnVisibilityChange(World* pWorld, SceneGame* pScene) noexcept
{
}

bool ScenarioFire::Update(float dtS, World* pWorld, SceneGame* pScene) noexcept
{
#if defined(PRINT_CPU_DEBUG_DATA)
	CPUProfiler::StartTimer(CPU_PROFILER_SLOT_4);
#endif

	std::vector<glm::ivec3> toRemoveOnFireIDs;
	std::vector<glm::ivec3> toRemoveSmokeIDs;

	// current, right, left, forward, back, up, down.
	for (uint32 i = 0; i < m_OnFire.size(); i++)
	{
		glm::ivec3 pos = m_OnFire[i];
		TileData& originTile = m_pWorld->GetLevel(pos.y).GetLevelData()[pos.x][pos.z];

		SpreadFireSideways(dtS, glm::ivec3(0, 0, 0), pos, pScene);
		SpreadFireSideways(dtS, glm::ivec3(1, 0, 0), pos, pScene);
		SpreadFireSideways(dtS, glm::ivec3(-1, 0, 0), pos, pScene);
		SpreadFireSideways(dtS, glm::ivec3(0, 0, 1), pos, pScene);
		SpreadFireSideways(dtS, glm::ivec3(0, 0, -1), pos, pScene);

		EvaporateWater(originTile, dtS);
		((MeshEmitter*)originTile.GameObjects[GAMEOBJECT_CONST_INDEX_FIRE])->SetParticlesPerSeconds((uint32)(originTile.Temp * 5.0f / originTile.BurnsAt));

		if (originTile.Temp < originTile.BurnsAt && originTile.Burning && originTile.GameObjects[GAMEOBJECT_CONST_INDEX_FIRE] != nullptr)
		{
			toRemoveOnFireIDs.push_back(pos);
		}
	}

	for (uint32 i = 0; i < m_OnFire.size(); i++)
	{
		glm::ivec3& pos = m_OnFire[i];
		WorldLevel& level = m_pWorld->GetLevel(pos.y);
		TileData& currentTile = level.GetLevelData()[pos.x][pos.z];
		TileData& tileData = m_pWorld->GetLevel(pos.y + (pos.y + 1) % 2).GetLevelData()[pos.x][pos.z];
		bool alreadySmoke = tileData.SmokeAmount >= tileData.SmokeLimit;

		Room& room = m_pWorld->GetRoom(level.GetLevel()[pos.x][pos.z]);

		uint32 id = m_pWorld->GetLevel(pos.y + (pos.y + 1) % 2).GetLevel()[pos.x][pos.z];
		uint32 tileID = level.GetLevel()[pos.x][pos.z];

		if (room.IsFireDetected())
		{
			m_DiscoveredRooms[id] = true;
			//SetFireVisible(id, true);
			//SetSmokeVisible(id, true);

			if (id == tileID)
			{
				/*GameObject* pObject = level.GetLevelData()[pos.x][pos.z].GameObjects[GAMEOBJECT_CONST_INDEX_FIRE];
				if (pObject)
				{
					pObject->SetIsVisible(true);
				}

				pObject = level.GetLevelData()[pos.x][pos.z].GameObjects[GAMEOBJECT_CONST_INDEX_SMOKE];
				if (pObject)
				{
					pObject->SetIsVisible(true);
				}*/
				SetSmokeVisible(id);
				SetFireVisible(id);
			}

			room.ExtendAudioPos(pScene->GetExtension());
		}

		if (currentTile.WaterLevel < 0.5f * WATER_MAX_LEVEL)
		{
			tileData.SmokeAmount += std::max((m_pWorld->GetLevel(pos.y).GetLevelData()[pos.x][pos.z].Temp - m_pWorld->GetLevel(pos.y).GetLevelData()[pos.x][pos.z].BurnsAt) * SMOKE_CREATION_RATE, 0.0f);
			tileData.SmokeAmount = std::min(tileData.SmokeAmount, 1000.0f);

			if (!alreadySmoke && tileData.SmokeAmount >= tileData.SmokeLimit)
			{
				m_Smoke.push_back(pos + glm::ivec3(0.0, (pos.y + 1) % 2, 0.0f));

				if (tileData.GameObjects[GAMEOBJECT_CONST_INDEX_SMOKE] != nullptr && currentTile.GameObjects[GAMEOBJECT_CONST_INDEX_FIRE] != nullptr)
				{
					const uint32 roomIndex = m_pWorld->GetLevel(pos.y + (pos.y + 1) % 2).GetLevel()[pos.x][pos.z];
					tileData.GameObjects[GAMEOBJECT_CONST_INDEX_SMOKE]->SetIsVisible(m_DiscoveredRooms[roomIndex] || m_FireAlwaysVisible);
				}

				TileData& lowerTileData = m_pWorld->GetLevel(pos.y).GetLevelData()[pos.x][pos.z];

				for (uint32 j = lowerTileData.NrOfBaseGameObjects; j < lowerTileData.GameObjects.size(); j++)
				{
					GameObject* pGameObject = lowerTileData.GameObjects[j];

					if (pGameObject != nullptr)
					{
						if (!pGameObject->HasDetectedSmoke())
						{
							pGameObject->OnSmokeDetected();
							//ShowInRoom(m_pWorld->GetLevel(pos.y + (pos.y + 1) % 2)->GetLevel()[pos.x][pos.z]);
							m_DiscoveredRooms[id] = true;

							room.SetFireDetected(true);

							if (id == tileID)
							{
								GameObject* pObject = level.GetLevelData()[pos.x][pos.z].GameObjects[GAMEOBJECT_CONST_INDEX_FIRE];
								if (pObject)
								{
									pObject->SetIsVisible(true);
								}

								pObject = level.GetLevelData()[pos.x][pos.z].GameObjects[GAMEOBJECT_CONST_INDEX_SMOKE];
								if (pObject)
								{
									pObject->SetIsVisible(true);
								}
							}
						}
					}
				}
			}
		}
	}


	for (uint32 i = 0; i < m_Smoke.size(); i++)
	{
		glm::ivec3 smokePos = m_Smoke[i];
		TileData& tile = m_pWorld->GetLevel(smokePos.y).GetLevelData()[smokePos.x][smokePos.z];

		uint32 aboveIndex = smokePos.y + 1;

		if (tile.SmokeAmount < tile.SmokeLimit && tile.GameObjects[GAMEOBJECT_CONST_INDEX_SMOKE] != nullptr)
		{
			toRemoveSmokeIDs.push_back(smokePos);
		}

		if (aboveIndex < m_pWorld->GetNumLevels())
		{
			TileData& aboveData = m_pWorld->GetLevel(aboveIndex).GetLevelData()[smokePos.x][smokePos.z];
			Room& aboveRoom = m_pWorld->GetRoom(m_pWorld->GetLevel(aboveIndex).GetLevel()[smokePos.x][smokePos.z]);
			// TWEAK HERE
 			aboveData.Temp += glm::max(tile.SmokeAmount - tile.SmokeLimit, 0.0f) * dtS * 0.01f / aboveData.BurnsAt;
			aboveData.Temp = std::min(aboveData.Temp, MAX_TILE_TEMP);

			if (aboveData.Temp > aboveData.BurnsAt && !aboveData.Burning)
			{
				glm::ivec3 pos = smokePos + glm::ivec3(0.0f, 1.0f, 0.0f);
				aboveData.Burning = true;

				m_OnFire.push_back(pos);
				aboveRoom.SetTileOnFire(pos);

				MeshEmitter* emitter = dynamic_cast<MeshEmitter*>(aboveData.GameObjects[GAMEOBJECT_CONST_INDEX_FIRE]);
				if (aboveData.WaterLevel < WATER_UPDATE_LEVEL_INTERVAL)
				{
					emitter->SetIsVisible(m_DiscoveredRooms[m_pWorld->GetLevel(aboveIndex).GetLevel()[pos.x][pos.z]] || m_FireAlwaysVisible);

					if (aboveData.GameObjects[GAMEOBJECT_CONST_INDEX_FLOOR] != nullptr)
					{
						aboveData.GameObjects[GAMEOBJECT_CONST_INDEX_FLOOR]->SetMaterial(
							World::ConvertExtToNonExtFloorMaterial(aboveData.GameObjects[GAMEOBJECT_CONST_INDEX_FLOOR]->GetMaterial()));
					}
				}
			}
		}
		//TWEAK HERE!
		float spread = tile.SmokeAmount - tile.SmokeLimit;
		//spread /= 4;
		spread *= RATE_OF_SMOKE_SPREAD * dtS;
		uint32 rest = 0;
		if (spread > 0.0001f)
		{
			rest += SpreadSmokeSideways(dtS, glm::ivec3(1, 0, 0), smokePos, spread, pScene);
			rest += SpreadSmokeSideways(dtS, glm::ivec3(-1, 0, 0), smokePos, spread, pScene);
			rest += SpreadSmokeSideways(dtS, glm::ivec3(0, 0, 1), smokePos, spread, pScene);
			rest += SpreadSmokeSideways(dtS, glm::ivec3(0, 0, -1), smokePos, spread, pScene);
			tile.SmokeAmount -= spread * rest;
		}

		tile.SmokeAmount -= SMOKE_DISSOLVE_RATE * dtS;

		((MeshEmitter*)tile.GameObjects[GAMEOBJECT_CONST_INDEX_SMOKE])->SetParticlesPerSeconds(tile.SmokeAmount / 50.0f);
	}

	//Remove Fire and Smoke
	for (uint32 i = 0; i < toRemoveOnFireIDs.size(); i++)
	{
		WorldLevel& currentFireWorldLevel = m_pWorld->GetLevel(toRemoveOnFireIDs[i].y);
		TileData& FireLevelData = currentFireWorldLevel.GetLevelData()[toRemoveOnFireIDs[i].x][toRemoveOnFireIDs[i].z];
		MeshEmitter* emitter = dynamic_cast<MeshEmitter*>(FireLevelData.GameObjects[GAMEOBJECT_CONST_INDEX_FIRE]);
		Room& room = m_pWorld->GetRoom(currentFireWorldLevel.GetLevel()[toRemoveOnFireIDs[i].x][toRemoveOnFireIDs[i].z]);

		//Fire
		FireLevelData.Burning = false;
		FireLevelData.MarkedForExtinguish = false;
		FireLevelData.Temp = 30.0f;
		emitter->SetIsVisible(false);
		m_OnFire.erase(std::remove(m_OnFire.begin(), m_OnFire.end(), toRemoveOnFireIDs[i]), m_OnFire.end());
		room.RemoveTileOnFire(toRemoveOnFireIDs[i]);
	}

	for (uint32 i = 0; i < toRemoveSmokeIDs.size(); i++)
	{
		WorldLevel& currentSmokeWorldLevel = m_pWorld->GetLevel(toRemoveSmokeIDs[i].y);
		TileData& SmokeLevelData = currentSmokeWorldLevel.GetLevelData()[toRemoveSmokeIDs[i].x][toRemoveSmokeIDs[i].z];

		//Smoke
		SmokeLevelData.SmokeAmount = -500.0f;
		SmokeLevelData.GameObjects[GAMEOBJECT_CONST_INDEX_SMOKE]->SetIsVisible(false);
		m_Smoke.erase(std::remove(m_Smoke.begin(), m_Smoke.end(), toRemoveSmokeIDs[i]), m_Smoke.end());

		TileData& lowerTileData = m_pWorld->GetLevel(toRemoveSmokeIDs[i].y - 1).GetLevelData()[toRemoveSmokeIDs[i].x][toRemoveSmokeIDs[i].z];

		for (uint32 j = lowerTileData.NrOfBaseGameObjects; j < lowerTileData.GameObjects.size(); j++)
		{
			if (dynamic_cast <FireAlarm*>(lowerTileData.GameObjects[j]))
			{
				FireAlarm* pFireAlarm = (FireAlarm*)lowerTileData.GameObjects[j];

				if (pFireAlarm != nullptr)
				{
					if (pFireAlarm->HasDetectedSmoke())
					{
						pFireAlarm->TurnOff();
					}
				}
			}
		}
	}

	toRemoveOnFireIDs.clear();
	toRemoveSmokeIDs.clear();

#if defined(PRINT_CPU_DEBUG_DATA)
	CPUProfiler::EndTimer("Fire Scenario Update took %.3f ms", CPU_PROFILER_SLOT_4);
#endif

	constexpr float totalAmount = 40.0f * 10.0f * 3.0f;
	GameState::SetBurningAmount(m_OnFire.size() / totalAmount);
	return m_OnFire.empty() && m_Smoke.empty();
}

std::string ScenarioFire::GetName() noexcept
{
	return "Eldsvåda";
}

int32 ScenarioFire::GetCooldownTime() noexcept
{
	return 20;
}

int32 ScenarioFire::GetMaxTimeBeforeOutbreak() noexcept
{
	return 10;
}

bool ScenarioFire::IsComplete() noexcept
{
	return m_HasStarted && m_OnFire.empty();
}

void ScenarioFire::ShowInRoom(uint32 roomID) noexcept
{
	for (uint32 i = 0; i < m_Smoke.size(); i++)
	{
		glm::ivec3 pos = m_Smoke[i];
		if (m_pWorld->GetLevel(pos.y).GetLevel()[pos.x][pos.z] == roomID)
		{
			TileData& data = m_pWorld->GetLevel(pos.y).GetLevelData()[pos.x][pos.z];
			for (uint32 j = GAMEOBJECT_CONST_INDEX_SMOKE; j < data.NrOfBaseGameObjects; j++)
			{
				MeshEmitter* emitter = dynamic_cast<MeshEmitter*>(data.GameObjects[j]);
				if(emitter != nullptr)
				{
					emitter->SetIsVisible(true);
				}
			}
		}
	}

	for (uint32 i = 0; i < m_OnFire.size(); i++)
	{
		glm::ivec3 pos = m_OnFire[i];
		WorldLevel& level = m_pWorld->GetLevel(pos.y);
		if (level.GetLevel()[pos.x][pos.z] == roomID)
		{
			TileData& data = level.GetLevelData()[pos.x][pos.z];
			for (uint32 j = GAMEOBJECT_CONST_INDEX_SMOKE; j < data.NrOfBaseGameObjects; j++)
			{
				MeshEmitter* emitter = dynamic_cast<MeshEmitter*>(data.GameObjects[j]);
				if (emitter != nullptr)
				{
					emitter->SetIsVisible(true);
				}
			}
		}
	}
}

void ScenarioFire::SpreadFireSideways(float dtS, const glm::ivec3& offset, const glm::ivec3& origin, Scene* scene)
{
	glm::ivec3 tileTo = origin + offset;
	WorldLevel& originLevel = m_pWorld->GetLevel(origin.y);
	WorldLevel& toLevel = m_pWorld->GetLevel(tileTo.y);
	TileData& originTile = originLevel.GetLevelData()[origin.x][origin.z];
	TileData& tileData = toLevel.GetLevelData()[tileTo.x][tileTo.z];
	Room& room = m_pWorld->GetRoom(toLevel.GetLevel()[tileTo.x][tileTo.z]);
	uint32 tilesBetweenBulkheads = originLevel.GetTilesBetweenBulkheads();
	//TWEAK HERE

	float rateOfSpread = RATE_OF_FIRE_SPREAD;
	uint32 mapTo = m_pppMap[tileTo.y][tileTo.x][tileTo.z];
	rateOfSpread *= (float)((mapTo == m_pppMap[origin.y][origin.x][origin.z]) || (originTile.HasDoor() && tileData.HasDoor()));
	rateOfSpread += (RATE_OF_FIRE_WALL_SPREAD * (offset.y + 1) + RATE_OF_FIRE_FLOOR_SPREAD) * (mapTo != 1);
	
	bool spreadingThroughBulkhead = glm::min<uint32>(origin.z, tileTo.z) % tilesBetweenBulkheads == 0;
	rateOfSpread *= CalculateDoorSpreadFactor(originTile, tileData, spreadingThroughBulkhead);
	rateOfSpread *= CalculateBulkheadSpreadFactor(spreadingThroughBulkhead);

	rateOfSpread /= (1.0f + (tileData.Temp / 100.0f));
	tileData.Temp += std::max((originTile.Temp - tileData.BurnsAt) * rateOfSpread * dtS, 0.0f);
	tileData.Temp = std::min(tileData.Temp, MAX_TILE_TEMP);

	if (tileData.Temp >= tileData.BurnsAt && !tileData.Burning)
	{
		m_OnFire.push_back(tileTo);
		room.SetTileOnFire(tileTo);
		tileData.Burning = true;

		MeshEmitter* emitter = dynamic_cast<MeshEmitter*>(tileData.GameObjects[GAMEOBJECT_CONST_INDEX_FIRE]);
		if (emitter != nullptr)
		{
			if (tileData.WaterLevel < WATER_UPDATE_LEVEL_INTERVAL)
			{
				emitter->SetIsVisible(m_DiscoveredRooms[mapTo] || m_FireAlwaysVisible);
			}
		}

		if (tileData.GameObjects[GAMEOBJECT_CONST_INDEX_FLOOR] != nullptr)
		{
			tileData.GameObjects[GAMEOBJECT_CONST_INDEX_FLOOR]->SetMaterial(
				World::ConvertExtToNonExtFloorMaterial(tileData.GameObjects[GAMEOBJECT_CONST_INDEX_FLOOR]->GetMaterial()));
		}
	}
}

bool ScenarioFire::SpreadSmokeSideways(float dtS, const glm::ivec3& offset, const glm::ivec3& origin, float amount, Scene* pScene)
{
	bool res = false;
	glm::ivec3 tileTo = origin + offset;
	WorldLevel& level = m_pWorld->GetLevel(origin.y);
	TileData& tileData = level.GetLevelData()[tileTo.x][tileTo.z];
	TileData& lowerTileData = m_pWorld->GetLevel(origin.y - 1).GetLevelData()[tileTo.x][tileTo.z];
	Room& room = m_pWorld->GetRoom(level.GetLevel()[tileTo.x][tileTo.z]);

	if (tileData.SmokeAmount * dtS < amount * 4.0f)
	{
		bool filled = tileData.SmokeAmount >= tileData.SmokeLimit;

		bool hasOpenDoor = lowerTileData.HasDoor();
		if (hasOpenDoor)
		{
			hasOpenDoor = !reinterpret_cast<GameObjectDoor*>(lowerTileData.GameObjects[GAMEOBJECT_CONST_INDEX_DOOR])->IsClosed();
		}
		
		//HasDoor and hasStairs never set?
		uint32 mapTo = m_pppMap[tileTo.y][tileTo.x][tileTo.z];
		if (mapTo == m_pppMap[origin.y][origin.x][origin.z] || hasOpenDoor || lowerTileData.HasStairs)
		{
			tileData.SmokeAmount += amount;
			if (!filled && tileData.SmokeAmount >= tileData.SmokeLimit)
			{
				m_Smoke.push_back(tileTo);

				if (tileData.GameObjects[GAMEOBJECT_CONST_INDEX_SMOKE] != nullptr)
				{
					if (lowerTileData.WaterLevel < 0.5f * WATER_MAX_LEVEL)
					{
						tileData.GameObjects[GAMEOBJECT_CONST_INDEX_SMOKE]->SetIsVisible(m_DiscoveredRooms[mapTo] || m_FireAlwaysVisible);
					}
				}

				for (uint32 i = lowerTileData.NrOfBaseGameObjects; i < lowerTileData.GameObjects.size(); i++)
				{
					GameObject* pGameObject = lowerTileData.GameObjects[i];

					if (pGameObject != nullptr)
					{
						if (!pGameObject->HasDetectedSmoke())
						{
							pGameObject->OnSmokeDetected();
							//ShowInRoom(m_pWorld->GetLevel(pos.y + (pos.y + 1) % 2)->GetLevel()[pos.x][pos.z]);
							uint32 id = mapTo;
							m_DiscoveredRooms[id] = true;

							room.SetFireDetected(true);


							if (id == level.GetLevel()[tileTo.x][tileTo.z])
							{
								GameObject* pObject = tileData.GameObjects[GAMEOBJECT_CONST_INDEX_FIRE];
								if (pObject)
								{
									pObject->SetIsVisible(true);
								}

								pObject = tileData.GameObjects[GAMEOBJECT_CONST_INDEX_SMOKE];
								if (pObject)
								{
									pObject->SetIsVisible(true);
								}
							}
						}
					}
				}
			}

			res = true;
		}
	}
	else
	{
		res = false;
	}

	return res;
}

void ScenarioFire::SetFireVisible(uint32 roomId, bool show) noexcept
{
	for (uint32 i = 0; i < m_OnFire.size(); i++)
	{
		glm::ivec3 pos = m_OnFire[i];
		WorldLevel& level = m_pWorld->GetLevel(pos.y);
		uint32 tileID = level.GetLevel()[pos.x][pos.z];
		if (roomId == tileID)
		{
			GameObject* pObject = level.GetLevelData()[pos.x][pos.z].GameObjects[GAMEOBJECT_CONST_INDEX_FIRE];
			if (pObject)
			{
				pObject->SetIsVisible(show);
			}
		}
	}
}

void ScenarioFire::SetSmokeVisible(uint32 roomId, bool show) noexcept
{
	for (uint32 i = 0; i < m_Smoke.size(); i++)
	{
		glm::ivec3 pos = m_Smoke[i];
		WorldLevel& level = m_pWorld->GetLevel(pos.y);
		if (roomId == level.GetLevel()[pos.x][pos.z])
		{
			GameObject* pObject = level.GetLevelData()[pos.x][pos.z].GameObjects[GAMEOBJECT_CONST_INDEX_SMOKE];
			if (pObject)
			{
				pObject->SetIsVisible(show);
			}
		}
	}
}