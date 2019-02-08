#include <EnginePch.h>
#include <Graphics/Textures/Texture2D.h>
#include <Graphics/Renderers/GLContext.h>

#define STB_IMAGE_IMPLEMENTATION
#include <STB/stb_image.h>

Texture2D::Texture2D(const void* pInitalData, const TextureDesc& desc, const TextureParams& params)
	: Texture(),
	m_Width(0),
	m_Height(0),
	m_Samples(0),
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

Texture2D::Texture2D(const char* const path, TEX_FORMAT format, bool generateMipmaps, const TextureParams& params)
	: Texture(),
	m_Width(0),
	m_Height(0),
	m_Samples(0),
	m_pTextureData(nullptr)
{
	Create(path, format, generateMipmaps, params);
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

	m_Width = desc.Width;
	m_Height = desc.Height;
	m_Format = desc.Format;
	m_Samples = desc.Samples;
	
	//std::cout << "Created Texture2D" << std::endl;

	GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
}

void Texture2D::CreateMS(const TextureDesc & desc, const TextureParams & params)
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

	m_Width = desc.Width;
	m_Height = desc.Height;
	m_Format = desc.Format;
	m_Samples = desc.Samples;

	//std::cout << "Created Texture2D with " << desc.Samples << "X MSAA"<< std::endl;

	GL_CALL(glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0));
}

void Texture2D::Create(const char* const path, TEX_FORMAT format, bool generateMipmaps, const TextureParams& params)
{
	int nrChannels;
	m_Format = format;
	m_Params = params;
	m_GenerateMipmaps = generateMipmaps;

	if (Texture::TexFormatToGLType(format) == GL_FLOAT)
	{
		stbi_set_flip_vertically_on_load(true);
		
		int channels = FormatToNrChannels(format);
		m_pTextureData = stbi_loadf(path, &m_Width, &m_Height, &nrChannels, channels);
	}
	else
	{
		m_pTextureData = stbi_load(path, &m_Width, &m_Height, &nrChannels, FormatToNrChannels(format));
	}

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

		uint32 glformat = Texture::TexFormatToGL(m_Format);
		uint32 internalFormat = Texture::TexFormatToGLInternal(m_Format);
		GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_Width, m_Height, 0, glformat, Texture::TexFormatToGLType(m_Format), m_pTextureData));

		if (m_GenerateMipmaps)
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