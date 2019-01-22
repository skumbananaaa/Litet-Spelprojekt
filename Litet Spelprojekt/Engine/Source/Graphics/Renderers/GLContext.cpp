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

	case Cap::CLIP_DISTANCE0:
		glEnable(GL_CLIP_DISTANCE0);
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

	case Cap::CLIP_DISTANCE0:
		glDisable(GL_CLIP_DISTANCE0);
		break;
	}
}

void GLContext::SetProgram(const ShaderProgram* pProgram) const noexcept
{
	if (pProgram == nullptr)
	{
		glUseProgram(0);
	}
	else
	{
		glUseProgram(pProgram->m_Program);
	}
}

void GLContext::SetTexture(const Texture* pTexture, uint32 slot) const noexcept
{
	GLenum textureSlot = GL_TEXTURE0 + slot;
	glActiveTexture(textureSlot);

	if (pTexture == nullptr)
	{
		glBindTexture(textureSlot, 0);
	}
	else
	{
		glBindTexture(textureSlot, pTexture->m_Texture);
	}
}

void GLContext::SetUniformBuffer(const UniformBuffer* pBuffer, uint32 slot) const noexcept
{
	if (pBuffer == nullptr)
	{
		glBindBufferBase(GL_UNIFORM_BUFFER, slot, 0);
	}
	else
	{
		glBindBufferBase(GL_UNIFORM_BUFFER, slot, pBuffer->m_Buffer);
	}
}

void GLContext::SetFramebuffer(const Framebuffer* pFramebuffer) const noexcept
{
	if (pFramebuffer == nullptr)
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	else
	{
		glBindFramebuffer(GL_FRAMEBUFFER, pFramebuffer->m_Framebuffer);
	}
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

void GLContext::DrawMesh(const Mesh& mesh, PrimitiveTopology primitiveTopology) const noexcept
{
	glBindVertexArray(mesh.m_VAO);
	glDrawArrays(primitiveTopology, 0, mesh.GetVertexCount());
	glBindVertexArray(0);
}