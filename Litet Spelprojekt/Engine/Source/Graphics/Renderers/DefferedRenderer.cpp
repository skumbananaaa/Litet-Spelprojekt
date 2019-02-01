#include <EnginePch.h>
#include <Graphics/Renderers/DefferedRenderer.h>
#include <System/Window.h>
#include <System/Application.h>

#define REFLECTIONSIZE 384

DefferedRenderer::DefferedRenderer()
	: m_pGBufferCBR(nullptr),
	m_pReflection(nullptr),
	m_pTriangle(nullptr),
	m_pDecalMesh(nullptr),
	m_pForwardCBR(nullptr),
	m_pBlur(nullptr),
	m_pLastResolveTarget(nullptr),
	m_pCurrentResolveTarget(nullptr),
	m_pGPassVSPerFrame(nullptr),
	m_pGeoPassPerObject(nullptr),
	m_pDecalPassPerFrame(nullptr),
	m_pDecalPassPerObject(nullptr),
	m_pLightPassBuffer(nullptr),
	m_pWaterPassPerFrame(nullptr),
	m_pWaterPassPerObject(nullptr),
	m_pWaterNormalMap(nullptr),
	m_pWaterDistortionMap(nullptr),
	m_pForwardPass(nullptr),
	m_pCbrBlurProgram(nullptr),
	m_pCbrReconstructionProgram(nullptr),
	m_pCbrResolveProgram(nullptr),
	m_pCbrStencilProgram(nullptr),
	m_pDepthPrePassProgram(nullptr),
	m_pGeometryPassProgram(nullptr),
	m_pDecalsPassProgram(nullptr),
	m_pLightPassProgram(nullptr),
	m_pWaterpassProgram(nullptr),
	m_pResolveTargets(),
	m_FrameCount(0)
{
	Create();
}

DefferedRenderer::~DefferedRenderer()
{
	DeleteSafe(m_pGBufferCBR);
	DeleteSafe(m_pReflection);
	DeleteSafe(m_pBlur);
	DeleteSafe(m_pForwardCBR);

	for (uint32 i = 0; i < 2; i++)
	{
		DeleteSafe(m_pResolveTargets[i]);
	}

	DeleteSafe(m_pTriangle);
	DeleteSafe(m_pDecalMesh);
	
	DeleteSafe(m_pGPassVSPerFrame);
	DeleteSafe(m_pGeoPassPerObject);
	DeleteSafe(m_pLightPassBuffer);

	DeleteSafe(m_pDecalPassPerFrame);
	DeleteSafe(m_pDecalPassPerObject);

	DeleteSafe(m_pWaterPassPerFrame);
	DeleteSafe(m_pWaterPassPerObject);
	
	DeleteSafe(m_pWaterNormalMap);
	DeleteSafe(m_pWaterDistortionMap);
	
	DeleteSafe(m_pCbrBlurProgram);
	DeleteSafe(m_pCbrReconstructionProgram);
	DeleteSafe(m_pCbrResolveProgram);
	DeleteSafe(m_pCbrStencilProgram);
	DeleteSafe(m_pDepthPrePassProgram);
	DeleteSafe(m_pGeometryPassProgram);
	DeleteSafe(m_pDecalsPassProgram);
	DeleteSafe(m_pLightPassProgram);
	DeleteSafe(m_pForwardPass);
	DeleteSafe(m_pWaterpassProgram);
}

