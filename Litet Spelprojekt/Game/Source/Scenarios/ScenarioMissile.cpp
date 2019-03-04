#include "..\..\Include\Scenarios\ScenarioMissile.h"
#include "..\..\Include\Scenarios\ScenarioManager.h"
#include "..\..\Include\Game.h"
#include <System/Random.h>
#include <World/Logger.h>
#include <Audio/Sources/AudioSource.h>

ScenarioMissile::ScenarioMissile() : pAudioSourceExplosion(nullptr)
{

}

ScenarioMissile::~ScenarioMissile()
{
	DeleteSafe(pAudioSourceExplosion);
}

void ScenarioMissile::Init(World* pWorld) noexcept
{

}

void ScenarioMissile::OnStart(SceneGame* scene) noexcept
{
	glm::vec3 pos = glm::vec3(Random::GenerateInt(-200, 200), 200, Random::GenerateInt(-200, 200));
	m_Target = glm::vec3(Random::GenerateInt(2, 9), 4, Random::GenerateInt(2, 39));
	m_pGameObjectMissile = new GameObjectMissile(pos, m_Target);

	scene->AddGameObject(m_pGameObjectMissile);
}

void ScenarioMissile::OnEnd(SceneGame* scene) noexcept
{
	scene->RemoveGameObject(m_pGameObjectMissile);
	DeleteSafe(m_pGameObjectMissile);
}

void ScenarioMissile::Escalate(const glm::ivec3& position) noexcept
{

}

void ScenarioMissile::OnVisibilityChange(World* pWorld, SceneGame* pScene, const std::vector<uint32>& activeRooms) noexcept
{

}

bool ScenarioMissile::Update(float dtS, World* world, SceneGame* scene, const std::vector<uint32>& activeRooms) noexcept
{
	if (m_pGameObjectMissile)
	{
		if (m_pGameObjectMissile->HasHitTarget())
		{
			ScenarioManager::Escalate(Game::GetGame()->m_ScenarioFire, m_Target);

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

			pAudioSourceExplosion = AudioSource::CreateSoundSource(SOUND::MONO_EXPLOSION);
			pAudioSourceExplosion->SetRollOffFactor(10.0f);
			pAudioSourceExplosion->SetReferenceDistance(0.0f);
			pAudioSourceExplosion->SetMaxDistance(500.0f);
			pAudioSourceExplosion->SetLooping(false);
			pAudioSourceExplosion->Play();

			Crew* crew = scene->GetCrew();
			for (int i = 0; i < crew->GetCount(); i++)
			{
				float distance = glm::distance(crew->GetMember(i)->GetPosition(), m_Target);
				if (distance <= 10)
				{
					crew->GetMember(i)->ApplyBurnInjury(10 - distance + 1);
					crew->GetMember(i)->ApplyBoneInjury();
					Logger::LogEvent(crew->GetMember(i)->GetName() + " was hit!", false);
				}
			}

			return true;
		}
		return false;
	}
	return true;
}

void ScenarioMissile::OnMeshEmitterKilled(MeshEmitter* emitter)
{
	DeleteSafe(pAudioSourceExplosion);
}

std::string ScenarioMissile::GetName() noexcept
{
	return "Missilträff";
}

int32 ScenarioMissile::GetCooldownTime() noexcept
{
	return 100;
}

int32 ScenarioMissile::GetMaxTimeBeforeOutbreak() noexcept
{
	return 60 * 5;
}