#include "..\..\Include\Scenarios\ScenarioIceberg.h"
#include "..\..\Include\Scenarios\ScenarioManager.h"
#include "..\..\Include\Game.h"
#include <System/Random.h>
#include <World/Logger.h>
#include <Audio/Sources/AudioSource.h>

ScenarioIceberg::ScenarioIceberg() : m_pAudioSourceExplosion(nullptr)
{
	
}

ScenarioIceberg::~ScenarioIceberg()
{
	DeleteSafe(m_pAudioSourceExplosion);
}

void ScenarioIceberg::BeginReplay(SceneGame* pScene, void* userData) noexcept
{
	auto pPair = (std::pair<glm::vec3, glm::vec3>*)userData;
	LaunchIceberg(pPair->first, pPair->second);
	ScenarioManager::StartScenario(this);
}

void ScenarioIceberg::Init(World* pWorld) noexcept
{
	m_pAudioSourceExplosion = AudioSource::CreateSoundSource(SOUND::MONO_CRASH);
	m_pAudioSourceExplosion->SetRollOffFactor(10.0f);
	m_pAudioSourceExplosion->SetReferenceDistance(0.0f);
	m_pAudioSourceExplosion->SetMaxDistance(500.0f);
	m_pAudioSourceExplosion->SetLooping(false);
}

void ScenarioIceberg::Release() noexcept
{
	DeleteSafe(m_pAudioSourceExplosion);
}

void ScenarioIceberg::OnStart(SceneGame* scene) noexcept
{
	if (!IsReplaying())
	{
		glm::vec3 centre;

		if (Random::GenerateBool())
		{
			m_Position = glm::vec3(-40, 0.0F, 150);
			centre = glm::vec3(0, 0, Random::GenerateInt(5, 35));
		}
		else
		{
			m_Position = glm::vec3(40 + 10, 0.0F, 150);
			centre = glm::vec3(10, 0, Random::GenerateInt(5, 35));
		}

		glm::vec3 ray = glm::normalize(m_Position - centre);
		m_Target = m_Position + ray * (float)(TestAgainstRay(ray, m_Position) + 10);

		LaunchIceberg(m_Position, m_Target);
		RegisterReplayEvent(new std::pair<glm::vec3, glm::vec3>(m_Position, m_Target));
	}
}

void ScenarioIceberg::OnEnd(SceneGame* scene) noexcept
{
	WaterOutdoorMaterial* pMaterial = reinterpret_cast<WaterOutdoorMaterial*> (ResourceHandler::GetMaterial(MATERIAL::WATER_OUTDOOR));
	pMaterial->SetIcebergPosition(glm::vec2(FLT_MAX, FLT_MAX));
}

void ScenarioIceberg::OnVisibilityChange(World* pWorld, SceneGame* pScene) noexcept
{

}

void ScenarioIceberg::Escalate(const glm::ivec3& position) noexcept
{

}

bool ScenarioIceberg::Update(float dtS, World* world, SceneGame* scene) noexcept
{
	float step = m_Velocity * dtS;
	m_DistanceTraveled += step;
	m_Position += step * m_Direction;

	WaterOutdoorMaterial* pMaterial = reinterpret_cast<WaterOutdoorMaterial*> (ResourceHandler::GetMaterial(MATERIAL::WATER_OUTDOOR));
	pMaterial->SetIcebergPosition(glm::vec2(m_Position.x, m_Position.z));

	if (m_DistanceTraveled >= m_TotalDistance)
	{
		if (!m_HasBounced)
		{
			m_Direction.x *= -1;
			m_TotalDistance = 150;
			m_DistanceTraveled = 0.0F;
			m_HasBounced = true;

			glm::vec3 target = glm::vec3(glm::clamp(m_Target.x, 1.0f, 10.0f), m_Target.y, glm::clamp(m_Target.z, 1.0f, 40.0f));

			ScenarioManager::Escalate(Game::GetGame()->m_ScenarioWater, target);

			m_pAudioSourceExplosion->Play();
			Logger::LogEvent("Båten fick hål i skrovet av ett isberg", false);

			Crew* crew = scene->GetCrew();
			for (int i = 0; i < crew->GetCount(); i++)
			{
				float distance = glm::distance(crew->GetMember(i)->GetPosition(), target);
				if (distance <= 2)
				{
					Logger::LogEvent(crew->GetMember(i)->GetName() + " blev skadad av ett isberg!", false);
					crew->GetMember(i)->ApplyBoneInjury(2 - distance + 1);
					crew->GetMember(i)->ApplyBleedInjury(2 - distance + 1);
				}
			}
		}
		else
		{
			return true;
		}
	}
	return false;
}

std::string ScenarioIceberg::GetName() noexcept
{
	return "Isberg";
}

int32 ScenarioIceberg::GetCooldownTime() noexcept
{
	return 100;
}

int32 ScenarioIceberg::GetMaxTimeBeforeOutbreak() noexcept
{
	return 60 * 5;
}

int32 ScenarioIceberg::TestAgainstRay(const glm::vec3 ray, const glm::vec3 origin) noexcept
{
	glm::vec3 centre = glm::vec3(6, 0, 21);

	glm::vec3 normals[]{
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
	};

	float size[] = {
		5,
		20
	};

	float d1[] = {
		glm::dot(centre - normals[0] * size[0], normals[0]),
		glm::dot(centre - normals[1] * size[1], normals[1])
	};
	float d2[] = {
		glm::dot(centre + normals[0] * size[0], normals[0]),
		glm::dot(centre + normals[1] * size[1], normals[1])
	};

	float t1[2];
	float t2[2];
	float t_min[2];
	float t_max[2];

	for (int j = 0; j < 2; j++)
	{
		if (std::abs(glm::dot(normals[j], ray)) > 0.01)
		{
			t1[j] = (d1[j] - glm::dot(normals[j], origin)) / glm::dot(normals[j], ray);
			t2[j] = (d2[j] - glm::dot(normals[j], origin)) / glm::dot(normals[j], ray);

			t_min[j] = std::min(t1[j], t2[j]);
			t_max[j] = std::max(t1[j], t2[j]);
		}
	}

	return std::min(t_max[0], t_max[1]);
}

void ScenarioIceberg::LaunchIceberg(glm::vec3 position, glm::vec3 target) noexcept
{
	m_Position = position;
	m_Target = target;

	glm::vec3 delta = m_Target - m_Position;
	m_TotalDistance = glm::length(delta);
	m_Direction = glm::normalize(delta);
	m_Velocity = 10;
	m_HasBounced = false;
	m_DistanceTraveled = 0;
}
