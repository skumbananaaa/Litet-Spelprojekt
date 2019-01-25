#include <Graphics/Renderers/FontRenderer.h>

#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include <ft2build.h>
#include FT_FREETYPE_H

#define VERTEX_COUNT 6

ShaderProgram* FontRenderer::m_pShaderProgram = nullptr;
unsigned int FontRenderer::m_VAO;
unsigned int FontRenderer::m_VBO;
std::vector<FontRenderer*> FontRenderer::m_Fontrenderers;

FontRenderer::FontRenderer(void* face)
{
	m_pPerFrameUniform = new UniformBuffer(&m_PerFrameData, 1, sizeof(PerFrameFontRenderer));

	
	// Disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	FT_Face ft_face = *(FT_Face*)face;

	// Load first 128 characters of ASCII set
	for (unsigned char c = 0; c < 128; c++)
	{
		// Load character glyph 
		if (FT_Load_Char(ft_face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// Generate texture
		uint32 texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			ft_face->glyph->bitmap.width,
			ft_face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			ft_face->glyph->bitmap.buffer
		);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Now store character for later use
		Character character =
		{
			texture,
			glm::ivec2(ft_face->glyph->bitmap.width, ft_face->glyph->bitmap.rows),
			glm::ivec2(ft_face->glyph->bitmap_left, ft_face->glyph->bitmap_top),
			ft_face->glyph->advance.x
		};
		m_Characters.insert(std::pair<char, Character>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, VERTEX_COUNT * sizeof(float) * 4, nullptr, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0); //Position
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	m_Fontrenderers.push_back(this);
}

FontRenderer::~FontRenderer()
{
	delete m_pPerFrameUniform;

	int counter = 0;
	for (FontRenderer* renderer : m_Fontrenderers)
	{
		if (renderer == this)
		{
			m_Fontrenderers.erase(m_Fontrenderers.begin() + counter);
			break;
		}
		counter++;
	}

	if (m_Fontrenderers.size() == 0)
	{
		delete m_pShaderProgram;
	}
}

void FontRenderer::RenderText(GLContext* context, std::string text, float x, float y, float scale)
{	
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	context->SetProgram(m_pShaderProgram);

	context->SetUniformBuffer(m_pPerFrameUniform, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(m_VAO);

	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character character = m_Characters[*c];

		float xpos = x + character.Bearing.x * scale;
		float ypos = y - (character.Size.y - character.Bearing.y) * scale;

		float w = character.Size.x * scale;
		float h = character.Size.y * scale;
		// Update VBO for each character
		float vertices[VERTEX_COUNT][4] =
		{
			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos,     ypos,       0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 1.0 },

			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos + w, ypos,       1.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 0.0 }
		};
		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, character.TextureID);
		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // Be sure to use glBufferSubData and not glBufferData

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, VERTEX_COUNT);
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (character.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void FontRenderer::UpdateRenderTargetSize(int width, int height)
{
	m_PerFrameData.projection = glm::ortho(0.0f, static_cast<float>(width), 0.0f, static_cast<float>(height));
	m_pPerFrameUniform->UpdateData(&m_PerFrameData);
}

void FontRenderer::SetColor(const glm::vec3& color)
{
	m_PerFrameData.color = color;
	m_pPerFrameUniform->UpdateData(&m_PerFrameData);
}

glm::vec2 FontRenderer::CalculateSize(std::string text, float scale)
{
	std::string::const_iterator c;
	glm::vec2 size(0, 0);
	for (c = text.begin(); c != text.end(); c++)
	{
		Character character = m_Characters[*c];
		float h = character.Bearing.y * scale;
		if (h > size.y)
		{
			size.y = h;
		}
	
		size.x += (character.Advance >> 6) * scale;
	}
	return size;
}

FontRenderer* FontRenderer::CreateFontRenderer(const char* font, int width, int height)
{
	FT_Library freeType;

	if (FT_Init_FreeType(&freeType))
	{
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
		return nullptr;
	}

	FT_Face face;
	if (FT_New_Face(freeType, font, 0, &face))
	{
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
		return nullptr;
	}

	FT_Set_Pixel_Sizes(face, 0, 48);

	if (m_pShaderProgram == nullptr)
	{
		Shader vShader;
		Shader fShader;

		vShader.CompileFromFile("Resources/Shaders/VShaderFont.glsl", VERTEX_SHADER);
		fShader.CompileFromFile("Resources/Shaders/FShaderFont.glsl", FRAGMENT_SHADER);

		m_pShaderProgram = new ShaderProgram(vShader, fShader);
	}

	FontRenderer* fontRenderer = new FontRenderer(&face);
	fontRenderer->SetColor(glm::vec3(1.0, 1.0, 1.0));
	fontRenderer->UpdateRenderTargetSize(width, height);

	FT_Done_Face(face);
	FT_Done_FreeType(freeType);
	
	return fontRenderer;
}
