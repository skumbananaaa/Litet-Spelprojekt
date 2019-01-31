#include <EnginePch.h>
#include <Graphics/Textures/Texture.h>
#include <Graphics/Renderers/GLContext.h>

Texture::Texture() noexcept
	: m_Format(TEX_FORMAT_UNKNOWN),
	m_Texture(0)
{
}

Texture::~Texture()
{
	if (glIsTexture(m_Texture))
	{
		GL_CALL(glDeleteTextures(1, &m_Texture));
		m_Texture = 0;
	}
}

void Texture::SetParameters(const TextureParams& params) noexcept
{
	if (m_Type != GL_TEXTURE_2D_MULTISAMPLE)
	{
		GL_CALL(glBindTexture(m_Type, m_Texture));
		GL_CALL(glTexParameteri(m_Type, GL_TEXTURE_MIN_FILTER, TexParamToGL(params.MinFilter)));
		GL_CALL(glTexParameteri(m_Type, GL_TEXTURE_MAG_FILTER, TexParamToGL(params.MagFilter)));
		GL_CALL(glTexParameteri(m_Type, GL_TEXTURE_WRAP_S, TexParamToGL(params.Wrap)));
		GL_CALL(glTexParameteri(m_Type, GL_TEXTURE_WRAP_T, TexParamToGL(params.Wrap)));
		GL_CALL(glBindTexture(m_Type, 0));
	}
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

uint32 Texture::TexFormatToGL(TEX_FORMAT format) noexcept
{
	static uint32 s_TexFormatTable[TEX_FORMAT_COUNT] =
	{
		0,
		GL_RED,
		GL_RED,
		GL_RED,
		GL_RG,
		GL_RGB,
		GL_RGBA,
		GL_RGBA,
		GL_DEPTH_COMPONENT,
		GL_DEPTH_STENCIL,
	};

	return s_TexFormatTable[format];
}

uint32 Texture::TexFormatToGLInternal(TEX_FORMAT format) noexcept
{
	static uint32 s_TexFormatTable[TEX_FORMAT_COUNT] =
	{
		0,
		GL_RED,
		GL_R16F,
		GL_R32F,
		GL_RG,
		GL_RGB,
		GL_RGBA,
		GL_RGBA16F,
		GL_DEPTH_COMPONENT,
		GL_DEPTH24_STENCIL8,
	};

	return s_TexFormatTable[format];
}

uint32 Texture::TexFormatToGLType(TEX_FORMAT format) noexcept
{
	static uint32 s_TexTypeTable[TEX_FORMAT_COUNT] =
	{
		0,
		GL_UNSIGNED_BYTE,
		GL_FLOAT,
		GL_FLOAT,
		GL_UNSIGNED_BYTE,
		GL_UNSIGNED_BYTE,
		GL_UNSIGNED_BYTE,
		GL_FLOAT,
		GL_UNSIGNED_INT,
		GL_UNSIGNED_INT_24_8,
	};

	return s_TexTypeTable[format];
}

uint32 Texture::FormatToNrChannels(TEX_FORMAT format) noexcept
{
	switch (format)
	{
	case TEX_FORMAT::TEX_FORMAT_R:
		return 1;
		
	case TEX_FORMAT::TEX_FORMAT_RG:
		return 2;

	case TEX_FORMAT::TEX_FORMAT_RGB:
		return 3;

	case TEX_FORMAT::TEX_FORMAT_RGBA:
		return 4;

	case TEX_FORMAT::TEX_FORMAT_RGBA16F:
		return 4;

	default:
		std::cout << "ERROR: Format could not be converted to nr of channels" << std::endl;
		return 0;
	}
}
