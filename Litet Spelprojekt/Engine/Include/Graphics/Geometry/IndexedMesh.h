#pragma once

#include <EnginePch.h>
#include <Defines.h>

class API IndexedMesh
{
public:
	IndexedMesh();
	~IndexedMesh();

	void Bind();
	void Unbind();

	int32_t GetIndexCount(); //signed int?
	int32_t GetVertexCount();

private:
	GLuint m_VAO;
	GLuint m_VBO;
	GLuint m_IBO;
};