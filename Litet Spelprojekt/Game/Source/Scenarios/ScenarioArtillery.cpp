#include "..\..\Include\Scenarios\ScenarioArtillery.h"
#include "..\..\Include\Scenarios\ScenarioManager.h"
#include "..\..\Include\Game.h"
#include <System/Random.h>
#include <World/Logger.h>
#include <Audio/Sources/AudioSource.h>

ScenarioArtillery::ScenarioArtillery(uint32 numInstances)
	: m_pAudioSourceExplosion(nullptr),
	m_InstancesToSpawn(numInstances),
	m_InstancesComplete(0)
{

}

ScenarioArtillery::~ScenarioArtillery()
{
	DeleteSafe(m_pAudioSourceExplosion);
}

void ScenarioArtillery::Init(World* pWorld) noexcept
{
	SetTimeOfNextOutBreak(1.0f);
}

void ScenarioArtillery::Release() noexcept
{
}

void ScenarioArtillery::OnStart(SceneGame* scene) noexcept
{
	glm::vec3 pos = glm::vec3(Random::GenerateInt(-200, 200), 200, Random::GenerateInt(-200, 200));
	m_Target = glm::vec3(Random::GenerateInt(2, 9), 4, Random::GenerateInt(2, 39));
	m_pGameObjectArtillery = new GameObjectArtillery(pos, m_Target);

	scene->AddGameObject(m_pGameObjectArtillery);
}

void ScenarioArtillery::OnEnd(SceneGame* scene) noexcept
{
	scene->RemoveGameObject(m_pGameObjectArtillery);
	DeleteSafe(m_pGameObjectArtillery);
}

void ScenarioArtillery::OnVisibilityChange(World* pWorld, SceneGame* pScene) noexcept
{
    
}

void ScenarioArtillery::Escalate(const glm::ivec3& position) noexcept
{

}

bool ScenarioArtillery::Update(float dtS, World* world, SceneGame* scene) noexcept
{
	if (m_pGameObjectArtillery)
	{
		if (m_pGameObjectArtillery->HasHitTarget())
		{
			ScenarioManager::Escalate(Game::GetGame()->m_ScenarioFire, m_Target);

			Logger::LogEvent("Båten blev träffad av artilleri!", false);

			MeshEmitter* pMeshEmitter = new MeshEmitter(1.0F, this);
			pMeshEmitter->SetMesh(MESH::MESH_PARTICLE);
			pMeshEmitter->SetTimeToLive(2.0f);
			pMeshEmitter->SetConeAngle(glm::radians<float>(100.0f));
			pMeshEmitter->SetSpeed(1.0f, 2.0f);
			pMeshEmitter->SetScale(glm::vec2(0.1f), glm::vec2(0.5f));
			pMeshEmitter->SetBeginColor(glm::vec4(0.1f, 0.1f, 0.1f, 1.0f));
			pMeshEmitter->SetEndColor(glm::vec4(0.05f, 0.05f, 0.05f, 1.0f));
			pMeshEmitter->SetPosition(m_Target);
			pMeshEmitter->SetParticlesPerSeconds(150);
			pMeshEmitter->UpdateTransform();
			pMeshEmitter->SetIsVisible(true);
			scene->AddGameObject(pMeshEmitter);

			m_pAudioSourceExplosion = AudioSource::CreateSoundSource(SOUND::MONO_EXPLOSION);
			m_pAudioSourceExplosion->SetRollOffFactor(10.0f);
			m_pAudioSourceExplosion->SetReferenceDistance(0.0f);
			m_pAudioSourceExplosion->SetMaxDistance(500.0f);
			m_pAudioSourceExplosion->SetLooping(false);
			m_pAudioSourceExplosion->Play();

			Crew* crew = scene->GetCrew();
			for (int i = 0; i < crew->GetCount(); i++)
			{
				if (crew->GetMember(i)->GetPosition().y >= 4)
				{
					float distance = glm::distance(crew->GetMember(i)->GetPosition(), m_Target);
					if (distance <= 4)
					{
						Logger::LogEvent(crew->GetMember(i)->GetName() + " blev skadad av artilleri!", false);
						crew->GetMember(i)->ApplyBurnInjury(4 - distance + 1);
						crew->GetMember(i)->ApplyBoneInjury(4 - distance + 1);
						crew->GetMember(i)->ApplyBleedInjury(4 - distance + 1);
					}
				}	
			}

			m_InstancesComplete++;
			return true;
		}
	
		return false;
	}

	return true;
}

void ScenarioArtillery::OnMeshEmitterKilled(MeshEmitter* emitter)
{
	DeleteSafe(m_pAudioSourceExplosion);
}

std::string ScenarioArtillery::GetName() noexcept
{
	return "Artilleriträff";
}

int32 ScenarioArtillery::GetCooldownTime() noexcept
{
	return 100;
}

int32 ScenarioArtillery::GetMaxTimeBeforeOutbreak() noexcept
{
	return 60 * 5;
}

bool ScenarioArtillery::IsComplete() noexcept
{
	return m_InstancesComplete >= m_InstancesToSpawn;
}
