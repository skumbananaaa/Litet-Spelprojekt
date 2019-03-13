#include <EnginePch.h>
#include <World/Scenarios/Fire/FireSprinkler.h>
#include <World/World.h>

FireSprinkler::FireSprinkler(int32 source) : GameObject(),
	m_HasDetectedSmoke(false)
{
	//Set particleemitters to be updated
	m_IsTickable = true;

	m_pAudioSrc = AudioSource::CreateSoundSource(source);
	m_pAudioSrc->SetRollOffFactor(10.0f);
	m_pAudioSrc->SetReferenceDistance(2.0f);
	m_pAudioSrc->SetMaxDistance(250.0f);
	m_pAudioSrc->SetLooping(true);
}

FireSprinkler::~FireSprinkler()
{
	DeleteSafe(m_pAudioSrc);
}

void FireSprinkler::SetPosition(const glm::vec3& position) noexcept
{
	GameObject::SetPosition(position);
	m_pAudioSrc->SetPosition(position);
}

bool FireSprinkler::HasDetectedSmoke() const noexcept
{
	return m_HasDetectedSmoke;
}

void FireSprinkler::TurnOff() noexcept
{
	m_pAudioSrc->Stop();
}

void FireSprinkler::Update(const Camera& camera, float dt) noexcept
{
	if (!m_HasDetectedSmoke)
	{
		return;
	}

	glm::ivec3 tilePos = GetTile();
	tilePos.y -= tilePos.y % 2;

	WorldLevel& worldLevel = m_pWorld->GetLevel(tilePos.y);
	uint32 levelSizeX = worldLevel.GetSizeX();
	uint32 levelSizeZ = worldLevel.GetSizeZ();

	TileData* const * ppLevelData = worldLevel.GetLevelData();

	for (int32 x = 0; x < levelSizeX; x++)
	{
		for (int32 z = 0; z < levelSizeZ; z++)
		{
			TileData& tile = ppLevelData[x][z];

			if (tile.Temp > tile.BurnsAt)
			{
				glm::vec2 toVector = glm::vec2(tilePos.x - x, tilePos.z - z);

				if (glm::length2(toVector) < SPRINKLER_RADIUS_SQRD)
				{
					tile.Temp -= FIRE_EXTINGUISH_BY_SPRINKLER_RATE * dt;
				}
			}

			for (uint32 i = 0; i < tile.GameObjects.size(); i++)
			{
				if (dynamic_cast <FireAlarm*>(tile.GameObjects[i]))
				{
					FireAlarm* pFireAlarm = (FireAlarm*)tile.GameObjects[i];

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
	}
}

void FireSprinkler::OnSmokeDetected() noexcept
{
	m_HasDetectedSmoke = true;
}
