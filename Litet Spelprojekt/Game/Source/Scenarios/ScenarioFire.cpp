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

	for (uint32 i = 0; i < m_pWorld->GetNumRooms(); i++)
	{
		m_DiscoveredRooms.push_back(false);
	}
}

void ScenarioFire::OnStart(Scene* scene) noexcept
{
	uint32 lvl = Random::GenerateInt(0, m_pWorld->GetNumLevels() - 1);
	lvl += lvl % 2;
	lvl = std::min(lvl, m_pWorld->GetNumLevels() - 1);
	lvl = 0;
	uint32 x = Random::GenerateInt(1, m_pWorld->GetLevel(lvl).GetSizeX() - 2);
	x = m_pWorld->GetLevel(lvl).GetSizeX() / 2;
	x -= 2;
	uint32 z = Random::GenerateInt(1, m_pWorld->GetLevel(lvl).GetSizeZ() - 2);
	z = m_pWorld->GetLevel(lvl).GetSizeZ() / 2;
	glm::ivec3 pos = glm::ivec3(x, lvl, z);
	m_OnFire.push_back(pos);

	TileData& tileData = m_pWorld->GetLevel(lvl).GetLevelData()[x][z];
	tileData.Temp = tileData.BurnsAt + 0.1f;
	tileData.Burning = true;

	tileData.GameObjects[GAMEOBJECT_CONST_INDEX_FIRE]->SetIsVisible(m_FireAlwaysVisible);
}

void ScenarioFire::OnEnd(Scene* scene) noexcept
{

}

void ScenarioFire::OnVisibilityChange(World* pWorld, Scene* pScene) noexcept
{
}

