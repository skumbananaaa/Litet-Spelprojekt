#include <EnginePch.h>
#include <Graphics/Renderers/DefferedRenderer.h>
#include <System/Window.h>
#include <System/Application.h>

#define REFLECTIONSIZE 512

DefferedRenderer::DefferedRenderer()
	: m_pGBuffer(nullptr),
	m_pFinalFramebuffer(nullptr),
	m_pWaterGBuffer(nullptr),
	m_pReflection(nullptr),
	m_pTriangle(nullptr),
	m_pDecalMesh(nullptr),
	m_pGPassVSPerFrame(nullptr),
	m_pGeoPassPerObject(nullptr),
	m_pDecalVSPerFrame(nullptr),
	m_pDecalVSPerObject(nullptr),
	m_pDecalFSPerFrame(nullptr),
	m_pDecalFSPerObject(nullptr),
	m_pLightPassBuffer(nullptr),
	m_pWaterPassPerFrame(nullptr),
	m_pWaterPassPerObject(nullptr),
	m_pWaterNormalMap(nullptr),
	m_pWaterDistortionMap(nullptr),
	m_pDepthPrePassProgram(nullptr),
	m_pDecalsPassProgram(nullptr),
	m_pGeometryPassProgram(nullptr),
	m_pLightPassProgram(nullptr),
	m_pWaterpassProgram(nullptr)
{
	Create();
}

DefferedRenderer::~DefferedRenderer()
{
	DeleteSafe(m_pGBuffer);
	DeleteSafe(m_pFinalFramebuffer);
	DeleteSafe(m_pWaterGBuffer);
	DeleteSafe(m_pReflection);
	
	DeleteSafe(m_pTriangle);
	DeleteSafe(m_pDecalMesh);
	
	DeleteSafe(m_pGPassVSPerFrame);
	DeleteSafe(m_pGeoPassPerObject);
	DeleteSafe(m_pLightPassBuffer);

	DeleteSafe(m_pDecalVSPerFrame);
	DeleteSafe(m_pDecalVSPerObject);
	DeleteSafe(m_pDecalFSPerFrame);
	DeleteSafe(m_pDecalFSPerObject);

	DeleteSafe(m_pWaterPassPerFrame);
	DeleteSafe(m_pWaterPassPerObject);
	
	DeleteSafe(m_pWaterNormalMap);
	DeleteSafe(m_pWaterDistortionMap);
	
	DeleteSafe(m_pDepthPrePassProgram);
	DeleteSafe(m_pDecalsPassProgram);
	DeleteSafe(m_pGeometryPassProgram);
	DeleteSafe(m_pLightPassProgram);
	DeleteSafe(m_pWaterpassProgram);
}

void DefferedRenderer::DrawScene(const Scene& scene, float dtS) const
{
	GLContext& context = Application::GetInstance().GetGraphicsContext();
	
	context.Enable(DEPTH_TEST);
	context.Enable(CULL_FACE);
	context.Disable(BLEND);

	context.SetClearColor(0.392f, 0.584f, 0.929f, 1.0f);
	context.SetClearDepth(1.0f);

	//DepthPrePass(scene);

	GeometryPass(scene.GetGameObjects(), scene.GetCamera(), m_pGBuffer);
	//DecalPass(scene);
	LightPass(scene.GetCamera(), scene, nullptr, m_pGBuffer);

	context.BlitFramebuffer(nullptr, m_pGBuffer, CLEAR_FLAG_DEPTH);

	WaterPass(scene, dtS);
}

