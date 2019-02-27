#include <EnginePch.h>
#include <Graphics/Textures/TextureCube.h>
#include <Graphics/Renderers/GLContext.h>
#include <Graphics/Shaders/ShaderProgram.h>
#include <Graphics/Shaders/Shader.h>
#include <System/Application.h>

TextureCube::TextureCube(const void* ppInitalData[6], const TextureDesc& desc, const TextureParams& params)
	: Texture()
{
	m_Type = GL_TEXTURE_CUBE_MAP;
	CreateFromMemory(ppInitalData, desc, params);
}

TextureCube::TextureCube(const char* const paths[6], const TextureDesc& desc, const TextureParams& params)
	: Texture()
{
	m_Type = GL_TEXTURE_CUBE_MAP;
	CreateFromFiles(paths, desc, params);
}

TextureCube::TextureCube(const Texture2D* tex)
	: Texture()
{
	m_Type = GL_TEXTURE_CUBE_MAP;
	CreateFromPanorama(tex);
}

TextureCube::~TextureCube()
{
}

void TextureCube::CreateFromMemory(const void* ppInitalData[6], const TextureDesc& desc, const TextureParams& params)
{
	GL_CALL(glGenTextures(1, &m_Texture));
	Texture::SetParameters(params);

	GL_CALL(glBindTexture(m_Type, m_Texture));

	uint32 glformat = Texture::TexFormatToGL(desc.Format);
	uint32 internalFormat = Texture::TexFormatToGLInternal(desc.Format);
	uint32 type = Texture::TexFormatToGLType(desc.Format);
	for (uint32 i = 0; i < 6; i++)
	{
		const void* pData = (ppInitalData != nullptr) ? ppInitalData[i] : nullptr;
		GL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, desc.Width, desc.Height, 0, glformat, type, pData));
	}

	m_Desc = desc;

	GL_CALL(glGenerateMipmap(m_Type));
	GL_CALL(glBindTexture(m_Type, 0));
}

void TextureCube::CreateFromFiles(const char* const ppPaths[6], const TextureDesc& desc, const TextureParams& params)
{
	GL_CALL(glGenTextures(1, &m_Texture));
	Texture::SetParameters(params);

	GL_CALL(glBindTexture(m_Type, m_Texture));

	uint32 glformat = Texture::TexFormatToGL(desc.Format);
	uint32 internalFormat = Texture::TexFormatToGLInternal(desc.Format);
	uint32 type = Texture::TexFormatToGLType(desc.Format);
	for (int i = 0; i < 6; i++)
	{
		int width, height, format;
		uint8* pData = stbi_load(ppPaths[i], &width, &height, &format, FormatToNrChannels(desc.Format));
		if (pData)
		{
			GL_CALL(glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, width, height, 0, glformat, type, pData));
			stbi_image_free(pData);
		}
		else
		{
			std::cout << "ERROR! Cubemap texture failed to load at path: " << ppPaths[i] << std::endl;
			return;
		}
	}

	GL_CALL(glGenerateMipmap(m_Type));
	GL_CALL(glBindTexture(m_Type, 0));
	
	std::cout << "Loaded CubeMap successfully" << std::endl;
}

void TextureCube::CreateFromPanorama(const Texture2D* pPanorama)
{
	m_Desc.Format = pPanorama->GetFormat();
	m_Desc.Width = pPanorama->GetWidth();
	m_Desc.Height = m_Desc.Width;

	uint32 captureFBO, captureRBO;
	glGenFramebuffers(1, &captureFBO);
	glGenRenderbuffers(1, &captureRBO);

	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, m_Desc.Width, m_Desc.Format);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

	GL_CALL(glGenTextures(1, &m_Texture));
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_Texture);
	for (uint32 i = 0; i < 6; i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, m_Desc.Width, m_Desc.Format, 0, GL_RGB, GL_FLOAT, nullptr);
	}

	TextureParams params;
	params.Wrap = TEX_PARAM_EDGECLAMP;
	params.MagFilter = TEX_PARAM_LINEAR;
	params.MinFilter = TEX_PARAM_LINEAR;
	SetParameters(params);

	glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	glm::mat4 captureViews[] =
	{
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
	   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};

	GLContext& context = GLContext::GetCurrentContext();
	context.SetProgram(ResourceHandler::GetShader(SHADER::EQUIREC_TO_CUBEMAP));
	
	struct PanoramaBuff
	{
		glm::mat4 projection;
		glm::mat4 view;
	} buff;

	buff.projection = captureProjection;
	buff.view = captureViews[0];
	
	UniformBuffer* pBuff = new UniformBuffer(&buff, 1, sizeof(PanoramaBuff));
	context.SetUniformBuffer(pBuff, 1);
	context.SetTexture(pPanorama, 0);
	context.SetViewport(m_Desc.Width, m_Desc.Height, 0, 0);

	for (uint32 i = 0; i < 6; i++)
	{
		buff.view = captureViews[i];
		pBuff->UpdateData(&buff);

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_Texture, 0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		context.DrawIndexedMesh(*ResourceHandler::GetMesh(MESH::CUBE));
	}

	delete pBuff;

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDeleteFramebuffers(1, &captureFBO);
	glDeleteRenderbuffers(1, &captureRBO);
}

TextureCube* TextureCube::CreateTextureCubeFromMemory(const void* ppInitalData[6], const TextureDesc& desc, const TextureParams& params)
{
	return new TextureCube(ppInitalData, desc, params);
}

TextureCube* TextureCube::CreateTextureCubeFromFiles(const char* const ppPaths[6], const TextureDesc& desc, const TextureParams& params)
{
	return new TextureCube(ppPaths, desc, params);
}

TextureCube* TextureCube::CreateTextureCubeFromPanorama(const Texture2D* pPanorama)
{
	return new TextureCube(pPanorama);
}
