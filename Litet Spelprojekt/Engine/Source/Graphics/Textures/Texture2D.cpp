#include <EnginePch.h>
#include <Graphics/Textures/Texture2D.h>
#define STB_IMAGE_IMPLEMENTATION
#include <STB/stb_image.h>

Texture2D::Texture2D(const void* pInitalData, TEX_FORMAT format, uint32 width, uint32 height, bool generateMipmaps, const TextureParams& params)
	: Texture(),
	m_Width(0),
	m_Height(0)
{
	Create(pInitalData, format, width, height, generateMipmaps, params);
}

Texture2D::Texture2D(const char* const path, TEX_FORMAT format, bool generateMipmaps, const TextureParams& params)
	: Texture(),
	m_Width(0),
	m_Height(0)
{
	Create(path, format, generateMipmaps, params);
}

Texture2D::~Texture2D()
{
}

void Texture2D::Create(const void* pInitalData, TEX_FORMAT format, uint32 width, uint32 height, bool generateMipmaps, const TextureParams& params)
{
	glGenTextures(1, &m_Texture);
	Texture::SetParameters(params);

	glBindTexture(GL_TEXTURE_2D, m_Texture);

	uint32 glformat = Texture::TexFormatToGL(format);
	uint32 internalFormat = Texture::TexFormatToGLInternal(format);
	uint32 type = Texture::TexFormatToGLType(format);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, glformat, type, pInitalData);
	
	if (generateMipmaps)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "GenerateMipmaps turned off for Texture2D" << std::endl;
	}

	m_Width = width;
	m_Height = height;
	m_Format = format;
	
	std::cout << "Created Texture2D" << std::endl;

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::Create(const char* const path, TEX_FORMAT format, bool generateMipmaps, const TextureParams& params)
{
	int width;
	int height;
	int nrChannels;
	uint8* textureData = stbi_load(path, &width, &height, &nrChannels, FormatToNrChannels(format));

	glGenTextures(1, &m_Texture);
	SetParameters(params);

	glBindTexture(GL_TEXTURE_2D, m_Texture);

	uint32 glformat = Texture::TexFormatToGL(format);
	uint32 internalFormat = Texture::TexFormatToGLInternal(format);
	uint32 type = Texture::TexFormatToGLType(format);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, glformat, type, textureData);

	if (generateMipmaps)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "GenerateMipmaps turned off for Texture2D" << std::endl;
	}

	m_Width = width;
	m_Height = height;
	m_Format = format;

	std::cout << "Loaded Texture2D" << std::endl;

	glBindTexture(GL_TEXTURE_2D, 0);
}