void DefferedRenderer::Create() noexcept
{
	std::cout << "Createing deffered renderer" << std::endl;

	//We can destroy desc when gbuffer is created
	{
		TextureParams params = {};
		params.MinFilter = TEX_NEAREST;
		params.MagFilter = TEX_NEAREST;
		params.Wrap = TEX_PARAM_REPEAT;

		FramebufferDesc desc = {};
		desc.ColorAttchmentFormats[0] = TEX_FORMAT_RGBA;
		desc.ColorAttchmentFormats[1] = TEX_FORMAT_RGBA;
		desc.NumColorAttachments = 2;
		desc.DepthStencilFormat = TEX_FORMAT_DEPTH;
		//desc.Width = 1920; 
		desc.Width = Window::GetCurrentWindow().GetWidth();
		//desc.Height = 1080;
		desc.Height = Window::GetCurrentWindow().GetHeight();
		desc.SamplingParams = params;

		m_pGBuffer = new Framebuffer(desc);
	}

	{
		FramebufferDesc desc = {};
		desc.ColorAttchmentFormats[0] = TEX_FORMAT_RGBA;
		desc.ColorAttchmentFormats[1] = TEX_FORMAT_RGBA;
		desc.NumColorAttachments = 2;
		desc.DepthStencilFormat = TEX_FORMAT_DEPTH;
		desc.Width = REFLECTIONSIZE;
		desc.Height = REFLECTIONSIZE;

		m_pWaterGBuffer = new Framebuffer(desc);
	}


	{
		TextureParams params = {};
		params.Wrap = TEX_PARAM_REPEAT;
		params.MinFilter = TEX_LINEAR;
		params.MagFilter = TEX_LINEAR;

		FramebufferDesc desc = {};
		desc.ColorAttchmentFormats[0] = TEX_FORMAT_RGBA;
		desc.NumColorAttachments = 1;
		desc.SamplingParams = params;
		desc.DepthStencilFormat = TEX_FORMAT_UNKNOWN;
		desc.Width = REFLECTIONSIZE;
		desc.Height = REFLECTIONSIZE;

		m_pReflection = new Framebuffer(desc);
	}

	{
		Shader* pVert = new Shader();
		if (pVert->CompileFromFile("Resources/Shaders/defferedGeometryVert.glsl", VERTEX_SHADER))
		{
			std::cout << "Created Geomtrypass Vertex shader" << std::endl;
		}

		Shader* pFrag = new Shader();
		if(pFrag->CompileFromFile("Resources/Shaders/defferedGeometryFrag.glsl", FRAGMENT_SHADER))
		{
			std::cout << "Created Geomtrypass Fragment shader" << std::endl;
		}

		m_pGeometryPassProgram = new ShaderProgram(*pVert, *pFrag);

		delete pVert;
		delete pFrag;
	}

	{
		Shader* pVert = new Shader();
		if (pVert->CompileFromFile("Resources/Shaders/defferedLightningVert.glsl", VERTEX_SHADER))
		{
			std::cout << "Created Lightpass Vertex shader" << std::endl;
		}

		Shader* pFrag = new Shader();
		if (pFrag->CompileFromFile("Resources/Shaders/defferedLightningFrag.glsl", FRAGMENT_SHADER))
		{
			std::cout << "Created Lightpass Fragment shader" << std::endl;
		}

		m_pLightPassProgram = new ShaderProgram(*pVert, *pFrag);

		delete pVert;
		delete pFrag;
	}

	{
		Shader* pVert = new Shader();
		if (pVert->CompileFromFile("Resources/Shaders/defferedDepthPreVert.glsl", VERTEX_SHADER))
		{
			std::cout << "Created DepthPrePass Vertex shader" << std::endl;
		}

		m_pDepthPrePassProgram = new ShaderProgram(*pVert);

		delete pVert;
	}

	{
		Shader* pVert = new Shader();
		if (pVert->CompileFromFile("Resources/Shaders/VShaderWater.glsl", VERTEX_SHADER))
		{
			std::cout << "Created Water Vertex shader" << std::endl;
		}

		Shader* pFrag = new Shader();
		if (pFrag->CompileFromFile("Resources/Shaders/FShaderWater.glsl", FRAGMENT_SHADER))
		{
			std::cout << "Created Water Fragment shader" << std::endl;
		}

		m_pWaterpassProgram = new ShaderProgram(*pVert, *pFrag);

		delete pVert;
		delete pFrag;
	}

	{
		Shader* pVert = new Shader();
		if (pVert->CompileFromFile("Resources/Shaders/defferedDecalsVert.glsl", VERTEX_SHADER))
		{
			std::cout << "Created Decal Vertex shader" << std::endl;
		}

		Shader* pFrag = new Shader();
		if (pFrag->CompileFromFile("Resources/Shaders/defferedDecalsFrag.glsl", FRAGMENT_SHADER))
		{
			std::cout << "Created Decal Fragment shader" << std::endl;
		}

		m_pDecalsPassProgram = new ShaderProgram(*pVert, *pFrag);

		delete pVert;
		delete pFrag;
	}

	//We can destroy object when uniformbuffer is created
	{
		GPassVSPerFrame object = {};
		object.ViewProjection = glm::mat4(1.0f);
		object.CameraPosition = glm::vec3();

		m_pGPassVSPerFrame = new UniformBuffer(&object, 1, sizeof(GPassVSPerFrame));
	}

	{
		GeometryPassPerObject object = {};
		object.Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

		m_pGeoPassPerObject = new UniformBuffer(&object, 1, sizeof(GeometryPassPerObject));
	}

	{
		DecalPassVSPerFrame object = {};
		object.ViewProj = glm::mat4(1.0f);

		m_pDecalVSPerFrame = new UniformBuffer(&object, 1, sizeof(DecalPassVSPerFrame));
	}

	{
		DecalPassVSPerObject object = {};
		object.Model = glm::mat4(1.0f);

		m_pDecalVSPerObject = new UniformBuffer(&object, 1, sizeof(DecalPassVSPerObject));
	}

	{
		DecalPassFSPerFrame object = {};
		object.InverseView = glm::mat4(1.0f);
		object.InverseProjection = glm::mat4(1.0f);

		m_pDecalFSPerFrame = new UniformBuffer(&object, 1, sizeof(DecalPassFSPerFrame));
	}

	{
		DecalPassFSPerObject object = {};
		object.InverseModel = glm::mat4(1.0f);

		m_pDecalFSPerObject = new UniformBuffer(&object, 1, sizeof(DecalPassFSPerObject));
	}

	{
		WaterPassPerFrame object = {};
		object.CameraCombined = glm::mat4(1.0f);
		object.CameraPosition = glm::vec3();
		object.DistortionMoveFactor = 0.0f;

		m_pWaterPassPerFrame = new UniformBuffer(&object, 1, sizeof(WaterPassPerFrame));
	}

	{
		WaterPassPerObjectVS object = {};
		object.Model = glm::mat4(1.0f);

		m_pWaterPassPerObject = new UniformBuffer(&object, 1, sizeof(WaterPassPerObjectVS));
	}

	{
		LightPassBuffer buff = {};
		buff.InverseView = glm::mat4(1.0f);
		buff.InverseProjection = glm::mat4(1.0f);
		buff.CameraPosition = glm::vec3();

		m_pLightPassBuffer = new UniformBuffer(&buff, 1, sizeof(LightPassBuffer));
	}

	{
		TextureParams params = {};
		params.Wrap = TEX_PARAM_REPEAT;
		params.MinFilter = TEX_LINEAR;
		params.MagFilter = TEX_LINEAR;

		m_pWaterDistortionMap = new Texture2D("Resources/Textures/waterDUDV.png", TEX_FORMAT_RGB, true, params);
		m_pWaterNormalMap = new Texture2D("Resources/Textures/waterNormalMap.png", TEX_FORMAT_RGB, true, params);
	}

	{
		m_pDecalMesh = IndexedMesh::CreateCube();
		m_pTriangle = new FullscreenTri();
	}
}

