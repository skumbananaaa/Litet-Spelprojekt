#include <EnginePch.h>
#include <Graphics/Renderers/GLContext.h>

GLContext::GLContext(float width, float height)
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

		SetViewport(width, height, 0, 0);
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
	glActiveTexture(GL_TEXTURE0 + slot);

	if (pTexture == nullptr)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D, pTexture->m_Texture);
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

void GLContext::SetViewport(uint32 width, uint32 height, uint32 topX, uint32 topY) noexcept
{
	glViewport(topX, topY, width, height);
	glScissor(topX, topY, width, height);
	m_ViewPort.x = width;
	m_ViewPort.y = height;
	m_ViewPort.z = topX;
	m_ViewPort.w = topY;
}

void GLContext::SetViewport(const glm::vec4& viewport) noexcept
{
	SetViewport(viewport.x, viewport.y, viewport.z, viewport.w);
}

const glm::vec4 GLContext::GetViewPort() const noexcept
{
	return m_ViewPort;
}

void GLContext::SetClearColor(float r, float g, float b, float a) const noexcept
{
	glClearColor(r, g, b, a);
}

void GLContext::SetClearDepth(float depth) const noexcept
{
	glClearDepthf(depth);
}

void GLContext::SetColorMask(uint8 r, uint8 g, uint8 b, uint8 a) const noexcept
{
	glColorMask(r, g, b, a);
}

void GLContext::SetDepthMask(bool writeDepth) const noexcept
{
	glDepthMask((writeDepth) ? GL_TRUE : GL_FALSE);
}

void GLContext::SetDepthFunc(Func func) const noexcept
{
	glDepthFunc(func);
}

void GLContext::Clear(uint32 flags) const noexcept
{
	glClear(flags);
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

void GLContext::DrawFullscreenTriangle(const FullscreenTri & triangle) const noexcept
{
	glBindVertexArray(triangle.m_VAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}
