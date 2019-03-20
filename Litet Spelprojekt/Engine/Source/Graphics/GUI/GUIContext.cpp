#include <EnginePch.h>
#include <Graphics/GUI/GUIContext.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

const glm::vec4 GUIContext::COLOR_TRANSPARENT(0.0f, 0.0f, 0.0f, 0.0f);
const glm::vec4 GUIContext::COLOR_WHITE(1.0f, 1.0f, 1.0f, 1.0f);
const glm::vec4 GUIContext::COLOR_BLACK(0.0f, 0.0f, 0.0f, 1.0f);
const glm::vec4 GUIContext::COLOR_RED(1.0f, 0.0f, 0.0f, 1.0f);
const glm::vec4 GUIContext::COLOR_GREEN(0.0f, 1.0f, 0.0f, 1.0f);
const glm::vec4 GUIContext::COLOR_BLUE(0.0f, 0.0f, 1.0f, 1.0f);
const glm::vec4 GUIContext::COLOR_YELLOW(1.0f, 1.0f, 0.0f, 1.0f);
const glm::vec4 GUIContext::COLOR_CYAN(0.0f, 1.0f, 1.0f, 1.0f);
const glm::vec4 GUIContext::COLOR_PURPLE(1.0f, 0.0f, 1.0f, 1.0f);

const glm::vec4 GUIContext::COLOR_BUTTON_BACKGROUND(0.231, 0.329, 0.255, 1.0f);
const glm::vec4 GUIContext::COLOR_SELECTED(0.51, 0.91, 0.369, 1.0f);
const glm::vec4 GUIContext::COLOR_HOVER(0.424, 0.6, 0.463, 1.0f);
const glm::vec4 GUIContext::COLOR_PRESSED(0.718, 0.949, 0.635, 1.0f);
const glm::vec4 GUIContext::COLOR_PANEL_BACKGROUND(0.05, 0.08, 0.06, 1.0f);
const glm::vec4 GUIContext::COLOR_PANEL_MIDGROUND(0.231, 0.329, 0.255, 1.0f);
const glm::vec4 GUIContext::COLOR_PANEL_FOREGROUND(0.424, 0.6, 0.463, 1.0f);
const glm::vec4 GUIContext::COLOR_TEXT(1.0f);
const glm::vec4 GUIContext::COLOR_TEXT_HOVER(1.0f);
const glm::vec4 GUIContext::COLOR_TEXT_SELECTED(0.0f, 0.0f, 0.0f, 1.0f);
const glm::vec4 GUIContext::COLOR_TEXT_PRESSED(0.0f, 0.0f, 0.0f, 1.0f);

GUIContext::GUIContext(GLContext* context, const ShaderProgram* shaderProgram, FontRenderer* fontRenderer) :
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
	DeleteSafe(m_pUniformBuffer);
}

void GUIContext::BeginSelfRendering(Framebuffer* frameBuffer, const glm::vec4& clearColor)
{
	SetTransform((float)frameBuffer->GetWidth(), (float)frameBuffer->GetHeight());

	m_pContext->SetFramebuffer(frameBuffer);
	m_pContext->SetClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
	m_pContext->Clear(CLEAR_FLAG_COLOR);
	m_pContext->SetViewport(frameBuffer->GetWidth(), frameBuffer->GetHeight(), 0, 0);
}

void GUIContext::BeginRootRendering()
{
	int32 width = Window::GetCurrentWindow().GetWidth();
	int32 height = Window::GetCurrentWindow().GetHeight();
	SetTransform((float)width, (float)height);

	m_pContext->SetFramebuffer(nullptr);
	m_pContext->SetViewport(width, height, 0, 0);

	m_pContext->ResetClearColor();
}

void GUIContext::SetTransform(float width, float height)
{
	m_UniformData.projection = glm::ortho(0.0f, width, 0.0f, height);
	m_pUniformBuffer->UpdateData(&m_UniformData);
}

void GUIContext::RenderFrameBuffer(Framebuffer* frameBuffer, float x, float y)
{
	m_pContext->SetProgram(m_pShaderProgram);
	m_pContext->SetUniformBuffer(m_pUniformBuffer, 0);
	SetVertexQuadData(x, y, (float)frameBuffer->GetWidth(), (float)frameBuffer->GetHeight(), COLOR_WHITE);
	m_pContext->SetTexture(frameBuffer->GetColorAttachment(0), 0);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	m_pContext->SetTexture(nullptr, 0);
}

void GUIContext::RenderText(const std::string& text, float x, float y, float width, float height, float scale, const glm::vec4& color, TextAlignment textAlignment)
{
	m_pFontRenderer->UpdateBuffer((int32)width, (int32)height, color);

	glm::vec2 size = m_pFontRenderer->CalculateSize(text, scale);

	switch (textAlignment)
	{
	case CENTER:
		x -= size.x / 2;
		y -= size.y / 2;
		break;
	case CENTER_VERTICAL:
		y -= size.y / 2;
		break;
	case CENTER_HORIZONTAL:
		x -= size.x / 2;
		break;
	default:
		break;
	}

	m_pFontRenderer->RenderText(m_pContext, text, (int32)x, (int32)y, scale);
}

void GUIContext::RenderTexture(const Texture2D* texture, float x, float y, float width, float height, const glm::vec4& color)
{
	m_pContext->SetProgram(m_pShaderProgram);
	m_pContext->SetUniformBuffer(m_pUniformBuffer, 0);
	SetVertexQuadData(x, y, width, height, color);
	m_pContext->SetTexture(texture, 0);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	m_pContext->SetTexture(nullptr, 0);
}

glm::vec2 GUIContext::CalculateTextSize(const std::string& text, float scale)
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

const ShaderProgram* GUIContext::GetShaderProgram() const
{
	return m_pShaderProgram;
}

void GUIContext::SetVertexQuadData(float x, float y, float width, float height, const glm::vec4& color)
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

	m_VertexQuad[0].color = color;
	m_VertexQuad[1].color = color;
	m_VertexQuad[2].color = color;
	m_VertexQuad[3].color = color;
	m_VertexQuad[4].color = color;
	m_VertexQuad[5].color = color;

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(VertexGUI) * 6, m_VertexQuad);
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

	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(VertexGUI), (void*)16); //Color
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}