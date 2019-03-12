#include "..\..\Include\Scenarios\ScenarioFire.h"
#include <System/Random.h>
#include <World/Scenarios/Fire/FireAlarm.h>

ScenarioFire::ScenarioFire(bool fireAlwaysVisible)
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
	/*uint32 lvl = Random::GenerateInt(0, m_pWorld->GetNumLevels() - 1);
	lvl += lvl % 2;
	lvl = std::min(lvl, m_pWorld->GetNumLevels() - 1);
	lvl = 0;
	uint32 x = Random::GenerateInt(1, m_pWorld->GetLevel(lvl).GetSizeX() - 2);
	x = m_pWorld->GetLevel(lvl).GetSizeX() / 2;
	x -= 2;
	uint32 z = Random::GenerateInt(1, m_pWorld->GetLevel(lvl).GetSizeZ() - 2);
	z = m_pWorld->GetLevel(lvl).GetSizeZ() / 2;
	glm::ivec3 pos = glm::ivec3(x, lvl, z);

	Escalate(pos);*/

	uint32 lvl = 4;
	uint32 x = 10;
	uint32 z = 1;
	glm::ivec3 pos = glm::ivec3(x, lvl, z);

	Escalate(pos);
}

void ScenarioFire::OnEnd(SceneGame* scene) noexcept
{

}

