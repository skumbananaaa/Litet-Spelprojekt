#include <EnginePch.h>
#include <Graphics/Textures/TextureCube.h>

TextureCube::TextureCube(const void** ppInitalData, TEX_FORMAT format, unsigned int width, unsigned int height, const TextureParams& params)
	: Texture(),
	m_Width(0),
	m_Height(0),
	m_Format(0)
{
	Create(ppInitalData, format, width, format, params);
}

TextureCube::~TextureCube()
{
}

void TextureCube::Create(const void** ppInitalData, TEX_FORMAT format, unsigned int width, unsigned int height, const TextureParams& params)
{
	glGenTextures(1, &m_Texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_Texture);

	SetParameters(params);

	m_Width = width;
	m_Height = height;
	m_Format = format;

	for (int i = 0; i < 6; i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, TexFormatToGL(format), width, height, 0, TexFormatToGLInternal(format), GL_UNSIGNED_BYTE, ppInitalData[i]);
	}

	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}