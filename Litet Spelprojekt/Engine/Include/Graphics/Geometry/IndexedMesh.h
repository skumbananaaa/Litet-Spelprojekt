#pragma once

#include <EnginePch.h>
#include <Defines.h>

class API IndexedMesh
{
	friend class GLContext;

public:
	IndexedMesh();
	~IndexedMesh();

	unsigned int GetIndexCount() const noexcept;
	unsigned int GetVertexCount() const noexcept;

private:
	GLuint m_VAO;
	GLuint m_VBO;
	GLuint m_IBO;
	int m_VertexCount;
	int m_IndexCount;
};

inline unsigned int IndexedMesh::GetIndexCount() const noexcept
{
	return m_IndexCount;
}

inline unsigned int IndexedMesh::GetVertexCount() const noexcept
{
	return m_VertexCount;
}