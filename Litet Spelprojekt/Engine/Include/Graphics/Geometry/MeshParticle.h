#pragma once
#include <EnginePch.h>
#include <IO/IResource.h>
#include <Graphics/Particles/ParticleData.h>

struct MeshParticleVertex
{
	glm::vec3 Position;
	glm::vec2 TexCoords;

public:
	inline bool operator==(const MeshParticleVertex& other) const
	{
		return (Position == other.Position) && (TexCoords == other.TexCoords);
	};
};

class API MeshParticle : public IResource
{
	friend class GLContext;
	friend class MESH;
	friend class ResourceHandler;

public:
	MeshParticle(MeshParticle&& other) = delete;
	MeshParticle(const MeshParticle& other) = delete;
	MeshParticle& operator=(MeshParticle&& other) = delete;
	MeshParticle& operator=(const MeshParticle& other) = delete;

	void SetInstances(const ParticleInstance* const pInstances, uint32 numInstances) const noexcept;

	uint32 GetIndexCount() const noexcept;
	uint32 GetVertexCount() const noexcept;
	uint32 GetInstanceCount() const noexcept;

private:
	MeshParticle(const MeshParticleVertex* const pVertices, const uint32* const pIndices, uint32 numVertices, uint32 numIndices) noexcept;
	~MeshParticle();

	virtual void Construct() override;

private:
	uint32 m_VAO;
	uint32 m_VBO;
	uint32 m_IBO;
	uint32 m_InstanceBuffer;
	uint32 m_VertexCount;
	uint32 m_IndexCount;
	const MeshParticleVertex* m_pVertices;
	const uint32* m_pIndices;
	mutable uint32 m_NumInstances;
	mutable uint32 m_NumReservedInstances;

private:
	static MeshParticle* CreateMeshParticleFromFile(const char* pFilename);
	static MeshParticle* CreateCube();
};

inline uint32 MeshParticle::GetIndexCount() const noexcept
{
	return m_IndexCount;
}

inline uint32 MeshParticle::GetVertexCount() const noexcept
{
	return m_VertexCount;
}

inline uint32 MeshParticle::GetInstanceCount() const noexcept
{
	return m_NumInstances;
}