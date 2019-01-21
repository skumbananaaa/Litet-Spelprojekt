#include <EnginePch.h>
#include <Graphics/Textures/Texture.h>

Texture::Texture()
{
}

Texture::~Texture()
{
	if (glIsTexture(m_Texture))
	{
		glDeleteTextures(1, &m_Texture);
		m_Texture = 0;
	}
}
