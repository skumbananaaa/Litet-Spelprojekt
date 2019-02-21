#include "..\..\Include\Scenarios\ScenarioFire.h"
#include <System/Random.h>

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

void ScenarioFire::OnStart() noexcept
{
	uint32 lvl = Random::GenerateInt(0, m_pWorld->GetNumLevels() - 1);
	uint32 x = Random::GenerateInt(1, m_pWorld->GetLevel(lvl)->GetSizeX() - 2);
	uint32 z = Random::GenerateInt(1, m_pWorld->GetLevel(lvl)->GetSizeZ() - 2);
	m_OnFire.push_back(glm::vec3(x, lvl, z));

	TileData& tileData = m_pWorld->GetLevel(lvl)->GetLevelData()[x][z];
	tileData.Temp = 100.1f;
}

void ScenarioFire::OnEnd() noexcept
{

}

void ScenarioFire::OnVisibilityChange(World * pWorld, Scene * pScene, const std::vector<uint32>& activeRooms) noexcept
{
}

bool ScenarioFire::Update(float dtS, World* world, Scene* scene, const std::vector<uint32>& activeRooms) noexcept
{
	// current, right, left, forward, back, up, down.
	for (uint32 i = 0; i < m_OnFire.size(); i++)
	{
		glm::vec3 curr = m_OnFire[i];
		CheckFire(dtS, glm::ivec3(0, 0, 0), curr, scene);
		CheckFire(dtS, glm::ivec3(1, 0, 0), curr, scene);
		CheckFire(dtS, glm::ivec3(-1, 0, 0), curr, scene);
		CheckFire(dtS, glm::ivec3(0, 0, 1), curr, scene);
		CheckFire(dtS, glm::ivec3(0, 0, -1), curr, scene);

		if (curr.y < m_pWorld->GetNumLevels() - 1)
		{
			CheckFire(dtS, glm::ivec3(0, 1, 0), curr, scene);
		}

		if (curr.y - 1 > 0)
		{
			CheckFire(dtS, glm::ivec3(0, -1, 0), curr, scene);
		}

		/*glm::ivec3 pos = glm::ivec3((int32)m_OnFire[i].x, (int32)m_OnFire[i].y + ((int32)m_OnFire[i].y + 1) % 2, (int32)m_OnFire[i].z);
		for (uint32 i = 0; i < m_OnFire.size(); i++)
		{
			TileData& tileData = m_pWorld->GetLevel((int32)m_OnFire[i].y + ((int32)m_OnFire[i].y + 1) % 2)->GetLevelData()[(int32)m_OnFire[i].x][(int32)m_OnFire[i].z];
			bool alreadySmoke = tileData.SmokeAmount >= tileData.SmokeLimit;

			tileData.SmokeAmount += m_pWorld->GetLevel((int32)m_OnFire[i].y)->GetLevelData()[(int32)m_OnFire[i].x][(int32)m_OnFire[i].z].Temp;
			tileData.SmokeAmount = std::min(tileData.SmokeAmount, 400.0f);

			if (!alreadySmoke && tileData.SmokeAmount >= tileData.SmokeLimit)
			{
				m_Smoke.push_back(pos);
			}
		}

		uint32 max = m_Smoke.size();
		for (uint32 i = 0; i < max; i++)
		{
			TileData& data = m_pWorld->GetLevel((int32)m_Smoke[i].y)->GetLevelData()[m_Smoke[i].x][m_Smoke[i].z];

			float spread = data.SmokeAmount - data.SmokeLimit;
			spread /= 4;
			spread *= dtS;
			uint32 rest = 0;
			rest += CheckSmoke(dtS, glm::ivec3(1, 0, 0), pos, spread);
			rest += CheckSmoke(dtS, glm::ivec3(-1, 0, 0), pos, spread);
			rest += CheckSmoke(dtS, glm::ivec3(0, 0, 1), pos, spread);
			rest += CheckSmoke(dtS, glm::ivec3(0, 0, -1), pos, spread);
			data.SmokeAmount -= spread * rest;
		}*/
	}

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

	rateOfSpread *= (m_pppMap[origin.y + offset.y][origin.x + offset.x][origin.z + offset.z] == m_pppMap[origin.y][origin.x][origin.z] || (tileData.HasDoor && originTile.HasDoor));
	rateOfSpread += (rateOfWallSpread * (offset.y + 1) + rateOfFloorSpread) * (m_pppMap[origin.y + offset.y][origin.x + offset.x][origin.z + offset.z] != 1);

	tileData.Temp += std::max((originTile.Temp - tileData.BurnsAt) * rateOfSpread * dtS, 0.0f);

	if (tileData.Temp >= tileData.BurnsAt && !tileData.Burning)
	{
		m_OnFire.push_back(glm::vec3(origin) + glm::vec3(offset));
		tileData.Burning = true;

		ParticleEmitter* pEmitter = new ParticleEmitter();
		pEmitter->SetPosition(glm::vec3(origin) + glm::vec3(offset) + glm::vec3(0.0f, 0.5f, 0.0f));
		pEmitter->SetParticleBlendMode(PARTICLE_ADDITIVE);
		pEmitter->SetTexture(TEXTURE::SMOKE);
		pEmitter->SetTimeToLive(1.2f);
		pEmitter->SetScale(glm::vec2(1.0f), glm::vec2(5.0f));
		pEmitter->SetConeAngle(glm::radians<float>(30.0f));
		pEmitter->SetSpeed(0.7f, 2.0f);
		pEmitter->SetBeginColor(glm::vec4(1.0f, 1.0f, 0.3f, 1.0f));
		pEmitter->AddColorNode(glm::vec4(1.0f, 0.92f, 0.03f, 1.0f), 0.3f);
		pEmitter->SetEndColor(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		pEmitter->SetParticlesPerSeconds(5);
		pEmitter->UpdateTransform();
		scene->AddGameObject(pEmitter);

		//GameObject* pGameObject;
		//{
		//	pGameObject = new GameObject();
		//	pGameObject->SetMaterial(MATERIAL::BOAT);
		//	pGameObject->SetMesh(MESH::CUBE_OBJ);
		//	pGameObject->SetPosition(glm::vec3(origin) + glm::vec3(offset) + glm::vec3(0.0f, 0.5f, 0.0f));
		//	pGameObject->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
		//	pGameObject->UpdateTransform();
		//	scene->AddGameObject(pGameObject);
		//}
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

bool ScenarioFire::CheckSmoke(float dtS, const glm::ivec3 & offset, const glm::ivec3 & origin, float amount)
{
	bool res = false;
	TileData& tileData = m_pWorld->GetLevel(origin.y + offset.y)->GetLevelData()[origin.x + offset.x][origin.z + offset.z];
	bool filled = tileData.SmokeAmount >= tileData.SmokeLimit;
	if (m_pppMap[origin.y + offset.y][origin.x + offset.x][origin.z + offset.z] == m_pppMap[origin.y][origin.x][origin.z] || m_pppMap[origin.y][origin.x][origin.z] == 0 || m_pppMap[origin.y + offset.y][origin.x + offset.x][origin.z + offset.z] == 0)
	{
		tileData.SmokeAmount += amount;
		if (!filled && tileData.SmokeAmount >= tileData.SmokeLimit)
		{
			m_Smoke.push_back(origin + offset);
		}
		res = true;
	}
	return res;
}
