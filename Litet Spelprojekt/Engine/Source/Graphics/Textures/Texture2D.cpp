#include <EnginePch.h>
#include <Graphics/Textures/Texture2D.h>

Texture2D::Texture2D(const void* pInitalData, TEX_FORMAT format, unsigned int width, unsigned int height, const TextureParams& params)
	: Texture(),
	m_Width(0),
	m_Height(0),
	m_Format(TEX_FORMAT_UNKNOWN)
{
	Create(pInitalData, format, width, height, params);
}

Texture2D::~Texture2D()
{
}

void Texture2D::Create(const void* pInitalData, TEX_FORMAT format, unsigned int width, unsigned int height, const TextureParams& params)
{
	glGenTextures(1, &m_Texture);
	glBindTexture(GL_TEXTURE_2D, m_Texture);

	SetParameters(params);
	
	glTexImage2D(GL_TEXTURE_2D, 0, TexFormatToGL(format), width, height, 0, TexFormatToGLInternal(format), GL_UNSIGNED_BYTE, pInitalData);
	glGenerateMipmap(GL_TEXTURE_2D);

	m_Width = width;
	m_Height = height;
	m_Format = format;
	
	glBindTexture(GL_TEXTURE_2D, 0);
}