bool ScenarioFire::Update(float dtS, World* world, Scene* scene) noexcept
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
		TileData& originTile = m_pWorld->GetLevel(pos.y)->GetLevelData()[pos.x][pos.z];

		SpreadFireSideways(dtS, glm::ivec3(0, 0, 0), pos, scene);
		SpreadFireSideways(dtS, glm::ivec3(1, 0, 0), pos, scene);
		SpreadFireSideways(dtS, glm::ivec3(-1, 0, 0), pos, scene);
		SpreadFireSideways(dtS, glm::ivec3(0, 0, 1), pos, scene);
		SpreadFireSideways(dtS, glm::ivec3(0, 0, -1), pos, scene);

		EvaporateWater(originTile, dtS);

		if (originTile.Temp < originTile.BurnsAt && originTile.Burning)
		{
			toRemoveOnFireIDs.push_back(pos);
		}
	}

	for (uint32 i = 0; i < m_OnFire.size(); i++)
	{
		glm::ivec3& pos = m_OnFire[i];
		TileData& currentTile = m_pWorld->GetLevel(pos.y)->GetLevelData()[pos.x][pos.z];
		TileData& tileData = m_pWorld->GetLevel(pos.y + (pos.y + 1) % 2)->GetLevelData()[pos.x][pos.z];
		bool alreadySmoke = tileData.SmokeAmount >= tileData.SmokeLimit;

		if (currentTile.WaterLevel < 0.5f * WATER_MAX_LEVEL)
		{
			tileData.SmokeAmount += m_pWorld->GetLevel(pos.y)->GetLevelData()[pos.x][pos.z].Temp * 2.0f;
			tileData.SmokeAmount = std::min(tileData.SmokeAmount, 1000.0f);

			if (!alreadySmoke && tileData.SmokeAmount >= tileData.SmokeLimit)
			{
				m_Smoke.push_back(pos + glm::ivec3(0.0, (pos.y + 1) % 2, 0.0f));

				if (tileData.GameObjects[GAMEOBJECT_CONST_INDEX_SMOKE] != nullptr && currentTile.GameObjects[GAMEOBJECT_CONST_INDEX_FIRE] != nullptr)
				{
					tileData.GameObjects[GAMEOBJECT_CONST_INDEX_SMOKE]->SetIsVisible(m_DiscoveredRooms[m_pWorld->GetLevel(pos.y + (pos.y + 1) % 2)->GetLevel()[pos.x][pos.z]] || m_FireAlwaysVisible);
				}

				TileData& lowerTileData = m_pWorld->GetLevel(pos.y)->GetLevelData()[pos.x][pos.z];

				for (uint32 i = tileData.NrOfBaseGameObjects; i < lowerTileData.GameObjects.size(); i++)
				{
    				FireAlarm* alarm = dynamic_cast<FireAlarm*>(lowerTileData.GameObjects[i]);

    				if (alarm != nullptr)
    				{
    					if (!alarm->HasDetected())
    					{
    						lowerTileData.GameObjects[i]->OnSmokeDetected();
    						//ShowInRoom(m_pWorld->GetLevel(pos.y + (pos.y + 1) % 2)->GetLevel()[pos.x][pos.z]);
    						uint32 id = m_pWorld->GetLevel(pos.y + (pos.y + 1) % 2)->GetLevel()[pos.x][pos.z];
    						m_DiscoveredRooms[id] = true;
    						SetFireVisible(id, true);
    						SetSmokeVisible(id, true);
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
		TileData& tile = m_pWorld->GetLevel(smokePos.y)->GetLevelData()[smokePos.x][smokePos.z];

		int32 aboveIndex = smokePos.y + 1;

		if (tile.SmokeAmount < tile.SmokeLimit)
		{
			toRemoveSmokeIDs.push_back(smokePos);
		}

		if (aboveIndex < m_pWorld->GetNumLevels())
		{
			TileData& aboveData = m_pWorld->GetLevel(aboveIndex)->GetLevelData()[smokePos.x][smokePos.z];
			// TWEAK HERE
 			aboveData.Temp += glm::max(tile.SmokeAmount - tile.SmokeLimit, 0.0f) * dtS * 3.0f / aboveData.BurnsAt;
			if (aboveData.Temp > aboveData.BurnsAt && !aboveData.Burning)
			{
				glm::ivec3 pos = smokePos + glm::ivec3(0.0f, 1.0f, 0.0f);
				aboveData.Burning = true;
				m_OnFire.push_back(pos);

				if (aboveData.WaterLevel < WATER_UPDATE_LEVEL_INTERVAL)
				{
					aboveData.GameObjects[GAMEOBJECT_CONST_INDEX_FIRE]->SetIsVisible(m_DiscoveredRooms[m_pWorld->GetLevel(aboveIndex)->GetLevel()[pos.x][pos.z]] || m_FireAlwaysVisible);
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
			rest += SpreadSmokeSideways(dtS, glm::ivec3(1, 0, 0), smokePos, spread, scene);
			rest += SpreadSmokeSideways(dtS, glm::ivec3(-1, 0, 0), smokePos, spread, scene);
			rest += SpreadSmokeSideways(dtS, glm::ivec3(0, 0, 1), smokePos, spread, scene);
			rest += SpreadSmokeSideways(dtS, glm::ivec3(0, 0, -1), smokePos, spread, scene);
			tile.SmokeAmount -= spread * rest;
		}
	}

	//Remove Fire and Smoke
	for (uint32 i = 0; i < toRemoveOnFireIDs.size(); i++)
	{
		WorldLevel* const pCurrentFireWorldLevel = m_pWorld->GetLevel(toRemoveOnFireIDs[i].y);
		TileData * const * ppFireLevelData = pCurrentFireWorldLevel->GetLevelData();

		//Fire
		ppFireLevelData[toRemoveOnFireIDs[i].x][toRemoveOnFireIDs[i].z].Burning = false;
		ppFireLevelData[toRemoveOnFireIDs[i].x][toRemoveOnFireIDs[i].z].GameObjects[GAMEOBJECT_CONST_INDEX_FIRE]->SetIsVisible(false);
		m_OnFire.erase(std::remove(m_OnFire.begin(), m_OnFire.end(), toRemoveOnFireIDs[i]));
	}

	for (uint32 i = 0; i < toRemoveSmokeIDs.size(); i++)
	{
		WorldLevel* const pCurrentSmokeWorldLevel = m_pWorld->GetLevel(toRemoveSmokeIDs[i].y);
		TileData * const * ppSmokeLevelData = pCurrentSmokeWorldLevel->GetLevelData();

		//Smoke
		ppSmokeLevelData[toRemoveSmokeIDs[i].x][toRemoveSmokeIDs[i].z].SmokeAmount = 0.0f;
		ppSmokeLevelData[toRemoveSmokeIDs[i].x][toRemoveSmokeIDs[i].z].GameObjects[GAMEOBJECT_CONST_INDEX_SMOKE]->SetIsVisible(false);
		m_Smoke.erase(std::remove(m_Smoke.begin(), m_Smoke.end(), toRemoveSmokeIDs[i]));
	}

	toRemoveOnFireIDs.clear();
	toRemoveSmokeIDs.clear();

#if defined(PRINT_CPU_DEBUG_DATA)
	CPUProfiler::EndTimer("Fire Scenario Update took %.3f ms", CPU_PROFILER_SLOT_4);
#endif
	return m_OnFire.empty();
}

std::string ScenarioFire::GetName() noexcept
{
	return "Bajseld";
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
			TileData& data = m_pWorld->GetLevel(pos.y)->GetLevelData()[pos.x][pos.z];
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
		if (m_pWorld->GetLevel(pos.y).GetLevel()[pos.x][pos.z] == roomID)
		{
			TileData& data = m_pWorld->GetLevel(pos.y)->GetLevelData()[pos.x][pos.z];
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
	TileData& originTile = m_pWorld->GetLevel(origin.y)->GetLevelData()[origin.x][origin.z];
	TileData& tileData = m_pWorld->GetLevel(tileTo.y)->GetLevelData()[tileTo.x][tileTo.z];
	uint32 tilesBetweenBulkheads = m_pWorld->GetLevel(origin.y)->GetTilesBetweenBulkheads();
	//TWEAK HERE
	float rateOfSpread = 1.0f;
	float rateOfWallSpread = 0.0002f;
	float rateOfNormalDoorSpread = 0.02f;
	float rateOfFloorSpread = 0.0003f;
	float rateOfBulkheadSpreadFactor = 0.01f;
	float rateOfBulkheadDoorSpreadFactor = 1.25f; //This spread is relative to the "rateOfBulkheadSpreadFactor"

	rateOfSpread *= (m_pppMap[tileTo.y][tileTo.x][tileTo.z] == m_pppMap[origin.y][origin.x][origin.z]) || (originTile.HasDoor() && tileData.HasDoor());
	rateOfSpread += (rateOfWallSpread * (offset.y + 1) + rateOfFloorSpread) * (m_pppMap[tileTo.y][tileTo.x][tileTo.z] != 1);
	
	if (offset.y == 0)
	{
		bool spreadingThroughBulkhead = glm::min<uint32>(origin.z, tileTo.z) % tilesBetweenBulkheads == 0;
		rateOfSpread *= CalculateDoorSpreadFactor(m_pWorld->GetLevel(origin.y)->GetLevelData(), origin, tileTo, spreadingThroughBulkhead, rateOfNormalDoorSpread, rateOfBulkheadDoorSpreadFactor, rateOfBulkheadSpreadFactor);
		rateOfSpread *= CalculateBulkheadSpreadFactor(spreadingThroughBulkhead, rateOfBulkheadSpreadFactor);
	}

	rateOfSpread /= (1.0f + (tileData.Temp / 100.0f));
	tileData.Temp += std::max((originTile.Temp - tileData.BurnsAt) * rateOfSpread * dtS, 0.0f);
	tileData.Temp = std::min(tileData.Temp, 1000.0f);

	if (tileData.Temp >= tileData.BurnsAt && !tileData.Burning)
	{
		m_OnFire.push_back(tileTo);
		tileData.Burning = true;

		if (tileData.GameObjects[GAMEOBJECT_CONST_INDEX_FIRE] != nullptr)
		{
			if (tileData.WaterLevel < WATER_UPDATE_LEVEL_INTERVAL)
			{
				tileData.GameObjects[GAMEOBJECT_CONST_INDEX_FIRE]->SetIsVisible(m_DiscoveredRooms[m_pppMap[tileTo.y][tileTo.x][tileTo.z]] || m_FireAlwaysVisible);
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

bool ScenarioFire::SpreadSmokeSideways(float dtS, const glm::ivec3& offset, const glm::ivec3& origin, float amount, Scene* scene)
{
	bool res = false;
	glm::ivec3 tileTo = origin + offset;
	TileData& originTile = m_pWorld->GetLevel(origin.y)->GetLevelData()[origin.x][origin.z];
	TileData& tileData = m_pWorld->GetLevel(origin.y)->GetLevelData()[tileTo.x][tileTo.z];
	TileData& lowerTileData = m_pWorld->GetLevel(origin.y - 1)->GetLevelData()[tileTo.x][tileTo.z];
	if (tileData.SmokeAmount * dtS < amount * 4.0f)
	{
		bool filled = tileData.SmokeAmount >= tileData.SmokeLimit;

		bool hasOpenDoor = lowerTileData.HasDoor();
		if (hasOpenDoor)
		{
			hasOpenDoor = !reinterpret_cast<GameObjectDoor*>(lowerTileData.GameObjects[GAMEOBJECT_CONST_INDEX_DOOR])->IsClosed();
		}
		
		//HasDoor and hasStairs never set?
		if (m_pppMap[tileTo.y][tileTo.x][tileTo.z] == m_pppMap[origin.y][origin.x][origin.z] || hasOpenDoor || lowerTileData.HasStairs)
		{
			tileData.SmokeAmount += amount;
			if (!filled && tileData.SmokeAmount >= tileData.SmokeLimit)
			{
				m_Smoke.push_back(tileTo);

				if (tileData.GameObjects[GAMEOBJECT_CONST_INDEX_SMOKE] != nullptr)
				{
					if (lowerTileData.WaterLevel < 0.5f * WATER_MAX_LEVEL)
					{
						tileData.GameObjects[GAMEOBJECT_CONST_INDEX_SMOKE]->SetIsVisible(m_DiscoveredRooms[m_pppMap[tileTo.y][tileTo.x][tileTo.z]] || m_FireAlwaysVisible);
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
							uint32 id = m_pppMap[tileTo.y][tileTo.x][tileTo.z];
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