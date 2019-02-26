#include "..\..\Include\Scenarios\ScenarioFire.h"
#include <System/Random.h>

void SpawnFire(Scene* pScene, const glm::vec3& position)
{
	MeshEmitter* pEmitter = new MeshEmitter();
	pEmitter->SetPosition(position);
	pEmitter->SetMesh(MESH::MESH_PARTICLE);
	pEmitter->SetTimeToLive(1.0f);
	pEmitter->SetScale(glm::vec2(0.05f), glm::vec2(0.15f));
	pEmitter->SetConeAngle(glm::radians<float>(30.0f));
	pEmitter->SetSpeed(0.7f, 2.0f);
	pEmitter->SetBeginColor(glm::vec4(1.0f, 1.0f, 0.3f, 1.0f));
	pEmitter->AddColorNode(glm::vec4(1.0f, 0.92f, 0.03f, 1.0f), 0.3f);
	pEmitter->SetEndColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	pEmitter->SetParticlesPerSeconds(20);
	pEmitter->UpdateTransform();
	pEmitter->SetIsVisible(false);

	pScene->AddGameObject(pEmitter);
}

void SpawnSmoke(Scene* pScene, const glm::vec3& position)
{
	MeshEmitter* pEmitter = new MeshEmitter();
	pEmitter->SetMesh(MESH::MESH_PARTICLE);
	pEmitter->SetTimeToLive(2.5f);
	pEmitter->SetConeAngle(glm::radians<float>(40.0f));
	pEmitter->SetSpeed(0.1f, 0.4f);
	pEmitter->SetScale(glm::vec2(0.1f), glm::vec2(0.3f));
	pEmitter->SetBeginColor(glm::vec4(0.2f, 0.2f, 0.2f, 0.3f));
	pEmitter->SetEndColor(glm::vec4(0.05f, 0.05f, 0.05f, 0.3f));
	pEmitter->SetPosition(position);
	pEmitter->SetParticlesPerSeconds(2);
	pEmitter->UpdateTransform();
	pEmitter->SetIsVisible(false);
	pScene->AddGameObject(pEmitter);
}

ScenarioFire::ScenarioFire(World* world)
{
	m_pWorld = world;

	m_pppMap = new const uint32* const*[m_pWorld->GetNumLevels()];

	for (uint32 i = 0; i < m_pWorld->GetNumLevels(); i++)
	{
		m_pppMap[i] = m_pWorld->GetLevel(i)->GetLevel();
	}
}

ScenarioFire::~ScenarioFire()
{
	DeleteArrSafe(m_pppMap);
}

void ScenarioFire::OnStart(Scene* scene) noexcept
{
	uint32 lvl = Random::GenerateInt(0, m_pWorld->GetNumLevels() - 1);
	lvl += lvl % 2;
	lvl = std::min(lvl, m_pWorld->GetNumLevels() - 1);
	lvl = 0;
	uint32 x = Random::GenerateInt(1, m_pWorld->GetLevel(lvl)->GetSizeX() - 2);
	x = m_pWorld->GetLevel(lvl)->GetSizeX() / 2;
	x -= 2;
	uint32 z = Random::GenerateInt(1, m_pWorld->GetLevel(lvl)->GetSizeZ() - 2);
	z = m_pWorld->GetLevel(lvl)->GetSizeZ() / 2;
	glm::ivec3 pos = glm::ivec3(x, lvl, z);
	m_OnFire.push_back(pos);

	TileData& tileData = m_pWorld->GetLevel(lvl)->GetLevelData()[x][z];
	tileData.Temp = 100.1f;
	tileData.Burning = true;

	SpawnFire(scene, pos);
}

void ScenarioFire::OnEnd() noexcept
{

}

void ScenarioFire::OnVisibilityChange(World * pWorld, Scene * pScene, const std::vector<uint32>& activeRooms) noexcept
{
}

