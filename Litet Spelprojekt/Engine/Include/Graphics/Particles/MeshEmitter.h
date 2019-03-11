#pragma once
#include <Graphics/GameObject.h>
#include "ParticleData.h"
#include <Audio/Sources/AudioSource.h>

#define MAX_PARTICLES 1000

class MeshEmitter;

class API IMeshListener
{
public:
	virtual void OnMeshEmitterKilled(MeshEmitter* emitter) = 0;
};

class API MeshEmitter : public GameObject
{
	friend class ForwardRenderer;
	friend class DefferedRenderer;

public:
	MeshEmitter(MeshEmitter&& other) = delete;
	MeshEmitter(const MeshEmitter& other) = delete;
	MeshEmitter& operator=(MeshEmitter&& other) = delete;
	MeshEmitter& operator=(const MeshEmitter& other) = delete;

	MeshEmitter(float autoDeleteTimer = -1, IMeshListener* listerner = nullptr);
	~MeshEmitter();

	virtual void Update(const Camera& camera, float deltaTime) noexcept override;
	virtual void UpdateTransform() noexcept override;


	void SetMesh(uint32 meshID) noexcept;
	void SetParticlesPerSeconds(uint32 numParticles) noexcept;
	void SetConeAngle(float angleRad) noexcept;
	void SetPosition(const glm::vec3& position) noexcept;
	void SetScale(const glm::vec2& begin, const glm::vec2& end) noexcept;
	void SetSpeed(float min, float max) noexcept;
	void SetTimeToLive(float timeToLive) noexcept;
	void SetBeginColor(const glm::vec4& color) noexcept;
	void SetEndColor(const glm::vec4& color) noexcept;

	Node<glm::vec4>& GetColorNode(uint32 index) noexcept;
	const Node<glm::vec4>& GetColorNode(uint32 index) const noexcept;
	uint32 GetNumParticles() const noexcept;

	void AddColorNode(const glm::vec4& color, float atLifeTime) noexcept;

	virtual void OnAddedToScene(Scene* scene) noexcept override;

private:
	void SpawnParticle() noexcept;
	const ParticleInstance* GetParticleInstances() const noexcept;
	ParticleData& GetLivingParticle(uint32 index) noexcept;
	void KillParticle(uint32 index) noexcept;
	const MeshParticle* GetMesh() const noexcept;

private:
	const MeshParticle* m_pMesh;
	float m_ParticleBacklog;
	uint32 m_ParticlesPerSecond;
	glm::vec3 m_Direction;
	uint32 m_LivingParticles[MAX_PARTICLES];
	ParticleData m_Particles[MAX_PARTICLES];
	ParticleInstance m_ParticleInstances[MAX_PARTICLES];
	std::vector<Node<glm::vec4>> m_ColorNodes;
	glm::vec2 m_BeginScale;
	glm::vec2 m_EndScale;
	float m_TimeToLive;
	float m_MinSpeed;
	float m_MaxSpeed;
	float m_ConeAngle;
	uint32 m_NumParticles;
	Scene* m_pScene;
	float m_AutoDeleteTimer;
	IMeshListener* m_pListener;
};
