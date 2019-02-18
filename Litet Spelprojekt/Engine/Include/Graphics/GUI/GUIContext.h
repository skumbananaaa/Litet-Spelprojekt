#pragma once
#include <EnginePch.h>
#include <Graphics/Renderers/GLContext.h>
#include <Graphics/Buffers/UniformBuffer.h>
#include <Graphics/Renderers/FontRenderer.h>
#include <System/Window.h>

struct VertexGUI
{
	glm::vec2 position;
	glm::vec2 texCoords;
	glm::vec4 color;

	inline bool operator==(const VertexGUI& rs) const
	{
		return (position == rs.position) && (texCoords == rs.texCoords) && (color == rs.color);
	}
};

struct GUIUniformData
{
	glm::mat4 projection;
};

enum TextAlignment
{
	CENTER = 0,
	CENTER_VERTICAL,
	CENTER_HORIZONTAL,
};

class API GUIContext
{
public:
	GUIContext(GLContext* context, const ShaderProgram* shaderProgram, FontRenderer* fontRenderer);
	~GUIContext();

	void BeginSelfRendering(Framebuffer* frameBuffer, const glm::vec4& clearColor);
	void BeginRootRendering();

	void RenderFrameBuffer(Framebuffer* frameBuffer, float x, float y);
	void RenderText(const std::string& text, float x, float y, float width, float height, float scale, const glm::vec4& color, TextAlignment textAlignment = CENTER_VERTICAL);
	void RenderTexture(const Texture2D* texture, float x, float y, float width, float height, const glm::vec4& color);
	glm::vec2 CalculateTextSize(const std::string& text, float scale);
	void SetVertexQuadData(float x, float y, float width, float height, const glm::vec4& color);


	GLContext* GetGraphicsContext() const;
	FontRenderer* GetFontRenderer() const;
	const ShaderProgram* GetShaderProgram() const;

	static const glm::vec4 COLOR_TRANSPARENT;
	static const glm::vec4 COLOR_WHITE;
	static const glm::vec4 COLOR_BLACK;
	static const glm::vec4 COLOR_RED;
	static const glm::vec4 COLOR_GREEN;
	static const glm::vec4 COLOR_BLUE;
	static const glm::vec4 COLOR_YELLOW;
	static const glm::vec4 COLOR_CYAN;
	static const glm::vec4 COLOR_PURPLE;

private:
	GUIUniformData m_UniformData;
	UniformBuffer* m_pUniformBuffer;
	const ShaderProgram* m_pShaderProgram;
	FontRenderer* m_pFontRenderer;
	GLContext* m_pContext;
	VertexGUI m_VertexQuad[6];
	unsigned int m_VAO;
	unsigned int m_VBO;

	void InitTextureCoords();
	void CreateVertexBuffer();
	void SetTransform(float width, float height);
};