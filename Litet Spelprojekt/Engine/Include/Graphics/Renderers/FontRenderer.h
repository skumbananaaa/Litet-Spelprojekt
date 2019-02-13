#pragma once

#include <EnginePch.h>
#include <IO/ResourceHandler.h>
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
	FontRenderer(void* face);

	static unsigned int m_VAO;
	static unsigned int m_VBO;
	static std::vector<FontRenderer*> m_Fontrenderers;

	PerFrameFontRenderer m_PerFrameData;
	std::map<char, Character> m_Characters;
	UniformBuffer* m_pPerFrameUniform;

public:
	~FontRenderer();

	void RenderText(GLContext* context, std::string text, int32 x, int32 y, float scale);
	void UpdateBuffer(int width, int height, const glm::vec4& color);
	glm::vec2 CalculateSize(std::string text, float scale);

	static FontRenderer* CreateFontRenderer(const char* font, int width, int height);
};