void DefferedRenderer::DepthPrePass(const Scene& scene) const noexcept
{
	GLContext& context = Application::GetInstance().GetGraphicsContext();

	context.SetProgram(m_pDepthPrePassProgram);

	context.SetColorMask(0, 0, 0, 0);
	context.SetDepthFunc(FUNC_LESS);

	GPassVSPerFrame perFrame = {};
	perFrame.ViewProjection = scene.GetCamera().GetCombinedMatrix();
	perFrame.CameraPosition = scene.GetCamera().GetPosition();
	m_pGPassVSPerFrame->UpdateData(&perFrame);

	GeometryPassPerObject perObject = {};
	for (uint32 i = 0; i < scene.GetGameObjects().size(); i++)
	{
		perObject.Model = scene.GetGameObjects()[i]->GetTransform();
		m_pGeoPassPerObject->UpdateData(&perObject);

		context.DrawIndexedMesh(scene.GetGameObjects()[i]->GetMesh());
	}

	context.SetDepthFunc(FUNC_LESS_EQUAL);
	context.SetColorMask(1, 1, 1, 1);

	context.SetDepthMask(false);
}

void DefferedRenderer::DecalPass(const Scene& scene) const noexcept
{
	GLContext& context = Application::GetInstance().GetGraphicsContext();

	context.SetProgram(m_pDecalsPassProgram);
	context.SetDepthMask(false);
	context.Enable(BLEND);
	context.Disable(CULL_FACE);

	context.SetUniformBuffer(m_pDecalVSPerFrame, 0);
	context.SetUniformBuffer(m_pDecalVSPerObject, 1);
	context.SetUniformBuffer(m_pDecalFSPerFrame, 2);
	context.SetUniformBuffer(m_pDecalFSPerObject, 3);

	context.SetTexture(m_pGBuffer->GetDepthAttachment(), 2);

	{
		DecalPassVSPerFrame perFrameVS = {};
		perFrameVS.ViewProj = scene.GetCamera().GetCombinedMatrix();
		m_pDecalVSPerFrame->UpdateData(&perFrameVS);
	}

	{
		DecalPassFSPerFrame perFrameFS = {};
		perFrameFS.InverseView = scene.GetCamera().GetInverseViewMatrix();
		perFrameFS.InverseProjection = scene.GetCamera().GetInverseProjectionMatrix();
		m_pDecalFSPerFrame->UpdateData(&perFrameFS);
	}

	DecalPassVSPerObject perObjectVS = {};
	DecalPassFSPerObject perObjectFS = {};

	for (uint32 i = 0; i < scene.GetGameObjects().size(); i++)
	{
		GameObject& gameobject = *scene.GetGameObjects()[i];
		if (gameobject.HasDecal())
		{
			perObjectVS.Model = gameobject.GetTransform();
			m_pDecalVSPerObject->UpdateData(&perObjectVS);

			perObjectFS.InverseModel = glm::inverse(gameobject.GetTransform());
			m_pDecalFSPerObject->UpdateData(&perObjectFS);

			if (gameobject.GetDecal().HasTexture())
			{
				context.SetTexture(gameobject.GetDecal().GetTexture(), 0);
			}

			if (gameobject.GetDecal().HasNormalMap())
			{
				context.SetTexture(gameobject.GetDecal().GetNormalMap(), 1);
			}

			context.DrawIndexedMesh(*m_pDecalMesh);
		}
	}

	context.SetDepthMask(true);
	context.Enable(CULL_FACE);
}

