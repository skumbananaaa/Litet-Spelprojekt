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

	inline bool operator==(const VertexGUI& rs) const
	{
		return (position == rs.position) && (texCoords == rs.texCoords);
	}
};

struct GUIUniformData
{
	glm::mat4 projection;
	glm::vec3 color;
};

class API GUIContext
{
public:
	GUIContext(GLContext* context, ShaderProgram* shaderProgram, FontRenderer* fontRenderer);
	~GUIContext();

	void BeginSelfRendering(Framebuffer* frameBuffer);
	void BeginRootRendering();

	void RenderFrameBuffer(Framebuffer* frameBuffer, float x, float y);
	void RenderText(const std::string& text, int32 x, int32 y, float scale);
	glm::vec2& CalculateTextSize(const std::string& text, float scale);
	void SetVertexQuadData(float x, float y, float width, float height);

	GLContext* GetGraphicsContext() const;
	FontRenderer* GetFontRenderer() const;
	ShaderProgram* GetShaderProgram() const;

private:
	GUIUniformData m_UniformData;
	UniformBuffer* m_pUniformBuffer;
	ShaderProgram* m_pShaderProgram;
	FontRenderer* m_pFontRenderer;
	GLContext* m_pContext;
	VertexGUI m_VertexQuad[6];
	unsigned int m_VAO;
	unsigned int m_VBO;

	void InitTextureCoords();
	void CreateVertexBuffer();
	void SetTransform(float width, float height);
};