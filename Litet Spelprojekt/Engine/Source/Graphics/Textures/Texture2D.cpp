#include <EnginePch.h>
#include <Graphics/Textures/Texture2D.h>

Texture2D::Texture2D(const void* pInitalData, unsigned int format, unsigned int width, unsigned int height, const TextureParams& params)
	: Texture(),
	m_Width(0),
	m_Height(0),
	m_Format(0)
{
	Create(pInitalData, format, width, height, params);
}

Texture2D::~Texture2D()
{
}

void Texture2D::Create(const void* pInitalData, unsigned int format, unsigned int width, unsigned int height, const TextureParams& params)
{
	glGenTextures(1, &m_Texture);
	glBindTexture(GL_TEXTURE_2D, m_Texture);

	SetParameters(params);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pInitalData);
	glGenerateMipmap(GL_TEXTURE_2D);

	m_Width = width;
	m_Height = height;
	
	glBindTexture(GL_TEXTURE_2D, 0);
}