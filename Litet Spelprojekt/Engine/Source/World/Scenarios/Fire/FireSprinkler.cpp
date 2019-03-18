#include <EnginePch.h>
#include <World/Scenarios/Fire/FireSprinkler.h>
#include <System/Application.h>
#include <World/World.h>

FireSprinkler::FireSprinkler(int32 source) : GameObject(),
	m_pScene(nullptr),
	m_HasDetectedSmoke(false),
	m_pParticleEmitter(nullptr),
	m_MinBounds(FLT_MAX, 0.0f, FLT_MAX),
	m_MaxBounds(-FLT_MAX, 0.0f, -FLT_MAX)
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
	m_HasDetectedSmoke = false;
	m_pAudioSrc->Stop();

	if (m_pParticleEmitter != nullptr)
	{
		m_pParticleEmitter->SetIsVisible(false);
	}
}

void FireSprinkler::OnAddedToScene(Scene* pScene) noexcept
{
	m_pScene = pScene;
}

void FireSprinkler::Update(const Camera& camera, float dt) noexcept
{
	if (!m_HasDetectedSmoke)
	{
		return;
	}

	glm::ivec3 tilePos = GetTile();

	WorldLevel& worldLevel = m_pWorld->GetLevel(tilePos.y);
	uint32 levelSizeX = worldLevel.GetSizeX();
	uint32 levelSizeZ = worldLevel.GetSizeZ();

	const uint32* const * ppLevel = worldLevel.GetLevel();
	TileData* const * ppLevelData = worldLevel.GetLevelData();

	uint32 currentRoomIndex = ppLevel[tilePos.x][tilePos.z];
	bool allFireInRoomExtinguished = true;

	for (uint32 x = 0; x < levelSizeX; x++)
	{
		for (uint32 z = 0; z < levelSizeZ; z++)
		{
			TileData& tile = ppLevelData[x][z];

			if (IsInsideBoundingBox(glm::ivec2(x, z)))
			{
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

			if (currentRoomIndex == ppLevel[x][z])
			{
				if (tile.Temp > tile.BurnsAt)
				{
					allFireInRoomExtinguished = false;
				}
			}
		}
	}

	if (allFireInRoomExtinguished)
	{
		TurnOff();
	}
}

void FireSprinkler::OnSmokeDetected() noexcept
{
	m_HasDetectedSmoke = true;

	glm::ivec3 tilePos = GetTile();
	WorldLevel& worldLevel = m_pWorld->GetLevel(tilePos.y);
	uint32 levelSizeX = worldLevel.GetSizeX();
	uint32 levelSizeZ = worldLevel.GetSizeZ();

	const uint32* const * ppLevel = worldLevel.GetLevel();
	TileData* const * ppLevelData = worldLevel.GetLevelData();

	uint32 currentRoomIndex = ppLevel[tilePos.x][tilePos.z];


	for (uint32 x = 0; x < levelSizeX; x++)
	{
		for (uint32 z = 0; z < levelSizeZ; z++)
		{
			if (currentRoomIndex == ppLevel[x][z])
			{
				TileData& tile = ppLevelData[x][z];

				m_MinBounds.x = glm::min<float>(m_MinBounds.x, (float)x);
				m_MinBounds.z = glm::min<float>(m_MinBounds.z, (float)z);
				m_MaxBounds.x = glm::max<float>(m_MaxBounds.x, (float)x);
				m_MaxBounds.z = glm::max<float>(m_MaxBounds.z, (float)z);

				for (uint32 i = tile.NrOfBaseGameObjects; i < tile.GameObjects.size(); i++)
				{
					FireSprinkler* pFireSprinkler = dynamic_cast<FireSprinkler*>(tile.GameObjects[i]);

					if (pFireSprinkler != nullptr)
					{
						if (!pFireSprinkler->HasDetectedSmoke())
						{
							pFireSprinkler->OnSmokeDetected();
						}
					}
				}
			}
		}
	}

	if (m_pScene != nullptr)
	{
		if (m_pParticleEmitter == nullptr)
		{
			glm::vec3 emitterPos = GetPosition();
			m_MinBounds.y = emitterPos.y + 0.2f;
			m_MaxBounds.y = emitterPos.y + 1.9f;
			emitterPos.y += 1.85f;
			m_pParticleEmitter = new MeshEmitter();
			m_pParticleEmitter->SetMesh(MESH::MESH_PARTICLE);
			m_pParticleEmitter->SetTimeToLive(0.9f);
			m_pParticleEmitter->SetConeAngle(glm::atan(SPRINKLER_RADIUS / 2.0f));
			m_pParticleEmitter->SetSpeed(5.0f, 6.0f);
			m_pParticleEmitter->SetScale(glm::vec2(0.05f), glm::vec2(0.08f));
			m_pParticleEmitter->SetBeginColor(glm::vec4(0.0f, 0.0f, 0.8f, 1.0f));
			m_pParticleEmitter->SetEndColor(glm::vec4(0.4f, 0.4f, 1.0f, 1.0f));
			m_pParticleEmitter->SetPosition(emitterPos);
			m_pParticleEmitter->SetRotation(glm::vec4(1.0f, 0.0f, 0.0f, glm::radians<float>(180.0f)));
			m_pParticleEmitter->SetParticlesPerSeconds(300);
			m_pParticleEmitter->SetBoundingBox(m_MinBounds, m_MaxBounds);
			m_pParticleEmitter->UpdateTransform();
			m_pScene->AddGameObject(m_pParticleEmitter);
		}
		else
		{
			m_pParticleEmitter->SetIsVisible(true);
		}
	}
	else
	{
		std::cout << "(FireSprinkler) Scene is null when smoke detected!" << std::endl;
	}
}
