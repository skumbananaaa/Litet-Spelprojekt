#include <EnginePch.h>
#include <Graphics/Geometry/FullscreenTri.h>

FullscreenTri::FullscreenTri() noexcept
	: m_VAO(0)
{
	Create();
}

FullscreenTri::~FullscreenTri()
{
	if (glIsVertexArray(m_VAO))
	{
		glDeleteVertexArrays(1, &m_VAO);
		m_VAO = 0;
	}
}

void FullscreenTri::Create() noexcept
{
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);
	glBindVertexArray(0);
}
