#pragma once
#include <EnginePch.h>
#include <GLM\glm.hpp>

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 tangent;
	glm::vec2 texCoords;

	inline bool operator==(const Vertex& rs) const
	{
		return (position == rs.position) && (normal == rs.normal) && (tangent == rs.tangent) && (texCoords == rs.texCoords);
	}
};

class API IndexedMesh
{
	friend class GLContext;

public:
	IndexedMesh(IndexedMesh&& other) = delete;
	IndexedMesh(const IndexedMesh& other) = delete;
	IndexedMesh& operator=(IndexedMesh&& other) = delete;
	IndexedMesh& operator=(const IndexedMesh& other) = delete;

	IndexedMesh(const Vertex* const vertices, const uint32* const indices, uint32 numVertices, uint32 numIndices) noexcept;
	~IndexedMesh();

	uint32 GetIndexCount() const noexcept;
	uint32 GetVertexCount() const noexcept;

private:
	uint32 m_VAO;
	uint32 m_VBO;
	uint32 m_IBO;
	uint32 m_VertexCount;
	uint32 m_IndexCount;

public:
	static IndexedMesh* CreateIndexedMeshFromFile(const char* pFilename);
	static IndexedMesh* CreateCube();
};

inline uint32 IndexedMesh::GetIndexCount() const noexcept
{
	return m_IndexCount;
}

inline uint32 IndexedMesh::GetVertexCount() const noexcept
{
	return m_VertexCount;
}