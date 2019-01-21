#include <EnginePch.h>
#include <Graphics/Textures/Texture.h>

Texture::Texture() noexcept
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

void Texture::SetParameters(const TextureParams& params) noexcept
{
	glBindTexture(GL_TEXTURE_2D, m_Texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, TexParamToGL(params.MinFilter));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, TexParamToGL(params.MagFilter));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, TexParamToGL(params.Wrap));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, TexParamToGL(params.Wrap));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, TexParamToGL(params.Wrap));
	glBindTexture(GL_TEXTURE_2D, 0);
}

unsigned int Texture::TexParamToGL(TEX_PARAM param) noexcept
{
	static unsigned int s_TexParamTable[TEX_PARAM_COUNT] =
	{ 
		0,
		GL_REPEAT,
		GL_NEAREST,
		GL_LINEAR,
		GL_NEAREST_MIPMAP_NEAREST,
		GL_LINEAR_MIPMAP_NEAREST,
		GL_NEAREST_MIPMAP_LINEAR,
		GL_LINEAR_MIPMAP_LINEAR,
	};

	return s_TexParamTable[param];
}

unsigned int Texture::TexFormatToGL(TEX_FORMAT param) noexcept
{
	static unsigned int s_TexFormatTable[TEX_FORMAT_COUNT] =
	{
		0,
		GL_RED,
		GL_RG,
		GL_RGB,
		GL_RGBA,
	};

	return s_TexFormatTable[param];
}

unsigned int Texture::TexFormatToGLInternal(TEX_FORMAT param) noexcept
{
	static unsigned int s_TexFormatTable[TEX_FORMAT_COUNT] =
	{
		0,
		GL_R8,
		GL_RG8,
		GL_RGB8,
		GL_RGBA8,
	};

	return s_TexFormatTable[param];
}