void DefferedRenderer::DrawScene(const Scene& scene, float dtS) const
{
	GLContext& context = Application::GetInstance().GetGraphicsContext();
	
	//Setup for start rendering
	context.Enable(DEPTH_TEST);
	context.Enable(CULL_FACE);
	context.Disable(BLEND);

	context.SetClearColor(0.392f, 0.584f, 0.929f, 1.0f);
	context.SetClearDepth(1.0f);

	//Render reflection for water
	context.SetViewport(m_pReflection->GetWidth(), m_pReflection->GetHeight(), 0, 0);
	context.SetFramebuffer(m_pReflection);
	context.Clear(CLEAR_FLAG_COLOR | CLEAR_FLAG_DEPTH);

	WaterReflectionPass(scene);

	//Render geometry to MSAA targets for checkerboard rendering
	context.Enable(MULTISAMPLE);
	context.SetViewport(m_pGBufferCBR->GetWidth(), m_pGBufferCBR->GetHeight(), 0, 0);
	context.SetFramebuffer(m_pGBufferCBR);
	context.Clear(CLEAR_FLAG_COLOR | CLEAR_FLAG_DEPTH);

	//First the deffered rendering passes
	GeometryPass(scene.GetCamera(), scene);
	DecalPass(scene.GetCamera(), scene);
	
	//Then the forwards passes (Only water for now)
	context.SetViewport(m_pForwardCBR->GetWidth(), m_pForwardCBR->GetHeight(), 0, 0);
	context.SetFramebuffer(m_pForwardCBR);
	context.Clear(CLEAR_FLAG_COLOR | CLEAR_FLAG_DEPTH);

	WaterPass(scene, dtS);
	context.Disable(MULTISAMPLE);

	//Resolve the gbuffer (aka we render the rendertargets to a non-MSAA target)
	context.SetFramebuffer(m_pCurrentResolveTarget);
	context.SetViewport(m_pCurrentResolveTarget->GetWidth(), m_pCurrentResolveTarget->GetHeight(), 0, 0);
	context.Disable(DEPTH_TEST);
	GBufferResolvePass(scene.GetCamera(), scene, m_pGBufferCBR);

	//Render to the window, now we want to put everything together
	context.SetViewport(Window::GetCurrentWindow().GetWidth(), Window::GetCurrentWindow().GetHeight(), 0, 0);
	context.SetFramebuffer(m_pBlur);
	
	context.Enable(DEPTH_TEST);
	context.SetDepthFunc(FUNC_ALWAYS);

	ReconstructionPass();
	
	context.SetDepthFunc(FUNC_LESS);

	m_FrameCount++;
	m_pLastResolveTarget = m_pCurrentResolveTarget;
	m_pCurrentResolveTarget = m_pResolveTargets[m_FrameCount % 2];
}