bool ScenarioFire::Update(float dtS, World* world, Scene* scene, const std::vector<uint32>& activeRooms) noexcept
{
#if defined(PRINT_CPU_DEBUG_DATA)
	CPUProfiler::StartTimer(CPU_PROFILER_SLOT_4);
#endif


	// current, right, left, forward, back, up, down.
	for (uint32 i = 0; i < m_OnFire.size(); i++)
	{
		glm::ivec3 pos = m_OnFire[i];
		CheckFire(dtS, glm::ivec3(0, 0, 0), pos, scene);
		CheckFire(dtS, glm::ivec3(1, 0, 0), pos, scene);
		CheckFire(dtS, glm::ivec3(-1, 0, 0), pos, scene);
		CheckFire(dtS, glm::ivec3(0, 0, 1), pos, scene);
		CheckFire(dtS, glm::ivec3(0, 0, -1), pos, scene);

		if (pos.y < m_pWorld->GetNumLevels() - 1)
		{
			CheckFire(dtS, glm::ivec3(0, 1, 0), pos, scene);
		}

		if (pos.y - 1 > 0)
		{
			CheckFire(dtS, glm::ivec3(0, -1, 0), pos, scene);
		}

	}

	for (uint32 i = 0; i < m_OnFire.size(); i++)
	{
		glm::ivec3& pos = m_OnFire[i];
		TileData& tileData = m_pWorld->GetLevel((int32)pos.y + ((int32)pos.y + 1) % 2)->GetLevelData()[(int32)pos.x][(int32)pos.z];
		bool alreadySmoke = tileData.SmokeAmount >= tileData.SmokeLimit;

		tileData.SmokeAmount += m_pWorld->GetLevel((int32)pos.y)->GetLevelData()[(int32)pos.x][(int32)pos.z].Temp * 2.0f;
		tileData.SmokeAmount = std::min(tileData.SmokeAmount, 1000.0f);

		if (!alreadySmoke && tileData.SmokeAmount >= tileData.SmokeLimit)
		{
			m_Smoke.push_back(pos + glm::ivec3(0.0, (pos.y + 1) % 2, 0.0f));

			SpawnSmoke(scene, pos + glm::ivec3(0.0f, ((int32)pos.y + 1) % 2, 0.0f));
			TileData& lowerTileData = m_pWorld->GetLevel((int32)pos.y)->GetLevelData()[(int32)pos.x][(int32)pos.z];

			for (uint32 i = 2; i < lowerTileData.GameObjects.size(); i++)
			{
				lowerTileData.GameObjects[i]->OnSmokeDetected();
			}
		}
	}

	uint32 max = m_Smoke.size();
	float rateOfSpread = 0.1f;
	for (uint32 j = 0; j < max; j++)
	{
		glm::ivec3& smoke = m_Smoke[j];
		TileData& data = m_pWorld->GetLevel((int32)smoke.y)->GetLevelData()[smoke.x][smoke.z];

		float spread = data.SmokeAmount - data.SmokeLimit;
		//spread /= 4;
		spread *= dtS * rateOfSpread;
		uint32 rest = 0;
		if (spread > 0.0001f)
		{
			glm::ivec3 smokeOriginPos = glm::ivec3(smoke) /*+ glm::ivec3(0.0, (smoke.y + 1) % 2, 0.0f)*/;

			rest += CheckSmoke(dtS, glm::ivec3(1, 0, 0), smokeOriginPos, spread, scene);
			rest += CheckSmoke(dtS, glm::ivec3(-1, 0, 0), smokeOriginPos, spread, scene);
			rest += CheckSmoke(dtS, glm::ivec3(0, 0, 1), smokeOriginPos, spread, scene);
			rest += CheckSmoke(dtS, glm::ivec3(0, 0, -1), smokeOriginPos, spread, scene);
			data.SmokeAmount -= spread * rest;
		}
	}
#if defined(PRINT_CPU_DEBUG_DATA)
	CPUProfiler::EndTimer("Fire Scenario Update took %.3f ms", CPU_PROFILER_SLOT_4);
#endif
	return m_OnFire.empty();
}

