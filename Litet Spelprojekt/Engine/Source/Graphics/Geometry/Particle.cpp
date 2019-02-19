#include <EnginePch.h>
#include <Graphics/Geometry/Particle.h>
#include <Graphics/Renderers/GLContext.h>

Particle::Particle()
	: m_VAO(0),
	m_VBO(0),
	m_InstanceBuffer(0),
	m_NumInstances(0)
{
	Create();
}

Particle::~Particle()
{
	if (glIsVertexArray(m_VAO))
	{
		GL_CALL(glDeleteVertexArrays(1, &m_VAO));
		m_VAO = 0;
	}

	if (glIsBuffer(m_VBO))
	{
		GL_CALL(glDeleteBuffers(1, &m_VBO));
		m_VBO = 0;
	}

	if (glIsBuffer(m_InstanceBuffer))
	{
		GL_CALL(glDeleteBuffers(1, &m_InstanceBuffer));
		m_InstanceBuffer = 0;
	}
}

void Particle::SetInstances(const ParticleInstance* pInstances, uint32 numInstances)
{
	GL_CALL(glBindVertexArray(0));
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_InstanceBuffer));

	uint32 size = sizeof(ParticleInstance) * numInstances;
	if (numInstances > m_NumInstances)
	{
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_STATIC_DRAW);
	}

	GL_CALL(glBufferSubData(GL_ARRAY_BUFFER, 0, size, pInstances));
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
	
	m_NumInstances = numInstances;
}

void Particle::Create()
{
	GL_CALL(glGenVertexArrays(1, &m_VAO));
	GL_CALL(glBindVertexArray(m_VAO));

	GL_CALL(glGenBuffers(1, &m_VBO));
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));

	struct ParticleVertex
	{
		glm::vec3 Position;
		glm::vec2 TexCoord;
	};

	ParticleVertex vertices[] =
	{
		{ glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec2(0.0f, 0.0f) },
		{ glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec2(0.0f, 1.0f) },
		{ glm::vec3(0.5f, -0.5f, 0.0f), glm::vec2(1.0f, 0.0f) },
		{ glm::vec3(0.5f, 0.5f, 0.0f), glm::vec2(1.0f, 1.0f) },
	};

	GL_CALL(glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(ParticleVertex), vertices, GL_STATIC_DRAW));
	//Position
	GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(ParticleVertex), (void*)0));
	GL_CALL(glEnableVertexAttribArray(0));
	//TexCoords
	GL_CALL(glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(ParticleVertex), (void*)sizeof(glm::vec3)));
	GL_CALL(glEnableVertexAttribArray(1));

	constexpr uint32 NumInstances = 10;
	m_NumInstances = NumInstances;
	ParticleInstance instances[10];
	for (uint32 i = 0; i < NumInstances; i++)
	{
		instances[i].Position = glm::vec3(0.0f);
		instances[i].Color = glm::vec4(1.0f);
	}

	GL_CALL(glGenBuffers(1, &m_InstanceBuffer));
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_InstanceBuffer));
	GL_CALL(glBufferData(GL_ARRAY_BUFFER, m_NumInstances * sizeof(ParticleInstance), instances, GL_STATIC_DRAW));

	//Per instance position
	GL_CALL(glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(ParticleInstance), (void*)0);
	GL_CALL(glEnableVertexAttribArray(2));
	GL_CALL(glVertexAttribDivisor(2, 1));

	//Per instance color
	GL_CALL(glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(ParticleInstance), (void*)sizeof(glm::vec3))));
	GL_CALL(glEnableVertexAttribArray(3));
	GL_CALL(glVertexAttribDivisor(3, 1));

	GL_CALL(glBindVertexArray(0));
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
