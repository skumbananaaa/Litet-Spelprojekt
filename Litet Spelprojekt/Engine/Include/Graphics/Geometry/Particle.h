#pragma once
#include <EnginePch.h>

struct ParticleInstance
{
	glm::vec3 Position;
	glm::vec4 Color;
	glm::vec2 Scale;
};

class API Particle
{
	friend class GLContext;

public:
	Particle(Particle&& other) = delete;
	Particle(const Particle& other) = delete;
	Particle& operator=(Particle&& other) = delete;
	Particle& operator=(const Particle& other) = delete;

	Particle();
	~Particle();

	void SetInstances(const ParticleInstance* pInstances, uint32 numInstances);

private:
	void Create();
	uint32 GetInstanceCount() const noexcept;

private:
	uint32 m_VAO;
	uint32 m_VBO;
	uint32 m_InstanceBuffer;
	uint32 m_NumInstances;
};

inline uint32 Particle::GetInstanceCount() const noexcept
{
	return m_NumInstances;
}