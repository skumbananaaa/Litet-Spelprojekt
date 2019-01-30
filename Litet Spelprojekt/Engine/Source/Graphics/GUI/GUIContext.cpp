#include <EnginePch.h>
#include <Graphics/GUI/GUIContext.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

GUIContext::GUIContext(GLContext* context, ShaderProgram* shaderProgram, FontRenderer* fontRenderer) :
	m_pContext(context),
	m_pShaderProgram(shaderProgram),
	m_pFontRenderer(fontRenderer)
{
	m_pUniformBuffer = new UniformBuffer(&m_UniformData, 1, sizeof(GUIUniformData));

	InitTextureCoords();
	CreateVertexBuffer();
}

GUIContext::~GUIContext()
{
	delete m_pUniformBuffer;
}

void GUIContext::BeginSelfRendering(Framebuffer* frameBuffer)
{
	SetTransform(frameBuffer->GetWidth(), frameBuffer->GetHeight());

	m_pContext->SetFramebuffer(frameBuffer);
	m_pContext->SetClearColor(0.0, 0.0, 0.0, 0.0);
	m_pContext->Clear(CLEAR_FLAG_COLOR);
	m_pContext->SetViewport(frameBuffer->GetWidth(), frameBuffer->GetHeight(), 0, 0);

	glBindVertexArray(m_VAO);
	m_pContext->SetProgram(m_pShaderProgram);
	m_pContext->SetUniformBuffer(m_pUniformBuffer, 0);
}

void GUIContext::BeginRootRendering()
{
	int32 width = Window::GetCurrentWindow().GetWidth();
	int32 height = Window::GetCurrentWindow().GetHeight();
	SetTransform(width, height);

	m_pContext->SetFramebuffer(nullptr);
	m_pContext->SetViewport(width, height, 0, 0);

	glBindVertexArray(m_VAO);
	m_pContext->SetProgram(m_pShaderProgram);
	m_pContext->SetUniformBuffer(m_pUniformBuffer, 0);
}

void GUIContext::SetTransform(float width, float height)
{
	m_UniformData.projection = glm::ortho(0.0f, width, 0.0f, height);
	m_pUniformBuffer->UpdateData(&m_UniformData);
}

void GUIContext::RenderFrameBuffer(Framebuffer* frameBuffer, float x, float y)
{
	SetVertexQuadData(x, y, frameBuffer->GetWidth(), frameBuffer->GetHeight());
	m_pContext->SetTexture(frameBuffer->GetColorAttachment(0), 0);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void GUIContext::RenderText(const std::string& text, int32 x, int32 y, float scale)
{
	m_pFontRenderer->RenderText(m_pContext, text, x, y, scale);
}

glm::vec2& GUIContext::CalculateTextSize(const std::string& text, float scale)
{
	return m_pFontRenderer->CalculateSize(text, scale);
}

GLContext* GUIContext::GetGraphicsContext() const
{
	return m_pContext;
}

FontRenderer* GUIContext::GetFontRenderer() const
{
	return m_pFontRenderer;
}

ShaderProgram* GUIContext::GetShaderProgram() const
{
	return m_pShaderProgram;
}

void GUIContext::SetVertexQuadData(float x, float y, float width, float height)
{
	m_VertexQuad[0].position.x = x;
	m_VertexQuad[0].position.y = y + height;
	m_VertexQuad[1].position.x = x;
	m_VertexQuad[1].position.y = y;
	m_VertexQuad[2].position.x = x + width;
	m_VertexQuad[2].position.y = y;

	m_VertexQuad[3].position.x = x;
	m_VertexQuad[3].position.y = y + height;
	m_VertexQuad[4].position.x = x + width;
	m_VertexQuad[4].position.y = y;
	m_VertexQuad[5].position.x = x + width;
	m_VertexQuad[5].position.y = y + height;

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(VertexGUI) * 6, m_VertexQuad);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void GUIContext::InitTextureCoords()
{
	m_VertexQuad[0].texCoords.x = 0.0;
	m_VertexQuad[0].texCoords.y = 0.0;

	m_VertexQuad[1].texCoords.x = 0.0;
	m_VertexQuad[1].texCoords.y = 1.0;

	m_VertexQuad[2].texCoords.x = 1.0;
	m_VertexQuad[2].texCoords.y = 1.0;

	m_VertexQuad[3].texCoords.x = 0.0;
	m_VertexQuad[3].texCoords.y = 0.0;

	m_VertexQuad[4].texCoords.x = 1.0;
	m_VertexQuad[4].texCoords.y = 1.0;

	m_VertexQuad[5].texCoords.x = 1.0;
	m_VertexQuad[5].texCoords.y = 0.0;
}

void GUIContext::CreateVertexBuffer()
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexGUI) * 6, m_VertexQuad, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(VertexGUI), (void*)0); //Position
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(VertexGUI), (void*)8); //Tex
	glEnableVertexAttribArray(1);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}