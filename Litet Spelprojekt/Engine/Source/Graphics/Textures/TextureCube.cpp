#include <EnginePch.h>
#include <Graphics/Textures/TextureCube.h>
#include <Graphics/Renderers/GLContext.h>

#include <STB/stb_image.h>

/*TextureCube::TextureCube(const void* ppInitalData[6], TEX_FORMAT format, uint32 width, uint32 height, const TextureParams& params)
	: Texture(),
	m_Width(0),
	m_Height(0)
{
	Create(ppInitalData, format, width, format, params);
}*/

TextureCube::TextureCube(const char* const paths[6], TEX_FORMAT format, const TextureParams & params)
	: Texture(),
	m_Width(0),
	m_Height(0)
{
	Create(paths, format, params);
}

TextureCube::~TextureCube()
{
}

/*void TextureCube::Create(const void** ppInitalData, TEX_FORMAT format, uint32 width, uint32 height, const TextureParams& params)
{
	GL_CALL(glGenTextures(1, &m_Texture));
	Texture::SetParameters(params);

	GL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, m_Texture));

	uint32 glformat = Texture::TexFormatToGL(format);
	uint32 internalFormat = Texture::TexFormatToGLInternal(format);
	uint32 type = Texture::TexFormatToGLType(format);
	for (uint32 i = 0; i < 6; i++)
	{
		GL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, width, height, 0, glformat, type, ppInitalData[i]));
	}

	m_Width = width;
	m_Height = height;
	m_Format = format;

	GL_CALL(glGenerateMipmap(GL_TEXTURE_CUBE_MAP));
	GL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
}*/

void TextureCube::Create(const char* const paths[6], TEX_FORMAT format, const TextureParams & params)
{
	bool res = true;
	m_Type = GL_TEXTURE_CUBE_MAP;
	GL_CALL(glGenTextures(1, &m_Texture));
	Texture::SetParameters(params);

	GL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, m_Texture));
	uint32 glFormat = Texture::TexFormatToGL(format);
	uint32 internalFormat = Texture::TexFormatToGLInternal(format);
	uint32 type = Texture::TexFormatToGLType(format);
	
	for (int i = 0; i < 6; i++)
	{
		uint8* pData = stbi_load(paths[i], (int*)&m_Width, (int*)&m_Height, (int*)&format, 0);

		if (pData)
		{
			GL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, m_Width, m_Height, 0, glFormat, type, pData));
			stbi_image_free(pData);
		}
		else
		{
			std::cout << "ERROR! Cubemap texture failed to load at path: " << paths[i] << std::endl;
			res = false;
		}
	}
	if (res)
	{
		GL_CALL(glGenerateMipmap(GL_TEXTURE_CUBE_MAP));
		GL_CALL(glBindTexture(GL_TEXTURE_CUBE_MAP, 0));
		m_Format = format;
		std::cout << "Loaded CubeMap successfully" << std::endl;
	}
	else
	{
		std::cout << "ERROR! CubeMap failed to load images";
	}
}