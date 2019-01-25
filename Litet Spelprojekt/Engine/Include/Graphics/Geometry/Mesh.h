#pragma once
#include "IndexedMesh.h"

class API Mesh
{
	friend class GLContext;

public:
	Mesh(Mesh&& other) = delete;
	Mesh(const Mesh& other) = delete;
	Mesh& operator=(Mesh&& other) = delete;
	Mesh& operator=(const Mesh& other) = delete;

	Mesh(const Vertex* const vertices, uint32 numVertices) noexcept;
	~Mesh();

	uint32 GetVertexCount() const noexcept;

private:
	uint32 m_VAO;
	uint32 m_VBO;
	uint32 m_VertexCount;

public:
	static Mesh* CreateMeshFromFile(const char* pFilename);
	static Mesh* CreateCube();
	static Mesh* CreateGrid(int32 width, int32 height, int32 depth);
};

inline unsigned int Mesh::GetVertexCount() const noexcept
{
	return m_VertexCount;
}