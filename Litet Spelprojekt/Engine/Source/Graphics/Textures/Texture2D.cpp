#include <EnginePch.h>
#include <Graphics/Textures/Texture2D.h>

Texture2D::Texture2D(const void* pInitalData, unsigned int format, unsigned int width, unsigned int height)
	: Texture(),
	m_Width(0),
	m_Height(0),
	m_Format(0)
{
	Create(pInitalData, format, width, height);
}

Texture2D::~Texture2D()
{
}

void Texture2D::Create(const void* pInitalData, unsigned int format, unsigned int width, unsigned int height)
{
	glGenTextures(1, &m_Texture);
	glBindTexture(GL_TEXTURE_2D, m_Texture);


	glBindTexture(GL_TEXTURE_2D, 0);
}