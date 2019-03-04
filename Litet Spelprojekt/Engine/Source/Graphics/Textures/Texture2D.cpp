#include <EnginePch.h>
#include <Graphics/Textures/Texture2D.h>
#include <Graphics/Renderers/GLContext.h>

Texture2D::Texture2D(const void* pInitalData, const TextureDesc& desc, const TextureParams& params)
	: Texture(),
	m_pTextureData(nullptr)
{
	if (desc.Samples > 1)
	{
		CreateMS(desc, params);
	}
	else
	{
		Create(pInitalData, desc, params);
	}
}

Texture2D::Texture2D(const char* const path, TEX_FORMAT format, bool generateMipmaps, bool flipVertically, const TextureParams& params)
	: Texture(),
	m_pTextureData(nullptr)
{
	Create(path, format, generateMipmaps, flipVertically, params);
}

Texture2D::~Texture2D()
{
}

void Texture2D::Create(const void* pInitalData, const TextureDesc& desc, const TextureParams& params)
{
	GL_CALL(glGenTextures(1, &m_Texture));
	m_Type = GL_TEXTURE_2D;

	Texture::SetParameters(params);

	GL_CALL(glBindTexture(GL_TEXTURE_2D, m_Texture));

	uint32 glformat = Texture::TexFormatToGL(desc.Format);
	uint32 internalFormat = Texture::TexFormatToGLInternal(desc.Format);
	uint32 type = Texture::TexFormatToGLType(desc.Format);
	GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, desc.Width, desc.Height, 0, glformat, type, pInitalData));
	if (desc.GenerateMips)
	{
		GL_CALL(glGenerateMipmap(GL_TEXTURE_2D));
	}
	else
	{
		//std::cout << "GenerateMipmaps turned off for Texture2D" << std::endl;
	}

	m_Desc = desc;
	
	//std::cout << "Created Texture2D" << std::endl;

	GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture2D::CreateMS(const TextureDesc& desc, const TextureParams& params)
{
	GL_CALL(glGenTextures(1, &m_Texture));
	m_Type = GL_TEXTURE_2D_MULTISAMPLE;

	Texture::SetParameters(params);

	GL_CALL(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_Texture));

	uint32 glformat = Texture::TexFormatToGL(desc.Format);
	uint32 internalFormat = Texture::TexFormatToGLInternal(desc.Format);
	uint32 type = Texture::TexFormatToGLType(desc.Format);
	GL_CALL(glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, desc.Samples, internalFormat, desc.Width, desc.Height, false));
	
	if (desc.GenerateMips)
	{
		std::cout << "ERROR: GenerateMipmaps is not available for textures with MSAA" << std::endl;
	}

	m_Desc = desc;

	std::cout << "Created Texture2D with " << desc.Samples << "X MSAA"<< std::endl;

	GL_CALL(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0));
}

Texture2D* Texture2D::CreateTextureFromFile(const char* pPath, TEX_FORMAT format, bool generateMipmaps, bool flipVertically, const TextureParams& params)
{
	return new Texture2D(pPath, format, generateMipmaps, flipVertically, params);
}

Texture2D* Texture2D::CreateTextureFromMemory(const void* pInitalData, const TextureDesc& desc, const TextureParams& params)
{
	return new Texture2D(pInitalData, desc, params);
}

void Texture2D::Create(const char* const path, TEX_FORMAT format, bool generateMipmaps, bool flipVertically, const TextureParams& params)
{
	int nrChannels;
	m_Params = params;

	stbi_set_flip_vertically_on_load(flipVertically);

	int width, height;
	if (Texture::TexFormatToGLType(format) == GL_FLOAT)
	{
		stbi_set_flip_vertically_on_load(true);
		m_pTextureData = stbi_loadf(path, &width, &height, &nrChannels, FormatToNrChannels(format));
	}
	else
	{
		m_pTextureData = stbi_load(path, &width, &height, &nrChannels, FormatToNrChannels(format));
	}

	m_Desc.Format = format;
	m_Desc.Width = width;
	m_Desc.Height = height;
	m_Params = params;

	if (m_pTextureData == nullptr)
	{
		std::cout << "Error: Could not load texture '" << path << "'" << std::endl;
		return;
	}
}

void Texture2D::Construct()
{
	if (m_pTextureData)
	{
		GL_CALL(glGenTextures(1, &m_Texture));
		m_Type = GL_TEXTURE_2D;

		SetParameters(m_Params);

		GL_CALL(glBindTexture(GL_TEXTURE_2D, m_Texture));

		uint32 glformat = Texture::TexFormatToGL(m_Desc.Format);
		uint32 internalFormat = Texture::TexFormatToGLInternal(m_Desc.Format);
		GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_Desc.Width, m_Desc.Height, 0, glformat, Texture::TexFormatToGLType(m_Desc.Format), m_pTextureData));

		if (m_Desc.GenerateMips)
		{
			GL_CALL(glGenerateMipmap(GL_TEXTURE_2D));
		}
		else
		{
			//std::cout << "GenerateMipmaps turned off for Texture2D" << std::endl;
		}

		//std::cout << "Loaded Texture2D" << std::endl;

		GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));

		stbi_image_free(m_pTextureData);
	}
}