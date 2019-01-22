#include <EnginePch.h>
#include <Graphics/Textures/TextureCube.h>

TextureCube::TextureCube(const void* ppInitalData[6], TEX_FORMAT format, uint32 width, uint32 height, const TextureParams& params)
	: Texture(),
	m_Width(0),
	m_Height(0)
{
	Create(ppInitalData, format, width, format, params);
}

TextureCube::~TextureCube()
{
}

void TextureCube::Create(const void** ppInitalData, TEX_FORMAT format, uint32 width, uint32 height, const TextureParams& params)
{
	glGenTextures(1, &m_Texture);
	Texture::SetParameters(params);

	glBindTexture(GL_TEXTURE_2D, m_Texture);

	uint32 glformat = Texture::TexFormatToGL(format);
	uint32 internalFormat = Texture::TexFormatToGLInternal(format);
	uint32 type = Texture::TexFormatToGLType(format);
	for (uint32 i = 0; i < 6; i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, width, height, 0, glformat, type, ppInitalData[i]);
	}

	m_Width = width;
	m_Height = height;
	m_Format = format;

	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}