void DefferedRenderer::GeometryPass(const std::vector<GameObject*>& gameobjects, const Camera& camera, const Framebuffer* const pFramebuffer) const noexcept
{
	GLContext& context = Application::GetInstance().GetGraphicsContext();

	context.SetViewport(pFramebuffer->GetWidth(), pFramebuffer->GetHeight(), 0, 0);
	context.SetFramebuffer(pFramebuffer);
	context.Clear(CLEAR_FLAG_COLOR | CLEAR_FLAG_DEPTH);

	context.SetProgram(m_pGeometryPassProgram);

	context.SetUniformBuffer(m_pGPassVSPerFrame, 0);
	context.SetUniformBuffer(m_pGeoPassPerObject, 1);

	GPassVSPerFrame perFrame = {};
	perFrame.ViewProjection = camera.GetCombinedMatrix();
	perFrame.CameraPosition = camera.GetPosition();
	m_pGPassVSPerFrame->UpdateData(&perFrame);

	GeometryPassPerObject perObject = {};
	for (uint32 i = 0; i < gameobjects.size(); i++)
	{
		GameObject& gameobject = *gameobjects[i];
		if (gameobject.HasMaterial() && gameobject.HasMesh())
		{
			const Material& material = gameobject.GetMaterial();
			perObject.Model = gameobject.GetTransform();
			perObject.Color = material.GetColor();

			if (material.HasTexture())
			{
				perObject.HasTexture = 1.0f;
				context.SetTexture(material.GetTexture(), 0);
			}
			else
			{
				perObject.HasTexture = 0.0f;
			}

			if (material.HasNormalMap())
			{
				perObject.HasNormalMap = 1.0f;
				context.SetTexture(material.GetNormalMap(), 1);
			}
			else
			{
				perObject.HasNormalMap = 0.0f;
			}

			m_pGeoPassPerObject->UpdateData(&perObject);
			context.DrawIndexedMesh(gameobject.GetMesh());
		}
	}
}

