#include <EnginePch.h>
#include <Graphics/Renderers/GLContext.h>
#include <System/Window.h>

GLContext* GLContext::s_CurrentContext = nullptr;

GLContext::GLContext(float width, float height) : m_DefaultClearColor(0.392f, 0.584f, 0.929f, 1.0f)
{
	assert(s_CurrentContext == nullptr);
	s_CurrentContext = this;

	if (!gladLoadGL())
	{
		std::cout << "Failed to initialize OpenGL" << std::endl;
	}
	else
	{
		std::cout << "OpenGL initialized" << std::endl;
		std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
		std::cout << "Renderer (Adapter): " << glGetString(GL_RENDERER) << std::endl;
		std::cout << "Version: " << glGetString(GL_VERSION) << std::endl;
		std::cout << "GLSL Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

		GLint param = 0;
		GL_CALL(glGetIntegerv(GL_MAX_TEXTURE_UNITS_ARB, &param));
		std::cout << "Max Texture units: " << param << std::endl;
		GL_CALL(glGetIntegerv(GL_MAX_TEXTURE_SIZE, &param));
		std::cout << "Max Texture size: " << param << " pixels" << std::endl;
		GL_CALL(glGetIntegerv(GL_MAX_UNIFORM_BUFFER_BINDINGS, &param));
		std::cout << "Max Uniformbuffer units: " << param << std::endl;
		GL_CALL(glGetIntegerv(GL_MAX_UNIFORM_BLOCK_SIZE, &param));
		std::cout << "Max Uniformbuffer size: " << param << " bytes"<< std::endl;
		GL_CALL(glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &param));
		std::cout << "Max Vertex Attributes: " << param << std::endl;

		SetViewport(static_cast<uint32>(width), static_cast<uint32>(height), 0, 0);
	}

	for (int i = 0; i < 16; i++)
	{
		m_CurrentTextures[i] = GL_TEXTURE_2D;
	}

	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
}

GLContext::~GLContext()
{
}

void GLContext::Enable(Cap cap) const noexcept
{
	GL_CALL(glEnable(cap));
}

void GLContext::Disable(Cap cap) const noexcept
{
	GL_CALL(glDisable(cap));
}

void GLContext::SetCullMode(CULL_MODE mode) const noexcept
{
	GL_CALL(glCullFace(mode));
}

void GLContext::SetProgram(const ShaderProgram* pProgram) const noexcept
{
	if (pProgram == nullptr)
	{
		GL_CALL(glUseProgram(0));
	}
	else
	{
		GL_CALL(glUseProgram(pProgram->m_Program));
	}
}

void GLContext::SetTexture(const Texture* pTexture, uint32 slot) const noexcept
{
	GL_CALL(glActiveTexture(GL_TEXTURE0 + slot));

	if (pTexture == nullptr)
	{
		GL_CALL(glBindTexture(m_CurrentTextures[slot], 0));
	}
	else
	{
		m_CurrentTextures[slot] = pTexture->GetType();
		GL_CALL(glBindTexture(m_CurrentTextures[slot], pTexture->m_Texture));
	}
}

void GLContext::SetUniformBuffer(const UniformBuffer* pBuffer, uint32 slot) const noexcept
{
	if (pBuffer == nullptr)
	{
		GL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, slot, 0));
	}
	else
	{
		GL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, slot, pBuffer->m_Buffer));
	}
}

void GLContext::SetFramebuffer(const Framebuffer* pFramebuffer) const noexcept
{
	if (pFramebuffer == nullptr)
	{
		GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	}
	else
	{
		GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, pFramebuffer->m_Framebuffer));
	}
}

void GLContext::BlitFramebuffer(const Framebuffer* const pDst, const Framebuffer* const pSrc, uint32 flags)
{
	uint32 srcWidth = 0;
	uint32 srcHeight = 0;
	uint32 srcFBO = 0;
	uint32 dstWidth = 0;
	uint32 dstHeight = 0;
	uint32 dstFBO = 0;

	if (pDst == nullptr)
	{
		dstWidth = Window::GetCurrentWindow().GetWidth();
		dstHeight = Window::GetCurrentWindow().GetHeight();
		srcWidth = pSrc->GetWidth();
		srcHeight = pSrc->GetHeight();
		srcFBO = pSrc->m_Framebuffer;
		dstFBO = 0;
	}
	else if (pSrc == nullptr)
	{
		dstWidth = pDst->GetWidth();
		dstHeight = pSrc->GetHeight();
		srcWidth = Window::GetCurrentWindow().GetWidth();
		srcHeight = Window::GetCurrentWindow().GetHeight();
		srcFBO = 0;
		dstFBO = pDst->m_Framebuffer;
	}
	else
	{
		srcFBO = pSrc->m_Framebuffer;
		dstFBO = pDst->m_Framebuffer;
	}

	GL_CALL(glBindFramebuffer(GL_READ_FRAMEBUFFER, srcFBO));
	GL_CALL(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, dstFBO));
	GL_CALL(glBlitFramebuffer(0, 0, srcWidth, srcHeight, 0, 0, dstWidth, dstHeight, flags, GL_NEAREST));
	GL_CALL(glBindFramebuffer(GL_READ_FRAMEBUFFER, 0));
	GL_CALL(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0));
}

