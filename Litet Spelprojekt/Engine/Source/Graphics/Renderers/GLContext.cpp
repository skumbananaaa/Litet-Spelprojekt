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
	}
}

GLContext::~GLContext()
{
}

void GLContext::Enable(Cap cap) const noexcept
{
	glEnable(cap);
}

void GLContext::Disable(Cap cap) const noexcept
{
	glDisable(cap);
}

void GLContext::SetProgram(const ShaderProgram& program) const noexcept
{
	glUseProgram(program.m_Program);
}

void GLContext::SetTexture(const Texture& texture, GLuint slot) const noexcept
{
	GLenum slot = GL_TEXTURE0 + slot;
	glActiveTexture(slot);
	glBindTexture(slot, texture.m_Texture);
}

void GLContext::SetUniformBuffer(const UniformBuffer& buffer, GLuint slot) const noexcept
{
	glBindBufferBase(GL_UNIFORM_BUFFER, slot, buffer.m_Buffer);
}

void GLContext::DrawIndexedMesh(const IndexedMesh& mesh) const noexcept
{
	glBindVertexArray(mesh.m_VAO);
	glDrawElements(GL_TRIANGLES, mesh.GetIndexCount(), GL_UNSIGNED_INT, nullptr);
	glBindVertexArray(0);
}