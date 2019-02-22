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

	ParticleEmitter* pEmitter = new ParticleEmitter();
	pEmitter->SetPosition(glm::vec3(pos.x, pos.y, pos.z));
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
		tileData.SmokeAmount = std::min(tileData.SmokeAmount, 400.0f);

		if (!alreadySmoke && tileData.SmokeAmount >= tileData.SmokeLimit)
		{
			m_Smoke.push_back(pos + glm::ivec3(0.0, (pos.y + 1) % 2, 0.0f));

			ParticleEmitter* pEmitter = new ParticleEmitter();
			pEmitter->SetParticleBlendMode(PARTICLE_NORMAL);
			pEmitter->SetTexture(TEXTURE::SMOKE);
			pEmitter->SetTimeToLive(7.0f);
			pEmitter->SetConeAngle(glm::radians<float>(40.0f));
			pEmitter->SetSpeed(0.1f, 0.4f);
			pEmitter->SetScale(glm::vec2(0.5f), glm::vec2(5.0f));
			pEmitter->SetBeginColor(glm::vec4(0.2f, 0.2f, 0.2f, 0.3f));
			pEmitter->SetEndColor(glm::vec4(0.05f, 0.05f, 0.05f, 0.3f));
			pEmitter->SetPosition(pos + glm::ivec3(0.0f, ((int32)pos.y + 1) % 2, 0.0f));
			pEmitter->SetParticlesPerSeconds(5);
			pEmitter->UpdateTransform();
			scene->AddGameObject(pEmitter);

			TileData& lowerTileData =  m_pWorld->GetLevel((int32)pos.y)->GetLevelData()[(int32)pos.x][(int32)pos.z];

			for (uint32 j = 1; j < tileData.GameObjects.size(); j++)
			{
				tileData.GameObjects[j]->OnSmokeDetected();
			}
			for (uint32 j = 1; j < lowerTileData.GameObjects.size(); j++)
			{
				lowerTileData.GameObjects[j]->OnSmokeDetected();
			}
		}
	}

	uint32 max = m_Smoke.size();
	if (max > 1)
		bool hej = true;
	float rateOfSpread = 1.0f;
	for (uint32 j = 0; j < max; j++)
	{
		glm::ivec3& smoke = m_Smoke[j];
		TileData& data = m_pWorld->GetLevel((int32)smoke.y)->GetLevelData()[smoke.x][smoke.z];

		float spread = data.SmokeAmount - data.SmokeLimit;
		spread /= 4;
		spread *= dtS * rateOfSpread;
		dtS *= rateOfSpread;
		uint32 rest = 0;
		if (spread > 0.0f)
		{
			glm::ivec3 smokeOriginPos = glm::ivec3(smoke) /*+ glm::ivec3(0.0, (smoke.y + 1) % 2, 0.0f)*/;

			rest += CheckSmoke(dtS, glm::ivec3(1, 0, 0), smokeOriginPos, spread, scene);
			rest += CheckSmoke(dtS, glm::ivec3(-1, 0, 0), smokeOriginPos, spread, scene);
			rest += CheckSmoke(dtS, glm::ivec3(0, 0, 1), smokeOriginPos, spread, scene);
			rest += CheckSmoke(dtS, glm::ivec3(0, 0, -1), smokeOriginPos, spread, scene);
			data.SmokeAmount -= spread * rest;
		}
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
	float rateOfSpread = 0.01f;
	float rateOfWallSpread = 0.02;
	float rateOfFloorSpread = 0.001;

	rateOfSpread *= (m_pppMap[origin.y + offset.y][origin.x + offset.x][origin.z + offset.z] == m_pppMap[origin.y][origin.x][origin.z] || (tileData.HasDoor && originTile.HasDoor));
	rateOfSpread += (rateOfWallSpread * (offset.y + 1) + rateOfFloorSpread) * (m_pppMap[origin.y + offset.y][origin.x + offset.x][origin.z + offset.z] != 1);

	rateOfSpread /= (1 + (tileData.Temp/100));
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

		for (uint32 i = 1; i < tileData.GameObjects.size(); i++)
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

bool ScenarioFire::CheckSmoke(float dtS, const glm::ivec3 & offset, const glm::ivec3 & origin, float amount, Scene* scene)
{
	bool res = false;
	TileData& tileData = m_pWorld->GetLevel(origin.y)->GetLevelData()[origin.x + offset.x][origin.z + offset.z];
	TileData& lowerTileData = m_pWorld->GetLevel(origin.y - 1)->GetLevelData()[origin.x + offset.x][origin.z + offset.z];
	bool filled = tileData.SmokeAmount >= tileData.SmokeLimit;
	//HasDoor and hasStairs never set?
	if (m_pppMap[origin.y + offset.y][origin.x + offset.x][origin.z + offset.z] == m_pppMap[origin.y][origin.x][origin.z] || lowerTileData.HasDoor || lowerTileData.HasStairs)
	{
		tileData.SmokeAmount += amount;
		if (!filled && tileData.SmokeAmount >= tileData.SmokeLimit)
		{
			m_Smoke.push_back(origin + offset);

			//GameObject* tmp = new GameObject();
			//tmp->SetMesh(MESH::SPHERE);
			//tmp->SetMaterial(MATERIAL::GREEN);
			//
			//tmp->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
			//scene->AddGameObject(tmp);

			ParticleEmitter* pEmitter = new ParticleEmitter();
			pEmitter->SetParticleBlendMode(PARTICLE_NORMAL);
			pEmitter->SetTexture(TEXTURE::SMOKE);
			pEmitter->SetTimeToLive(7.0f);
			pEmitter->SetConeAngle(glm::radians<float>(40.0f));
			pEmitter->SetSpeed(0.1f, 0.4f);
			pEmitter->SetScale(glm::vec2(0.5f), glm::vec2(5.0f));
			pEmitter->SetBeginColor(glm::vec4(0.2f, 0.2f, 0.2f, 0.3f));
			pEmitter->SetEndColor(glm::vec4(0.05f, 0.05f, 0.05f, 0.3f));
			pEmitter->SetPosition(origin + offset + glm::ivec3(0.0f, ((int32)(origin + offset).y + 1) % 2, 0.0f));
			pEmitter->SetParticlesPerSeconds(5);
			pEmitter->UpdateTransform();
			scene->AddGameObject(pEmitter);

			if (origin + offset == glm::ivec3(-2 + (m_pWorld->GetLevel(0)->GetSizeX() / 2), 1, -4 + m_pWorld->GetLevel(0)->GetSizeZ() / 2))
				bool hej = true;
			for (uint32 j = 1; j < tileData.GameObjects.size(); j++)
			{
				tileData.GameObjects[j]->OnSmokeDetected();
			}
			for (uint32 j = 1; j < lowerTileData.GameObjects.size(); j++)
			{
				lowerTileData.GameObjects[j]->OnSmokeDetected();
			}
		}

		res = true;
	}

	return res;
}