void GLContext::SetViewport(uint32 width, uint32 height, uint32 topX, uint32 topY) noexcept
{
	GL_CALL(glViewport(topX, topY, width, height));
	GL_CALL(glScissor(topX, topY, width, height));
	m_ViewPort.x = static_cast<float>(width);
	m_ViewPort.y = static_cast<float>(height);
	m_ViewPort.z = static_cast<float>(topX);
	m_ViewPort.w = static_cast<float>(topY);
}

void GLContext::SetViewport(const glm::vec4& viewport) noexcept
{
	SetViewport(viewport.x, viewport.y, viewport.z, viewport.w);
}

const glm::vec4 GLContext::GetViewPort() const noexcept
{
	return m_ViewPort;
}

void GLContext::ResetClearColor() const noexcept
{
	GL_CALL(glClearColor(m_DefaultClearColor.r, m_DefaultClearColor.g, m_DefaultClearColor.b, m_DefaultClearColor.a));
}

void GLContext::SetClearColor(float r, float g, float b, float a) const noexcept
{
	GL_CALL(glClearColor(r, g, b, a));
}

void GLContext::SetClearDepth(float depth) const noexcept
{
	GL_CALL(glClearDepthf(depth));
}

void GLContext::SetColorMask(uint8 r, uint8 g, uint8 b, uint8 a) const noexcept
{
	GL_CALL(glColorMask(r, g, b, a));
}

void GLContext::SetDepthMask(bool writeDepth) const noexcept
{
	GL_CALL(glDepthMask((writeDepth) ? GL_TRUE : GL_FALSE));
}

void GLContext::SetStencilMask(uint8 mask) const noexcept
{
	GL_CALL(glStencilMask(mask));
}

void GLContext::SetDepthFunc(Func func) const noexcept
{
	GL_CALL(glDepthFunc(func));
}

void GLContext::SetStencilFunc(Func func, uint8 ref, uint8 mask) const noexcept
{
	GL_CALL(glStencilFunc(func, ref, mask));
}

void GLContext::SetStencilOp(StencilOp sFail, StencilOp dpFail, StencilOp dpPass) const noexcept
{
	GL_CALL(glStencilOp(sFail, dpFail, dpPass));
}

void GLContext::Clear(uint32 flags) const noexcept
{
	GL_CALL(glClear(flags));
}

void GLContext::DrawIndexedMesh(const IndexedMesh& mesh) const noexcept
{
	GL_CALL(glBindVertexArray(mesh.m_VAO));
	GL_CALL(glDrawElements(GL_TRIANGLES, mesh.GetIndexCount(), GL_UNSIGNED_INT, nullptr));
	GL_CALL(glBindVertexArray(0));
}

void GLContext::DrawIndexedMeshInstanced(const IndexedMesh& mesh) const noexcept
{
	GL_CALL(glBindVertexArray(mesh.m_VAO));
	GL_CALL(glDrawElementsInstanced(GL_TRIANGLES, mesh.GetIndexCount(), GL_UNSIGNED_INT, nullptr, mesh.GetInstanceCount()));
	GL_CALL(glBindVertexArray(0));
}

void GLContext::DrawMesh(const Mesh& mesh, PrimitiveTopology primitiveTopology) const noexcept
{
	GL_CALL(glBindVertexArray(mesh.m_VAO));
	GL_CALL(glDrawArrays(primitiveTopology, 0, mesh.GetVertexCount()));
	GL_CALL(glBindVertexArray(0));
}

void GLContext::DrawFullscreenTriangle(const FullscreenTri & triangle) const noexcept
{
	GL_CALL(glBindVertexArray(triangle.m_VAO));
	GL_CALL(glDrawArrays(GL_TRIANGLES, 0, 3));
	GL_CALL(glBindVertexArray(0));
}

bool GLContext::HasErrors() const noexcept
{
	int32 lastError = GL_NO_ERROR;
	int32 error = GL_NO_ERROR;

	while ((error = glGetError()) != GL_NO_ERROR)
	{
		std::cout << "OpenGL Error - Code(" << error << ") - ";

		switch (error)
		{
		case GL_NO_ERROR:
			std::cout << "GL_NO_ERROR" << std::endl;
			break;
		case GL_INVALID_ENUM:
			std::cout << "GL_INVALID_ENUM" << std::endl;
			break;
		case GL_INVALID_VALUE:
			std::cout << "GL_INVALID_VALUE" << std::endl;
			break;
		case GL_INVALID_OPERATION:
			std::cout << "GL_INVALID_OPERATION" << std::endl;
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			std::cout << "GL_INVALID_FRAMEBUFFER_OPERATION" << std::endl;
			break;
		case GL_OUT_OF_MEMORY:
			std::cout << "GL_OUT_OF_MEMORY" << std::endl;
			break;
		case GL_STACK_UNDERFLOW:
			std::cout << "GL_STACK_UNDERFLOW" << std::endl;
			break;
		case GL_STACK_OVERFLOW:
			std::cout << "GL_STACK_OVERFLOW" << std::endl;
			break;
		}

		lastError = error;
	}

	return (lastError != GL_NO_ERROR);
}

void GLContext::ClearErrors() const noexcept
{
	while (glGetError() != GL_NO_ERROR);
}

GLContext& GLContext::GetCurrentContext() noexcept
{
	assert(s_CurrentContext != nullptr);
	return *s_CurrentContext;
}