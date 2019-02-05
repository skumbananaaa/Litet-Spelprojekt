#include <EnginePch.h>
#include <Graphics/Geometry/FullscreenTri.h>
#include <Graphics/Renderers/GLContext.h>

FullscreenTri::FullscreenTri() noexcept
	: m_VAO(0)
{
	Create();
}

FullscreenTri::~FullscreenTri()
{
	if (glIsVertexArray(m_VAO))
	{
		GL_CALL(glDeleteVertexArrays(1, &m_VAO));
		m_VAO = 0;
	}
}

void FullscreenTri::Create() noexcept
{
	GL_CALL(glGenVertexArrays(1, &m_VAO));
	GL_CALL(glBindVertexArray(m_VAO));
	GL_CALL(glBindVertexArray(0));
}