std::string ScenarioFire::GetName() noexcept
{
	return "Fire";
}

int32 ScenarioFire::GetCooldownTime() noexcept
{
	return 20;
}

int32 ScenarioFire::GetMaxTimeBeforeOutbreak() noexcept
{
	return 60;
}

void ScenarioFire::CheckFire(float dtS, const glm::ivec3& offset, const glm::ivec3& origin, Scene* scene)
{
	TileData& originTile = m_pWorld->GetLevel(origin.y)->GetLevelData()[origin.x][origin.z];
	TileData& tileData = m_pWorld->GetLevel(origin.y + offset.y)->GetLevelData()[origin.x + offset.x][origin.z + offset.z];
	float rateOfSpread = 0.1f;
	float rateOfWallSpread = 0.02;
	float rateOfFloorSpread = 0.001;

	rateOfSpread *= (m_pppMap[origin.y + offset.y][origin.x + offset.x][origin.z + offset.z] == m_pppMap[origin.y][origin.x][origin.z] || (tileData.HasDoor() && originTile.HasDoor()));
	rateOfSpread += (rateOfWallSpread * (offset.y + 1) + rateOfFloorSpread) * (m_pppMap[origin.y + offset.y][origin.x + offset.x][origin.z + offset.z] != 1);

	rateOfSpread /= (1 + (tileData.Temp/100));
	tileData.Temp += std::max((originTile.Temp - tileData.BurnsAt) * rateOfSpread * dtS, 0.0f);

	if (tileData.Temp >= tileData.BurnsAt && !tileData.Burning)
	{
		m_OnFire.push_back(glm::vec3(origin) + glm::vec3(offset));
		tileData.Burning = true;

		SpawnFire(scene, glm::vec3(origin) + glm::vec3(offset) + glm::vec3(0.0f, 0.5f, 0.0f));

		for (uint32 i = 2; i < tileData.GameObjects.size(); i++)
		{
			tileData.GameObjects[i]->OnFireDetected();
		}
	}
	else if (tileData.Temp < tileData.BurnsAt && tileData.Burning)
	{
		tileData.Burning = false;
		for (uint32 i = 0; i < m_OnFire.size(); i++)
		{
			if (m_OnFire[i] == glm::ivec3(origin) + glm::ivec3(offset))
			{
				m_OnFire.erase(m_OnFire.begin() + i);
			}
		}
	}
}

bool ScenarioFire::CheckSmoke(float dtS, const glm::ivec3& offset, const glm::ivec3& origin, float amount, Scene* scene)
{
	bool res = false;
	TileData& tileData = m_pWorld->GetLevel(origin.y)->GetLevelData()[origin.x + offset.x][origin.z + offset.z];
	TileData& lowerTileData = m_pWorld->GetLevel(origin.y - 1)->GetLevelData()[origin.x + offset.x][origin.z + offset.z];
	if (tileData.SmokeAmount * dtS < amount * 4)
	{
		bool filled = tileData.SmokeAmount >= tileData.SmokeLimit;
		
		//HasDoor and hasStairs never set?
		if (m_pppMap[origin.y + offset.y][origin.x + offset.x][origin.z + offset.z] == m_pppMap[origin.y][origin.x][origin.z] || lowerTileData.HasDoor() || lowerTileData.HasStairs)
		{
			tileData.SmokeAmount += amount;
			if (!filled && tileData.SmokeAmount >= tileData.SmokeLimit)
			{
				m_Smoke.push_back(origin + offset);

				SpawnSmoke(scene, origin + offset + glm::ivec3(0.0f, ((int32)(origin + offset).y + 1) % 2, 0.0f));
			}
			res = true;

			for (uint32 i = 2; i < lowerTileData.GameObjects.size(); i++)
			{
				lowerTileData.GameObjects[i]->OnSmokeDetected();
			}
		}
	}
	else
	{
		res = false;
	}
	return res;
}
