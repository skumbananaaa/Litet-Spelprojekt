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
	IndexedMesh(const Vertex* const vertices, const unsigned int* const indices, unsigned int numVertices, unsigned int numIndices) noexcept;
	~IndexedMesh();

	unsigned int GetIndexCount() const noexcept;
	unsigned int GetVertexCount() const noexcept;

private:
	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_IBO;
	unsigned int m_VertexCount;
	unsigned int m_IndexCount;

public:
	static IndexedMesh* CreateIndexedMeshFromFile(const char* pFilename);
	static IndexedMesh* CreateCube();
};

inline unsigned int IndexedMesh::GetIndexCount() const noexcept
{
	return m_IndexCount;
}

inline unsigned int IndexedMesh::GetVertexCount() const noexcept
{
	return m_VertexCount;
}