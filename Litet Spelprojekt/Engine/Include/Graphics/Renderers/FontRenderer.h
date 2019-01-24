#pragma once

#include <EnginePch.h>
#include <Graphics\Shaders\ShaderProgram.h>
#include <Graphics/Renderers/GLContext.h>

#include <map>

struct Character
{
	uint32 TextureID;   // ID handle of the glyph texture
	glm::ivec2 Size;    // Size of glyph
	glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
	uint32 Advance;    // Horizontal offset to advance to next glyph
};

struct PerFrameFontRenderer
{
	glm::mat4 projection;
	glm::vec3 color;
};

class API FontRenderer
{
private:
	FontRenderer(const GLContext& context, void* face);

	static ShaderProgram* m_pShaderProgram;
	static unsigned int m_VAO;
	static unsigned int m_VBO;
	static std::vector<FontRenderer*> m_Fontrenderers;

	PerFrameFontRenderer m_PerFrameData;
	std::map<char, Character> m_Characters;
	UniformBuffer* m_pPerFrameUniform;

public:
	~FontRenderer();

	void RenderText(GLContext& context, std::string text, float x, float y, float scale);
	void UpdateRenderTargetSize(int width, int height);
	void SetColor(const glm::vec3& color);

	static FontRenderer* CreateFontRenderer(const GLContext& context, const char* font, int width, int height);
};
