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

	Mesh(const Vertex* const vertices, unsigned int numVertices) noexcept;
	~Mesh();

	unsigned int GetVertexCount() const noexcept;

private:
	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_VertexCount;

public:
	static Mesh* CreateMeshFromFile(const char* pFilename);
	static Mesh* CreateCube();
	static Mesh* CreateGrid(int width, int height, int depth);
};

inline unsigned int Mesh::GetVertexCount() const noexcept
{
	return m_VertexCount;
}