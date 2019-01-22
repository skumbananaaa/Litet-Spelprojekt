#include <EnginePch.h>
#include <Graphics/Textures/Texture2D.h>

Texture2D::Texture2D(const void* pInitalData, TEX_FORMAT format, uint32 width, uint32 height, bool generateMipmaps, const TextureParams& params)
	: Texture(),
	m_Width(0),
	m_Height(0)
{
	Create(pInitalData, format, width, height, generateMipmaps, params);
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
	glTexImage2D(GL_TEXTURE_2D, 0, glformat, width, height, 0, internalFormat, GL_UNSIGNED_BYTE, pInitalData);
	
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