#pragma once


#include <EnginePch.h>
#include <Graphics\Shaders\ShaderProgram.h>
#include <Graphics/Renderers/GLContext.h>

#include <map>

struct Character {
	uint32 TextureID;   // ID handle of the glyph texture
	glm::ivec2 Size;    // Size of glyph
	glm::ivec2 Bearing;  // Offset from baseline to left/top of glyph
	uint32 Advance;    // Horizontal offset to advance to next glyph
};

class API test
{
public:
	std::map<char, Character> Characters;
	uint32 VAO, VBO;
	ShaderProgram* m_pShaderProgramDefault;

	UniformBuffer* m_pPerFrameUniform;

	test(GLContext& context);

	void RenderText(GLContext& context, std::string text, float x, float y, float scale, glm::vec3 color);
};
