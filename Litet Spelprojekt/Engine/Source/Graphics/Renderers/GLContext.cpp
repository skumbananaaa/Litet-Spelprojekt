#include <EnginePch.h>
#include <Graphics/Renderers/GLContext.h>

GLContext::GLContext()
{
	if (!gladLoadGL())
	{
		std::cout << "Failed to initialize OpenGL" << std::endl;
	}
	else
	{
		std::cout << "OpenGL initialized" << std::endl;
		std::cout << glGetString(GL_VENDOR) << std::endl;
		std::cout << glGetString(GL_RENDERER) << std::endl;
	}
}

GLContext::~GLContext()
{
}

void GLContext::Enable(Cap cap) const noexcept
{

	switch (cap)
	{
	case Cap::DEPTH_TEST:
		glEnable(GL_DEPTH_TEST);
		break;
		
	case Cap::CULL_FACE:
		glEnable(GL_CULL_FACE);
		break;

	case Cap::BLEND:
		glEnable(GL_BLEND);
		break;
	}
}

void GLContext::Disable(Cap cap) const noexcept
{
	switch (cap)
	{
	case Cap::DEPTH_TEST:
		glDisable(GL_DEPTH_TEST);
		break;

	case Cap::CULL_FACE:
		glDisable(GL_CULL_FACE);
		break;

	case Cap::BLEND:
		glDisable(GL_BLEND);
		break;
	}
}

void GLContext::SetProgram(const ShaderProgram& program) const noexcept
{
	glUseProgram(program.m_Program);
}

void GLContext::SetTexture(const Texture& texture, uint32 slot) const noexcept
{
	GLenum textureSlot = GL_TEXTURE0 + slot;
	glActiveTexture(textureSlot);
	glBindTexture(textureSlot, texture.m_Texture);
}

void GLContext::SetUniformBuffer(const UniformBuffer& buffer, uint32 slot) const noexcept
{
	glBindBufferBase(GL_UNIFORM_BUFFER, slot, buffer.m_Buffer);
}

void GLContext::SetViewport(uint32 width, uint32 height, uint32 topX, uint32 topY) const noexcept
{
	glViewport(topX, topY, width, height);
	glScissor(topX, topY, width, height);
}

void GLContext::SetClearColor(float r, float g, float b, float a) const noexcept
{
	glClearColor(r, g, b, a);
}

void GLContext::Clear() const noexcept
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GLContext::DrawIndexedMesh(const IndexedMesh& mesh) const noexcept
{
	glBindVertexArray(mesh.m_VAO);
	glDrawElements(GL_TRIANGLES, mesh.GetIndexCount(), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}