void DefferedRenderer::Create() noexcept
{
	std::cout << "Creating deffered renderer" << std::endl;

	//We can destroy desc when gbuffer is created
	{
		TextureParams params = {};
		params.MinFilter = TEX_PARAM_NEAREST;
		params.MagFilter = TEX_PARAM_NEAREST;
		params.Wrap = TEX_PARAM_EDGECLAMP;

		FramebufferDesc desc = {};
		desc.ColorAttchmentFormats[0] = TEX_FORMAT_RGBA;
		desc.ColorAttchmentFormats[1] = TEX_FORMAT_RGBA;
		desc.NumColorAttachments = 2;
		desc.DepthStencilFormat = TEX_FORMAT_DEPTH;
		desc.Width = Window::GetCurrentWindow().GetWidth() / 2;
		desc.Height = Window::GetCurrentWindow().GetHeight() / 2;
		desc.SamplingParams = params;
		desc.Samples = 2;

		m_pGBufferCBR = new Framebuffer(desc);

		desc.ColorAttchmentFormats[0] = TEX_FORMAT_RGBA;
		desc.NumColorAttachments = 1;

		m_pForwardCBR = new Framebuffer(desc);
	}

	{
		TextureParams params = {};
		params.MinFilter = TEX_PARAM_NEAREST;
		params.MagFilter = TEX_PARAM_NEAREST;
		params.Wrap = TEX_PARAM_EDGECLAMP;

		FramebufferDesc desc = {};
		desc.ColorAttchmentFormats[0] = TEX_FORMAT_RGBA;
		desc.ColorAttchmentFormats[1] = TEX_FORMAT_R32F;
		desc.NumColorAttachments = 2;
		desc.Width = m_pGBufferCBR->GetWidth() * 2;
		desc.Height = m_pGBufferCBR->GetHeight();
		desc.SamplingParams = params;
		desc.Samples = 1;

		for (uint32 i = 0; i < 2; i++)
		{
			m_pResolveTargets[i] = new Framebuffer(desc);
		}

		m_pCurrentResolveTarget = m_pResolveTargets[m_FrameCount % 2];
		m_pLastResolveTarget = m_pCurrentResolveTarget;
	}

	{
		TextureParams params = {};
		params.Wrap = TEX_PARAM_REPEAT;
		params.MinFilter = TEX_PARAM_LINEAR;
		params.MagFilter = TEX_PARAM_LINEAR;

		FramebufferDesc desc = {};
		desc.ColorAttchmentFormats[0] = TEX_FORMAT_RGBA;
		desc.NumColorAttachments = 1;
		desc.SamplingParams = params;
		desc.DepthStencilFormat = TEX_FORMAT_DEPTH;
		desc.Width = REFLECTIONSIZE;
		desc.Height = REFLECTIONSIZE;

		m_pReflection = new Framebuffer(desc);
	}

	{
		TextureParams params = {};
		params.Wrap = TEX_PARAM_REPEAT;
		params.MinFilter = TEX_PARAM_LINEAR;
		params.MagFilter = TEX_PARAM_LINEAR;

		FramebufferDesc desc = {};
		desc.ColorAttchmentFormats[0] = TEX_FORMAT_RGBA;
		desc.NumColorAttachments = 1;
		desc.SamplingParams = params;
		desc.DepthStencilFormat = TEX_FORMAT_UNKNOWN;
		desc.Width = Window::GetCurrentWindow().GetWidth();
		desc.Height = Window::GetCurrentWindow().GetHeight();

		m_pBlur = new Framebuffer(desc);
	}

	{
		m_pDecalMesh = ResourceHandler::GetMesh(MESH::CUBE);
		m_pTriangle = new FullscreenTri();
	}

	Shader fullscreenTri = Shader();
	if (fullscreenTri.CompileFromFile("Resources/Shaders/fullscreenTriVert.glsl", VERTEX_SHADER))
	{
		std::cout << "Created fullscreen Vertex shader" << std::endl;
	}

	{
		Shader* pFrag = new Shader();
		if (pFrag->CompileFromFile("Resources/Shaders/cbrResolveFrag.glsl", FRAGMENT_SHADER))
		{
			std::cout << "Created CBR Resolve Fragment shader" << std::endl;
		}

		m_pCbrResolveProgram = new ShaderProgram(fullscreenTri, *pFrag);
		
		delete pFrag;
	}

	{
		Shader* pFrag = new Shader();
		if (pFrag->CompileFromFile("Resources/Shaders/cbrReconstructionFrag.glsl", FRAGMENT_SHADER))
		{
			std::cout << "Created CBR Reconstruction Fragment shader" << std::endl;
		}

		m_pCbrReconstructionProgram = new ShaderProgram(fullscreenTri, *pFrag);

		delete pFrag;
	}

	{
		Shader* pFrag = new Shader();
		if (pFrag->CompileFromFile("Resources/Shaders/cbrFilterFrag.glsl", FRAGMENT_SHADER))
		{
			std::cout << "Created CBR Blur Fragment shader" << std::endl;
		}

		m_pCbrBlurProgram = new ShaderProgram(fullscreenTri, *pFrag);

		delete pFrag;
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
		Shader* pFrag = new Shader();
		if (pFrag->CompileFromFile("Resources/Shaders/defferedLightningFrag.glsl", FRAGMENT_SHADER))
		{
			std::cout << "Created Lightpass Fragment shader" << std::endl;
		}

		m_pLightPassProgram = new ShaderProgram(fullscreenTri, *pFrag);

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


	{
		Shader* pVert = new Shader();
		if (pVert->CompileFromFile("Resources/Shaders/forwardVert.glsl", VERTEX_SHADER))
		{
			std::cout << "Created Forward-Pass Vertex shader" << std::endl;
		}

		Shader* pFrag = new Shader();
		if (pFrag->CompileFromFile("Resources/Shaders/forwardFrag.glsl", FRAGMENT_SHADER))
		{
			std::cout << "Created Forward-Pass Fragment shader" << std::endl;
		}

		m_pForwardPass = new ShaderProgram(*pVert, *pFrag);

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
		object.HasNormalMap = 0.0f;
		object.HasTexture = 0.0f;
		object.Model = glm::mat4(1.0f);

		m_pGeoPassPerObject = new UniformBuffer(&object, 1, sizeof(GeometryPassPerObject));
	}

	{
		DecalPassPerFrame object = {};
		object.ViewProj = glm::mat4(1.0f);
		object.InverseView = glm::mat4(1.0f);
		object.InverseProjection = glm::mat4(1.0f);

		m_pDecalPassPerFrame = new UniformBuffer(&object, 1, sizeof(DecalPassPerFrame));
	}

	{
		DecalPassPerObject object = {};
		object.Model = glm::mat4(1.0f);
		object.InverseModel = glm::mat4(1.0f);

		m_pDecalPassPerObject = new UniformBuffer(&object, 1, sizeof(DecalPassPerObject));
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
		m_pWaterDistortionMap = ResourceHandler::GetTexture2D(TEXTURE::WATER_DISTORTION);
		m_pWaterNormalMap = ResourceHandler::GetTexture2D(TEXTURE::WATER_NORMAL);
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

void DefferedRenderer::DecalPass(const Camera& camera, const Scene& scene) const noexcept
{
	GLContext& context = Application::GetInstance().GetGraphicsContext();

	context.SetProgram(m_pDecalsPassProgram);
	
	context.SetDepthMask(false);
	context.Disable(DEPTH_TEST);
	context.Disable(CULL_FACE);
	context.Enable(BLEND);

	context.SetUniformBuffer(m_pDecalPassPerFrame, 0);
	context.SetUniformBuffer(m_pDecalPassPerObject, 1);

	context.SetTexture(m_pGBufferCBR->GetDepthAttachment(), 2);

	DecalPassPerFrame perFrame = {};
	perFrame.ViewProj = camera.GetCombinedMatrix();
	perFrame.InverseView = camera.GetInverseViewMatrix();
	perFrame.InverseProjection = camera.GetInverseProjectionMatrix();
	m_pDecalPassPerFrame->UpdateData(&perFrame);

	DecalPassPerObject perObject = {};
	for (uint32 i = 0; i < scene.GetGameObjects().size(); i++)
	{
		GameObject& gameobject = *scene.GetGameObjects()[i];
		if (gameobject.HasDecal())
		{
			perObject.Model = gameobject.GetTransform();
			perObject.InverseModel = gameobject.GetInverseTransform();
			perObject.Direction = perObject.Model * glm::vec4(0.0f, 1.0f, 0.0f, 0.0f);

			if (gameobject.GetDecal().HasTexture())
			{
				context.SetTexture(gameobject.GetDecal().GetTexture(), 0);
			}

			if (gameobject.GetDecal().HasNormalMap())
			{
				context.SetTexture(gameobject.GetDecal().GetNormalMap(), 1);
			}

			m_pDecalPassPerObject->UpdateData(&perObject);
			context.DrawIndexedMesh(*m_pDecalMesh);
		}
	}

	//Unbind = no bugs
	context.SetUniformBuffer(nullptr, 0);
	context.SetUniformBuffer(nullptr, 1);

	context.SetTexture(nullptr, 0);
	context.SetTexture(nullptr, 1);
	context.SetTexture(nullptr, 2);

	context.SetDepthMask(true);
	context.Disable(BLEND);
	context.Enable(CULL_FACE);
	context.Enable(DEPTH_TEST);
}

void DefferedRenderer::GBufferResolvePass(const Camera& camera, const Scene& scene, const Framebuffer* const pGBuffer)  const noexcept
{
	GLContext& context = Application::GetInstance().GetGraphicsContext();

	context.SetProgram(m_pCbrResolveProgram);

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

		const std::vector<SpotLight*>& spotLights = scene.GetSpotLights();
		for (size_t i = 0; i < spotLights.size(); i++)
		{
			buff.SpotLights[i].Color = spotLights[i]->GetColor();
			buff.SpotLights[i].Position = spotLights[i]->GetPosition();
			buff.SpotLights[i].Direction = spotLights[i]->GetDirection();
			buff.SpotLights[i].CutOffAngle = spotLights[i]->GetCutOffAngle();
			buff.SpotLights[i].OuterCutOffAngle = spotLights[i]->GetOuterCutOffAngle();
		}

		m_pLightPassBuffer->UpdateData(&buff);
	}

	context.SetTexture(pGBuffer->GetColorAttachment(0), 0); //color buffer
	context.SetTexture(pGBuffer->GetColorAttachment(1), 1); //normal buffer
	context.SetTexture(pGBuffer->GetDepthAttachment(), 2);  //depth buffer

	context.DrawFullscreenTriangle(*m_pTriangle);

	//Unbind resources = no bugs
	context.SetUniformBuffer(nullptr, 0);

	context.SetTexture(nullptr, 0);
	context.SetTexture(nullptr, 1);
	context.SetTexture(nullptr, 2);
}

void DefferedRenderer::ReconstructionPass() const noexcept
{
	GLContext& context = GLContext::GetCurrentContext();

	context.SetProgram(m_pCbrReconstructionProgram);

	context.SetTexture(m_pCurrentResolveTarget->GetColorAttachment(0), 0);	//Color
	context.SetTexture(m_pCurrentResolveTarget->GetColorAttachment(1), 1);	//Depth
	context.SetTexture(m_pForwardCBR->GetColorAttachment(0), 2); //Forward color
	context.SetTexture(m_pForwardCBR->GetDepthAttachment(), 3); //Forward depth

	context.DrawFullscreenTriangle(*m_pTriangle);

	context.SetProgram(m_pCbrBlurProgram);

	context.SetTexture(m_pBlur->GetColorAttachment(0), 0);
	context.SetFramebuffer(nullptr);
	
	context.DrawFullscreenTriangle(*m_pTriangle);

	//Unbind = no bugs
	context.SetTexture(nullptr, 1);
	context.SetTexture(nullptr, 2);
	context.SetTexture(nullptr, 3);

	//Unbind = no bugs
	context.SetTexture(nullptr, 0);
}

void DefferedRenderer::GeometryPass(const Camera& camera, const Scene& scene) const noexcept
{
	GLContext& context = Application::GetInstance().GetGraphicsContext();

	context.SetProgram(m_pGeometryPassProgram);

	context.SetUniformBuffer(m_pGPassVSPerFrame, 0);
	context.SetUniformBuffer(m_pGeoPassPerObject, 1);

	GPassVSPerFrame perFrame = {};
	perFrame.ViewProjection = camera.GetCombinedMatrix();
	perFrame.CameraPosition = camera.GetPosition();
	m_pGPassVSPerFrame->UpdateData(&perFrame);

	GeometryPassPerObject perObject = {};
	for (uint32 i = 0; i < scene.GetGameObjects().size(); i++)
	{
		GameObject& gameobject = *scene.GetGameObjects()[i];
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

	//Unbind = no bugs
	context.SetUniformBuffer(nullptr, 0);
	context.SetUniformBuffer(nullptr, 1);

	context.SetTexture(nullptr, 0);
	context.SetTexture(nullptr, 1);
}

void DefferedRenderer::LightPass(const Camera& camera, const Scene& scene, const Framebuffer* const pGBuffer) const noexcept
{
	GLContext& context = Application::GetInstance().GetGraphicsContext();

	context.Disable(DEPTH_TEST);

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

		const std::vector<SpotLight*>& spotLights = scene.GetSpotLights();
		for (size_t i = 0; i < spotLights.size(); i++)
		{
			buff.SpotLights[i].Color = spotLights[i]->GetColor();
			buff.SpotLights[i].Position = spotLights[i]->GetPosition();
			buff.SpotLights[i].Direction = spotLights[i]->GetDirection();
			buff.SpotLights[i].CutOffAngle = spotLights[i]->GetCutOffAngle();
			buff.SpotLights[i].OuterCutOffAngle = spotLights[i]->GetOuterCutOffAngle();
		}

		m_pLightPassBuffer->UpdateData(&buff);
	}

	context.SetTexture(pGBuffer->GetColorAttachment(0), 0); //color buffer
	context.SetTexture(pGBuffer->GetColorAttachment(1), 1); //normal buffer
	context.SetTexture(pGBuffer->GetDepthAttachment(), 2);  //depth buffer

	context.DrawFullscreenTriangle(*m_pTriangle);
}

void DefferedRenderer::ForwardPass(const Camera& camera, const Scene& scene) const noexcept
{
	GLContext& context = Application::GetInstance().GetGraphicsContext();

	context.SetProgram(m_pForwardPass);

	context.SetUniformBuffer(m_pGPassVSPerFrame, 0);
	context.SetUniformBuffer(m_pGeoPassPerObject, 1);
	context.SetUniformBuffer(m_pLightPassBuffer, 2);

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

		const std::vector<SpotLight*>& spotLights = scene.GetSpotLights();
		for (size_t i = 0; i < spotLights.size(); i++)
		{
			buff.SpotLights[i].Color = spotLights[i]->GetColor();
			buff.SpotLights[i].Position = spotLights[i]->GetPosition();
			buff.SpotLights[i].Direction = spotLights[i]->GetDirection();
			buff.SpotLights[i].CutOffAngle = spotLights[i]->GetCutOffAngle();
			buff.SpotLights[i].Direction = spotLights[i]->GetDirection();
			buff.SpotLights[i].OuterCutOffAngle = spotLights[i]->GetOuterCutOffAngle();
		}

		m_pLightPassBuffer->UpdateData(&buff);
	}

	GPassVSPerFrame perFrame = {};
	perFrame.ViewProjection = camera.GetCombinedMatrix();
	perFrame.CameraPosition = camera.GetPosition();
	m_pGPassVSPerFrame->UpdateData(&perFrame);

	GeometryPassPerObject perObject = {};	
	for (uint32 i = 0; i < scene.GetGameObjects().size(); i++)
	{
		GameObject& gameobject = *scene.GetGameObjects()[i];
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

	//Unbind = no bugs
	context.SetUniformBuffer(nullptr, 0);
	context.SetUniformBuffer(nullptr, 1);
	context.SetUniformBuffer(nullptr, 2);

	context.SetTexture(nullptr, 0);
	context.SetTexture(nullptr, 1);
}

void DefferedRenderer::WaterReflectionPass(const Scene& scene) const noexcept
{
	GLContext& context = Application::GetInstance().GetGraphicsContext();

	Camera reflectionCam = scene.GetCamera();
	float reflDistance = reflectionCam.GetPosition().y * 2.0f;
	reflectionCam.SetPos(reflectionCam.GetPosition() - glm::vec3(0.0f, reflDistance, 0.0f));
	reflectionCam.InvertPitch();
	reflectionCam.UpdateFromPitchYawNoInverse();

	context.Enable(CLIP_DISTANCE0);
	ForwardPass(reflectionCam, scene);
	context.Disable(CLIP_DISTANCE0);
}

void DefferedRenderer::WaterPass(const Scene& scene, float dtS) const noexcept
{
	static float dist = 0.0f;

	GLContext& context = Application::GetInstance().GetGraphicsContext();

	//Start water with forward rendering
	context.SetProgram(m_pWaterpassProgram);

	context.SetTexture(m_pReflection->GetColorAttachment(0), 0);
	context.SetTexture(m_pWaterDistortionMap, 1);
	context.SetTexture(m_pWaterNormalMap, 2);
	context.SetTexture(m_pGBufferCBR->GetDepthAttachment(), 3);

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

	//Unbind = no bugs
	context.SetTexture(nullptr, 0);
	context.SetTexture(nullptr, 1);
	context.SetTexture(nullptr, 2);
	context.SetTexture(nullptr, 3);

	context.SetUniformBuffer(nullptr, 0);
	context.SetUniformBuffer(nullptr, 1);
}
