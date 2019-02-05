#include <EnginePch.h>
#include <Graphics/Textures/TextureCube.h>
#include <Graphics/Renderers/GLContext.h>

#include <STB/stb_image.h>
#include <GLM/gtc/matrix_transform.hpp>
#include <Graphics/Shaders/ShaderProgram.h>
#include <Graphics/Shaders/Shader.h>
#include <System/Application.h>

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

TextureCube::TextureCube(const Texture2D* tex)
	: Texture(),
	m_Width(0),
	m_Height(0)
{
	m_Type = GL_TEXTURE_CUBE_MAP;
	CreateFromPanorama(tex);
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

void TextureCube::CreateFromPanorama(const Texture2D * tex)
{
	m_Format = tex->GetFormat();

	Shader vShader;
	Shader fShader;

	vShader.CompileFromFile("Resources/Shaders/VShaderEquirecToCubemap.glsl",ShaderType::VERTEX_SHADER);
	fShader.CompileFromFile("Resources/Shaders/FShaderEquirecToCubemap.glsl",ShaderType::FRAGMENT_SHADER);

	ShaderProgram program(vShader,fShader);

	uint32 captureFBO, captureRBO;
	glGenFramebuffers(1, &captureFBO);
	glGenRenderbuffers(1, &captureRBO);

	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 1024, 1024);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

	GL_CALL(glGenTextures(1, &m_Texture));
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_Texture);
	for (uint32 i = 0; i < 6; i++)
	{
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 1024, 1024, 0, GL_RGB, GL_FLOAT, nullptr);
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

	GLContext& context = Application::GetInstance().GetGraphicsContext();

	context.SetProgram(&program);
	PanoramaBuff buff = {};
	buff.projection = captureProjection;
	buff.view = captureViews[0];
	
	UniformBuffer * pBuff = new UniformBuffer(&buff, 1, sizeof(PanoramaBuff));
	context.SetUniformBuffer(pBuff, 1);
	context.SetTexture(tex, 0);
	context.SetViewport(1024, 1024, 0, 0);

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
	//glDeleteFramebuffers(1, &captureFBO);
	//glDeleteRenderbuffers(1, &captureRBO);
}