void DefferedRenderer::LightPass(const Camera& camera, const Scene& scene, const Framebuffer* const pFramebuffer, const Framebuffer* const pGBuffer) const noexcept
{
	GLContext& context = Application::GetInstance().GetGraphicsContext();

	context.SetFramebuffer(pFramebuffer);
	context.Clear(CLEAR_FLAG_COLOR);

	context.Disable(DEPTH_TEST);

	if (pGBuffer != nullptr)
	{
		context.SetViewport(pGBuffer->GetWidth(), pGBuffer->GetHeight(), 0, 0);
	}
	else
	{
		context.SetViewport(Window::GetCurrentWindow().GetWidth(), Window::GetCurrentWindow().GetHeight(), 0, 0);
	}

	context.SetProgram(m_pLightPassProgram);
	context.SetUniformBuffer(m_pLightPassBuffer, 0);

	{
		LightPassBuffer buff = {};
		buff.InverseView = camera.GetInverseViewMatrix();
		buff.InverseProjection = camera.GetInverseProjectionMatrix();
		buff.CameraPosition = camera.GetPosition();

		const std::vector<DirectionalLight*>& directionalLights = scene.GetDirectionalLights();
		for (size_t i = 0; i < directionalLights.size(); i++)
		{
			buff.DirectionalLights[i].Color = directionalLights[i]->GetColor();
			buff.DirectionalLights[i].Direction = directionalLights[i]->GetDirection();
		}

		const std::vector<PointLight*>& pointLights = scene.GetPointLights();
		for (size_t i = 0; i < pointLights.size(); i++)
		{
			buff.PointLights[i].Color = pointLights[i]->GetColor();
			buff.PointLights[i].Position = pointLights[i]->GetPosition();
		}

		m_pLightPassBuffer->UpdateData(&buff);
	}

	context.SetTexture(pGBuffer->GetColorAttachment(0), 0);
	context.SetTexture(pGBuffer->GetColorAttachment(1), 1);
	context.SetTexture(pGBuffer->GetDepthAttachment(), 2);

	context.DrawFullscreenTriangle(*m_pTriangle);
}

void DefferedRenderer::WaterPass(const Scene& scene, float dtS) const noexcept
{
	static float dist = 0.0f;

	GLContext& context = Application::GetInstance().GetGraphicsContext();

	Camera reflectionCam = scene.GetCamera();
	float reflDistance = reflectionCam.GetPosition().y * 2.0f;
	reflectionCam.SetPos(reflectionCam.GetPosition() - glm::vec3(0.0f, reflDistance, 0.0f));
	reflectionCam.InvertPitch();
	reflectionCam.UpdateFromPitchYaw();

	context.Enable(Cap::CLIP_DISTANCE0);
	GeometryPass(scene.GetGameObjects(), reflectionCam, m_pWaterGBuffer);
	context.Disable(Cap::CLIP_DISTANCE0);
	
	LightPass(reflectionCam, scene, m_pReflection, m_pWaterGBuffer);

	//Start rendering forward
	context.SetProgram(m_pWaterpassProgram);
	context.Enable(DEPTH_TEST);

	context.SetFramebuffer(nullptr);
	context.SetViewport(Window::GetCurrentWindow().GetWidth(), Window::GetCurrentWindow().GetHeight(), 0, 0);
	
	context.SetTexture(m_pReflection->GetColorAttachment(0), 0);
	context.SetTexture(m_pWaterDistortionMap, 1);
	context.SetTexture(m_pWaterNormalMap, 2);
	context.SetTexture(m_pGBuffer->GetDepthAttachment(), 3);

	WaterPassPerFrame perFrame = {};
	perFrame.CameraCombined = scene.GetCamera().GetCombinedMatrix();
	perFrame.CameraPosition = scene.GetCamera().GetPosition();
	
	dist += 0.02f * dtS;
	perFrame.DistortionMoveFactor = dist;

	m_pWaterPassPerFrame->UpdateData(&perFrame);

	context.SetUniformBuffer(m_pWaterPassPerObject, 0);
	context.SetUniformBuffer(m_pWaterPassPerFrame, 1);

	WaterPassPerObjectVS perObject= {};
	for (uint32 i = 0; i < scene.GetGameObjects().size(); i++)
	{
		GameObject& gameobject = *scene.GetGameObjects()[i];
		if (!gameobject.HasMaterial() && gameobject.HasMesh())
		{
			perObject.Model = gameobject.GetTransform();
			m_pWaterPassPerObject->UpdateData(&perObject);

			context.DrawIndexedMesh(gameobject.GetMesh());
		}
	}
}