void ScenarioFire::Escalate(const glm::ivec3& position) noexcept
{
	TileData& tileData = m_pWorld->GetLevel(position.y).GetLevelData()[position.x][position.z];
	Room& room = m_pWorld->GetRoom(m_pWorld->GetLevel(position.y).GetLevel()[position.x][position.z]);
	tileData.Temp = tileData.BurnsAt + 0.1f;
	tileData.Burning = true;
	
	if (!room.IsBurning())
	{
		room.SetBurning(true);
	}
	
	room.SetBurning(true);

	MeshEmitter* emitter = dynamic_cast<MeshEmitter*>(tileData.GameObjects[GAMEOBJECT_CONST_INDEX_FIRE]);
	emitter->SetIsVisible(m_FireAlwaysVisible);
	m_OnFire.push_back(position);
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
		((MeshEmitter*)originTile.GameObjects[GAMEOBJECT_CONST_INDEX_FIRE])->SetParticlesPerSeconds(originTile.Temp * 5.0f / originTile.BurnsAt);

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
			room.ExtendAudioPos(pScene->GetExtension());
		}

		if (currentTile.WaterLevel < 0.5f * WATER_MAX_LEVEL)
		{
			tileData.SmokeAmount += std::max((m_pWorld->GetLevel(pos.y).GetLevelData()[pos.x][pos.z].Temp - m_pWorld->GetLevel(pos.y).GetLevelData()[pos.x][pos.z].BurnsAt) * 1.3f, 0.0f);
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

				for (uint32 i = tileData.NrOfBaseGameObjects; i < lowerTileData.GameObjects.size(); i++)
				{
    				FireAlarm* alarm = dynamic_cast<FireAlarm*>(lowerTileData.GameObjects[i]);
    				if (alarm)
    				{
    					if (!alarm->HasDetected())
    					{
    						lowerTileData.GameObjects[i]->OnSmokeDetected();
    						//ShowInRoom(m_pWorld->GetLevel(pos.y + (pos.y + 1) % 2)->GetLevel()[pos.x][pos.z]);
    						m_DiscoveredRooms[id] = true;
    						//SetFireVisible(id, true);
    						//SetSmokeVisible(id, true);


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

	float rateOfSpread = 0.1f;
	for (uint32 i = 0; i < m_Smoke.size(); i++)
	{
		glm::ivec3 smokePos = m_Smoke[i];
		TileData& tile = m_pWorld->GetLevel(smokePos.y).GetLevelData()[smokePos.x][smokePos.z];

		int32 aboveIndex = smokePos.y + 1;

		if (tile.SmokeAmount < tile.SmokeLimit && tile.GameObjects[GAMEOBJECT_CONST_INDEX_SMOKE] != nullptr)
		{
			toRemoveSmokeIDs.push_back(smokePos);
		}

		if (aboveIndex < m_pWorld->GetNumLevels())
		{
			TileData& aboveData = m_pWorld->GetLevel(aboveIndex).GetLevelData()[smokePos.x][smokePos.z];
			Room& aboveRoom = m_pWorld->GetRoom(m_pWorld->GetLevel(aboveIndex).GetLevel()[smokePos.x][smokePos.z]);
			// TWEAK HERE
 			aboveData.Temp += glm::max(tile.SmokeAmount - tile.SmokeLimit, 0.0f) * dtS * 0.1f / aboveData.BurnsAt;
			if (aboveData.Temp > aboveData.BurnsAt && !aboveData.Burning)
			{
				glm::ivec3 pos = smokePos + glm::ivec3(0.0f, 1.0f, 0.0f);
				aboveData.Burning = true;
				if (!aboveRoom.IsBurning())
				{
					aboveRoom.SetBurning(true);
				}

				m_OnFire.push_back(pos);

				MeshEmitter* emitter = dynamic_cast<MeshEmitter*>(aboveData.GameObjects[GAMEOBJECT_CONST_INDEX_FIRE]);
				if (aboveData.WaterLevel < WATER_UPDATE_LEVEL_INTERVAL)
				{
					emitter->SetIsVisible(m_DiscoveredRooms[m_pWorld->GetLevel(aboveIndex).GetLevel()[pos.x][pos.z]] || m_FireAlwaysVisible);
				}
			}
		}
		//TWEAK HERE!
		float spread = tile.SmokeAmount - tile.SmokeLimit;
		//spread /= 4;
		spread *= rateOfSpread * dtS;
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

		((MeshEmitter*)tile.GameObjects[GAMEOBJECT_CONST_INDEX_SMOKE])->SetParticlesPerSeconds(tile.SmokeAmount / 50);
	}

	//Remove Fire and Smoke
	for (uint32 i = 0; i < toRemoveOnFireIDs.size(); i++)
	{
		WorldLevel& currentFireWorldLevel = m_pWorld->GetLevel(toRemoveOnFireIDs[i].y);
		TileData& FireLevelData = currentFireWorldLevel.GetLevelData()[toRemoveOnFireIDs[i].x][toRemoveOnFireIDs[i].z];
		MeshEmitter* emitter = dynamic_cast<MeshEmitter*>(FireLevelData.GameObjects[GAMEOBJECT_CONST_INDEX_FIRE]);

		//Fire
		FireLevelData.Burning = false;
		emitter->SetIsVisible(false);
		m_OnFire.erase(std::remove(m_OnFire.begin(), m_OnFire.end(), toRemoveOnFireIDs[i]), m_OnFire.end());
	}

	for (uint32 i = 0; i < toRemoveSmokeIDs.size(); i++)
	{
		WorldLevel& currentSmokeWorldLevel = m_pWorld->GetLevel(toRemoveSmokeIDs[i].y);
		TileData * const * ppSmokeLevelData = currentSmokeWorldLevel.GetLevelData();

		//Smoke
		ppSmokeLevelData[toRemoveSmokeIDs[i].x][toRemoveSmokeIDs[i].z].SmokeAmount = 0.0f;
		ppSmokeLevelData[toRemoveSmokeIDs[i].x][toRemoveSmokeIDs[i].z].GameObjects[GAMEOBJECT_CONST_INDEX_SMOKE]->SetIsVisible(false);
		m_Smoke.erase(std::remove(m_Smoke.begin(), m_Smoke.end(), toRemoveSmokeIDs[i]), m_Smoke.end());
	}

	toRemoveOnFireIDs.clear();
	toRemoveSmokeIDs.clear();

#if defined(PRINT_CPU_DEBUG_DATA)
	CPUProfiler::EndTimer("Fire Scenario Update took %.3f ms", CPU_PROFILER_SLOT_4);
#endif
	return false;
	return m_OnFire.empty();
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
	return 60;
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
	float rateOfSpread = 0.9f;//0.35f;
	float rateOfWallSpread = 0.00002f;
	float rateOfNormalDoorSpread = 0.02f;
	float rateOfFloorSpread = 0.003f;
	float rateOfBulkheadSpreadFactor = 0.01f;
	float rateOfBulkheadDoorSpreadFactor = 1.25f; //This spread is relative to the "rateOfBulkheadSpreadFactor"

	uint32 mapTo = m_pppMap[tileTo.y][tileTo.x][tileTo.z];
	rateOfSpread *= (mapTo == m_pppMap[origin.y][origin.x][origin.z]) || (originTile.HasDoor() && tileData.HasDoor());
	rateOfSpread += (rateOfWallSpread * (offset.y + 1) + rateOfFloorSpread) * (mapTo != 1);
	
	if (offset.y == 0)
	{
		bool spreadingThroughBulkhead = glm::min<uint32>(origin.z, tileTo.z) % tilesBetweenBulkheads == 0;
		rateOfSpread *= CalculateDoorSpreadFactor(originLevel.GetLevelData(), origin, tileTo, spreadingThroughBulkhead, rateOfNormalDoorSpread, rateOfBulkheadDoorSpreadFactor, rateOfBulkheadSpreadFactor);
		rateOfSpread *= CalculateBulkheadSpreadFactor(spreadingThroughBulkhead, rateOfBulkheadSpreadFactor);
	}

	rateOfSpread /= (1.0f + (tileData.Temp / 100.0f));
	tileData.Temp += std::max((originTile.Temp - tileData.BurnsAt) * rateOfSpread * dtS, 0.0f);
	tileData.Temp = std::min(tileData.Temp, 1000.0f);

	if (tileData.Temp >= tileData.BurnsAt && !tileData.Burning)
	{
		m_OnFire.push_back(tileTo);
		tileData.Burning = true;
		if (!room.IsBurning())
		{
			room.SetBurning(true);
		}

		MeshEmitter* emitter = dynamic_cast<MeshEmitter*>(tileData.GameObjects[GAMEOBJECT_CONST_INDEX_FIRE]);
		if (emitter != nullptr)
		{
			if (tileData.WaterLevel < WATER_UPDATE_LEVEL_INTERVAL)
			{
				emitter->SetIsVisible(m_DiscoveredRooms[mapTo] || m_FireAlwaysVisible);
			}
		}

		for (uint32 i = tileData.NrOfBaseGameObjects; i < tileData.GameObjects.size(); i++)
		{
			FireAlarm* alarm = dynamic_cast<FireAlarm*>(tileData.GameObjects[i]);

			if (alarm != nullptr)
			{
				if (!alarm->HasDetected())
				{
					//tileData.GameObjects[i]->OnFireDetected();
					//ShowInRoom(m_pppMap[origin.y][origin.x][origin.z]);
				}
			}
		}
	}
}

bool ScenarioFire::SpreadSmokeSideways(float dtS, const glm::ivec3& offset, const glm::ivec3& origin, float amount, Scene* pScene)
{
	bool res = false;
	glm::ivec3 tileTo = origin + offset;
	WorldLevel& level = m_pWorld->GetLevel(origin.y);
	TileData& originTile = level.GetLevelData()[origin.x][origin.z];
	TileData& tileData = level.GetLevelData()[tileTo.x][tileTo.z];
	TileData& lowerTileData = m_pWorld->GetLevel(origin.y - 1).GetLevelData()[tileTo.x][tileTo.z];
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

				for (uint32 i = tileData.NrOfBaseGameObjects; i < lowerTileData.GameObjects.size(); i++)
				{
					FireAlarm* alarm = dynamic_cast<FireAlarm*>(lowerTileData.GameObjects[i]);

					if (alarm != nullptr)
					{
						if (!alarm->HasDetected())
						{
							lowerTileData.GameObjects[i]->OnSmokeDetected();
							//ShowInRoom(m_pppMap[tileTo.y][tileTo.x][tileTo.z]);
							uint32 id = mapTo;
							m_DiscoveredRooms[id] = true;
							SetFireVisible(id, true);
							SetSmokeVisible(id, true);
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

float ScenarioFire::CalculateDoorSpreadFactor(
	const TileData * const * ppLevelData, const glm::ivec2& tileFrom, const glm::ivec2& tileTo,
	bool spreadingThroughBulkhead, float rateOfNormalDoorSpread, float rateOfBulkheadDoorSpreadFactor, float rateOfBulkheadSpreadFactor) const noexcept
{
	if (ppLevelData[tileFrom.x][tileFrom.y].HasDoor() && ppLevelData[tileTo.x][tileTo.y].HasDoor())
	{
		bool doorIsOpen = !reinterpret_cast<GameObjectDoor*>(ppLevelData[tileTo.x][tileTo.y].GameObjects[GAMEOBJECT_CONST_INDEX_DOOR])->IsClosed();

		//If the smallest of the tiles x coordinates is a multiple of "tilesBetweenBulkheads", the water is trying to flood over a bulkhead.
		//Since CanSpreadTo returns 0 if there isnt a door when the water is trying to flood to a different room we know its trying to flood over a door.
		if (spreadingThroughBulkhead)
		{
			return doorIsOpen ? 1.0f / rateOfBulkheadSpreadFactor : rateOfBulkheadDoorSpreadFactor;
		}

		//If the water is trying to flood over a door that is not in a bulkhead, reduce the flood factor.
		return doorIsOpen ? 1.0f : rateOfNormalDoorSpread;
	}

	//Water is not trying to flood to a different room but tileTo has a door.
	return 1.0f;
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