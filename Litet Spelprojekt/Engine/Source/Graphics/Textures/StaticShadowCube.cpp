#include <EnginePch.h>
#include <Graphics/Textures/StaticShadowCube.h>
#include <Graphics/GameObject.h>
#include <Graphics/Scene.h>
#include <IO/ResourceHandler.h>

StaticShadowCube::StaticShadowCube(const glm::vec3& position, const Scene& scene)
	: m_pCube(nullptr)
{
	Create(position, scene);
}

StaticShadowCube::~StaticShadowCube()
{
	DeleteSafe(m_pCube);
}

void StaticShadowCube::Create(const glm::vec3& position, const Scene& scene)
{
	{
		TextureDesc desc = {};
		desc.GenerateMips = false;
		desc.Format = TEX_FORMAT_DEPTH;
		desc.Width = SHADOW_SIZE;
		desc.Height = SHADOW_SIZE;
		desc.Samples = 1;

		TextureParams params = {};
		params.MagFilter = TEX_PARAM_NEAREST;
		params.MinFilter = TEX_PARAM_NEAREST;
		params.Wrap = TEX_PARAM_EDGECLAMP;

		m_pCube = TextureCube::CreateTextureCubeFromMemory(nullptr, desc);
		m_pCube->SetDebugName("ShadowCube");
	}

	Framebuffer* ppFrameBuffers[6] = 
	{
		new Framebuffer(nullptr, nullptr, 0, m_pCube, GL_TEXTURE_CUBE_MAP_POSITIVE_X),
		new Framebuffer(nullptr, nullptr, 0, m_pCube, GL_TEXTURE_CUBE_MAP_NEGATIVE_X),
		new Framebuffer(nullptr, nullptr, 0, m_pCube, GL_TEXTURE_CUBE_MAP_POSITIVE_Y),
		new Framebuffer(nullptr, nullptr, 0, m_pCube, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y),
		new Framebuffer(nullptr, nullptr, 0, m_pCube, GL_TEXTURE_CUBE_MAP_POSITIVE_Z),
		new Framebuffer(nullptr, nullptr, 0, m_pCube, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z),
	};

	GLContext& context = GLContext::GetCurrentContext();
	for (uint32 i = 0; i < 6; i++)
	{
		context.SetFramebuffer(ppFrameBuffers[i]);
		context.SetClearDepth(1.0f);
		context.Clear(CLEAR_FLAG_COLOR);
	}

	CameraBuffer cameraBuff = {};
	cameraBuff.ProjectionView = glm::mat4(1.0f);

	struct ModelBuffer
	{
		glm::mat4 Model = glm::mat4(1.0f);
	} modelBuff;

	UniformBuffer* pCameraBuffer = new UniformBuffer(&cameraBuff, 1, sizeof(CameraBuffer));
	UniformBuffer* pModelBuffer = new UniformBuffer(&modelBuff, 1, sizeof(ModelBuffer));

	glm::mat4 proj = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 100.0f);
	glm::mat4 cameraMatrices[6] =
	{
		proj * glm::lookAt(position, position + glm::vec3( 1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
		proj * glm::lookAt(position, position + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
		proj * glm::lookAt(position, position + glm::vec3(0.0f,  1.0f, 0.0f), glm::vec3(0.0f, 0.0f,  1.0f)),
		proj * glm::lookAt(position, position + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
		proj * glm::lookAt(position, position + glm::vec3(0.0f, 0.0f,  1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
		proj * glm::lookAt(position, position + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
	};

	context.SetColorMask(0, 0, 0, 0);
	context.SetDepthMask(true);
	context.Enable(DEPTH_TEST);
	context.SetDepthFunc(FUNC_LESS);
	context.Disable(CLIP_DISTANCE0);
	context.SetViewport(SHADOW_SIZE, SHADOW_SIZE, 0, 0);
	
	context.SetProgram(ResourceHandler::GetShader(SHADER::SHADOW));
	context.SetUniformBuffer(pCameraBuffer, 0);
	context.SetUniformBuffer(pModelBuffer, 1);

	const std::vector<GameObject*> drawables = scene.GetDrawables();
	for (uint32 i = 0; i < drawables.size(); i++)
	{
		const IndexedMesh& mesh = *(drawables[i]->GetMesh());

		modelBuff.Model = drawables[i]->GetTransform();
		pModelBuffer->UpdateData(&modelBuff);
		for (uint32 j = 0; j < 6; j++)
		{
			cameraBuff.ProjectionView = cameraMatrices[j];
			pCameraBuffer->UpdateData(&cameraBuff);

			context.SetFramebuffer(ppFrameBuffers[j]);
			context.DrawIndexedMesh(mesh);
		}
	}

	context.SetColorMask(1, 1, 1, 1);
	context.SetUniformBuffer(nullptr, 0);
	context.SetUniformBuffer(nullptr, 1);

	for (uint32 i = 0; i < 6; i++)
	{
		DeleteSafe(ppFrameBuffers[i]);
	}

	DeleteSafe(pCameraBuffer);
	DeleteSafe(pModelBuffer);

	std::cout << "Created ShadowCube" << std::endl;
}
