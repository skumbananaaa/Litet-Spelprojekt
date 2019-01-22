#include <EnginePch.h>
#include <Graphics/Textures/Texture.h>

Texture::Texture() noexcept
	: m_Format(TEX_FORMAT_UNKNOWN),
	m_Texture(0)
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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, TexParamToGL(params.Wrap));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, TexParamToGL(params.Wrap));
	glBindTexture(GL_TEXTURE_2D, 0);
}

uint32 Texture::TexParamToGL(TEX_PARAM param) noexcept
{
	static uint32 s_TexParamTable[TEX_PARAM_COUNT] =
	{ 
		0,
		GL_REPEAT,
		GL_CLAMP_TO_EDGE,
		GL_CLAMP_TO_BORDER,
		GL_NEAREST,
		GL_LINEAR,
		GL_NEAREST_MIPMAP_NEAREST,
		GL_LINEAR_MIPMAP_NEAREST,
		GL_NEAREST_MIPMAP_LINEAR,
		GL_LINEAR_MIPMAP_LINEAR,
	};

	return s_TexParamTable[param];
}

uint32 Texture::TexFormatToGL(TEX_FORMAT param) noexcept
{
	static uint32 s_TexFormatTable[TEX_FORMAT_COUNT] =
	{
		0,
		GL_RED,
		GL_RG,
		GL_RGB,
		GL_RGBA,
		GL_RGBA,
		GL_DEPTH_COMPONENT,
		GL_DEPTH_STENCIL,
	};

	return s_TexFormatTable[param];
}

uint32 Texture::TexFormatToGLInternal(TEX_FORMAT param) noexcept
{
	static uint32 s_TexFormatTable[TEX_FORMAT_COUNT] =
	{
		0,
		GL_RED,
		GL_RG,
		GL_RGB,
		GL_RGBA,
		GL_RGBA16F,
		GL_DEPTH_COMPONENT,
		GL_DEPTH24_STENCIL8,
	};

	return s_TexFormatTable[param];
}

uint32 Texture::TexFormatToGLType(TEX_FORMAT param) noexcept
{
	static uint32 s_TexTypeTable[TEX_FORMAT_COUNT] =
	{
		0,
		GL_UNSIGNED_BYTE,
		GL_UNSIGNED_BYTE,
		GL_UNSIGNED_BYTE,
		GL_UNSIGNED_BYTE,
		GL_FLOAT,
		GL_UNSIGNED_INT,
		GL_UNSIGNED_INT_24_8,
	};

	return s_TexTypeTable[param];
}
