#include <EnginePch.h>
#include <System/Application.h>
#include <Graphics/Renderers/ForwardRenderer.h>
#include <Graphics/Renderers/GLContext.h>

ForwardRenderer::ForwardRenderer()
	: m_pLightBuffer(nullptr),
	m_pCameraBuffer(nullptr),
	m_pMaterialBuffer(nullptr),
	m_pPlaneBuffer(nullptr),
	m_pSkyBoxPassPerFrame(nullptr),
	m_pSkyBoxPassPerObject(nullptr),
	m_pParticle(nullptr),
	m_pParticleProgram(nullptr),
	m_pDepthPrePassProgram(nullptr),
	m_pSkyBoxPassProgram(nullptr),
	m_pCurrentQuery(nullptr),
	m_FrameCounter(0),
	m_FrameTimes(),
	m_pQueries(),
	m_DrawableBatches()
{
	m_pQueries[0] = nullptr;
	m_pQueries[1] = nullptr;

	Create();
}

ForwardRenderer::~ForwardRenderer()
{
	DeleteSafe(m_pLightBuffer);
	DeleteSafe(m_pCameraBuffer);
	DeleteSafe(m_pMaterialBuffer);
	DeleteSafe(m_pPlaneBuffer);
	DeleteSafe(m_pSkyBoxPassPerFrame);
	DeleteSafe(m_pSkyBoxPassPerObject);
	DeleteSafe(m_pParticle);
	DeleteSafe(m_pQueries[0]);
	DeleteSafe(m_pQueries[1]);
}

void ForwardRenderer::DrawScene(const Scene& scene, float dtS) const
{
	static float timer = 0.0f;
	static float frametime = 0.0f;
	frametime += dtS;
	timer += dtS;

	GLContext& context = Application::GetInstance().GetGraphicsContext();

	//Set current query
	m_pCurrentQuery = m_pQueries[m_FrameCounter % 2];

	//Clear last frame's batches
	for (size_t i = 0; i < m_DrawableBatches.size(); i++)
	{
		m_DrawableBatches[i].Instances.clear();
	}

	//Create batches
	CreateBatches(scene);

	//Update lights
	UpdateLightBuffer(scene);

	//Reflections
	glQueryCounter(m_pCurrentQuery->pQueries[0], GL_TIMESTAMP);
	ReflectionPass(scene);
	glQueryCounter(m_pCurrentQuery->pQueries[1], GL_TIMESTAMP);

	//Render scene
	const Camera& mainCamera = scene.GetCamera();
	glQueryCounter(m_pCurrentQuery->pQueries[2], GL_TIMESTAMP);
	SkyBoxPass(mainCamera, scene);
	glQueryCounter(m_pCurrentQuery->pQueries[3], GL_TIMESTAMP);
	MainPass(mainCamera, scene);
	glQueryCounter(m_pCurrentQuery->pQueries[4], GL_TIMESTAMP);
	ParticlePass(mainCamera, scene);
	glQueryCounter(m_pCurrentQuery->pQueries[5], GL_TIMESTAMP);

	//Get query results
	uint64 startTime = 0;
	uint64 stopTime = 0;
	glGetQueryObjectui64v(m_pCurrentQuery->pQueries[0], GL_QUERY_RESULT, &startTime);
	glGetQueryObjectui64v(m_pCurrentQuery->pQueries[1], GL_QUERY_RESULT, &stopTime);
	m_FrameTimes.ReflectionPass += static_cast<float>(stopTime - startTime) / 1000000.0f;

	glGetQueryObjectui64v(m_pCurrentQuery->pQueries[2], GL_QUERY_RESULT, &startTime);
	glGetQueryObjectui64v(m_pCurrentQuery->pQueries[3], GL_QUERY_RESULT, &stopTime);
	m_FrameTimes.SkyboxPass += static_cast<float>(stopTime - startTime) / 1000000.0f;
	glGetQueryObjectui64v(m_pCurrentQuery->pQueries[4], GL_QUERY_RESULT, &startTime);
	m_FrameTimes.LightPass += static_cast<float>(startTime - stopTime) / 1000000.0f;
	glGetQueryObjectui64v(m_pCurrentQuery->pQueries[5], GL_QUERY_RESULT, &stopTime);
	m_FrameTimes.ParticlePass += static_cast<float>(stopTime - startTime) / 1000000.0f;

	if (timer >= 1.0f)
	{
		float fps = static_cast<float>(Application::GetInstance().GetFPS());

		std::cout << "Frametimes: (Total time: " << (frametime / fps) * 1000.0f << "ms) " << std::endl;
		std::cout << " Reflectionpass: " << m_FrameTimes.ReflectionPass / fps << "ms" << std::endl;
		std::cout << " Skyboxpass: " << m_FrameTimes.SkyboxPass / fps << "ms" << std::endl;
		std::cout << " Particlepass: " << m_FrameTimes.ParticlePass / fps << "ms" << std::endl;
		std::cout << " Lightpass: " << m_FrameTimes.LightPass / fps << "ms" << std::endl;
		std::cout << "-----------" << std::endl;

		frametime = 0.0f;
		m_FrameTimes.ReflectionPass = 0.0f;
		m_FrameTimes.SkyboxPass = 0.0f;
		m_FrameTimes.ParticlePass = 0.0f;
		m_FrameTimes.LightPass = 0.0f;

		timer = 0.0f;
	}

	m_FrameCounter++;
}

void ForwardRenderer::Create() noexcept
{
	std::cout << "Creating forward renderer" << std::endl;

	//CREATE QUERIES
	m_pQueries[0] = new TimerQuery(6);
	m_pQueries[1] = new TimerQuery(6);

	//CREATE MESHES NEEDED
	{
		m_pParticle = new Particle();
	}

	m_pDepthPrePassProgram = ResourceHandler::GetShader(SHADER::DEPTH_PRE_PASS);
	m_pSkyBoxPassProgram = ResourceHandler::GetShader(SHADER::SKYBOX_PASS);
	m_pParticleProgram = ResourceHandler::GetShader(SHADER::DEFERRED_PARTICLES);

	//We can destroy object when uniformbuffer is created
	{
		CameraBuffer buff = {};
		buff.InverseView = glm::mat4(1.0f);
		buff.InverseProjection = glm::mat4(1.0f);
		buff.CameraPosition = glm::vec3();

		m_pCameraBuffer = new UniformBuffer(&buff, 1, sizeof(CameraBuffer));
	}

	{
		MaterialBuffer buff = {};
		buff.Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

		m_pMaterialBuffer = new UniformBuffer(&buff, 1, sizeof(MaterialBuffer));
	}

	{
		PlaneBuffer buff = {};
		buff.ClipPlane = glm::vec4(0.0f);

		m_pPlaneBuffer = new UniformBuffer(&buff, 1, sizeof(PlaneBuffer));
	}

	{
		LightBuffer buff = {};
		for (uint32 i = 0; i < NUM_DIRECTIONAL_LIGHTS; i++)
		{
			buff.DirectionalLights[i].Color = glm::vec4(0.0f);
			buff.DirectionalLights[i].Direction = glm::vec3(0.0f);
		}

		for (uint32 i = 0; i < NUM_POINT_LIGHTS; i++)
		{
			buff.PointLights[i].Color = glm::vec4(0.0f);
			buff.PointLights[i].Position = glm::vec3(0.0f);
		}

		for (uint32 i = 0; i < NUM_SPOT_LIGHTS; i++)
		{
			buff.SpotLights[i].Color = glm::vec4(0.0f);
			buff.SpotLights[i].Position = glm::vec3(0.0f);
			buff.SpotLights[i].Direction = glm::vec3(0.0f);
			buff.SpotLights[i].CutOffAngle = 0.0f;
			buff.SpotLights[i].OuterCutOffAngle = 0.0f;
		}

		m_pLightBuffer = new UniformBuffer(&buff, 1, sizeof(LightBuffer));
	}

	{
		SkyBoxPassBuffer buff = {};
		buff.CameraCombined = glm::mat4(1.0f);
		buff.CameraPosition = glm::vec4();

		m_pSkyBoxPassPerFrame = new UniformBuffer(&buff, 1, sizeof(SkyBoxPassBuffer));

		SkyBoxPassPerObject object = {};
		object.model = glm::mat4(1.0f);

		m_pSkyBoxPassPerObject = new UniformBuffer(&object, 1, sizeof(SkyBoxPassPerObject));
	}
}

void ForwardRenderer::CreateBatches(const Scene& scene) const noexcept
{
	//Create batches for drawables
	//Dahlsson är detta verkligen det mest optimierade du kan göra?
	//-Nej men får duga tills vidare
	{
		const std::vector<GameObject*>& drawables = scene.GetDrawables();
		for (size_t i = 0; i < drawables.size(); i++)
		{
			bool batchFound = false;
			const Material* pMaterial = drawables[i]->GetMaterial();
			const IndexedMesh* pMesh = drawables[i]->GetMesh();

			InstanceData instance = {};
			instance.Model = drawables[i]->GetTransform();
			instance.InverseModel = drawables[i]->GetInverseTransform();

			for (size_t j = 0; j < m_DrawableBatches.size(); j++)
			{
				if (pMaterial == m_DrawableBatches[j].pMaterial && pMesh == m_DrawableBatches[j].pMesh)
				{
					m_DrawableBatches[j].Instances.push_back(instance);
					batchFound = true;
					break;
				}
			}

			if (!batchFound)
			{
				DrawableBatch batch = {};
				batch.pMaterial = pMaterial;
				batch.pMesh = pMesh;
				batch.Instances.push_back(instance);

				m_DrawableBatches.push_back(batch);
			}
		}
	}
}

void ForwardRenderer::UpdateLightBuffer(const Scene& scene) const noexcept
{
	{
		LightBuffer buff = {};

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

		m_pLightBuffer->UpdateData(&buff);
	}
}

void ForwardRenderer::SetClipDistance(const glm::vec4& plane, uint32 index)
{
}

void ForwardRenderer::UpdateCameraBuffer(const Camera& camera) const noexcept
{
}

void ForwardRenderer::ReflectionPass(const Scene& sceen) const noexcept
{
}

void ForwardRenderer::MainPass(const Camera& camera, const Scene& scene) const noexcept
{
}

void ForwardRenderer::ParticlePass(const Camera& camera, const Scene& scene) const noexcept
{
}

void ForwardRenderer::SkyBoxPass(const Camera& camera, const Scene& scene) const noexcept
{
	GLContext& context = Application::GetInstance().GetGraphicsContext();
	context.SetProgram(m_pSkyBoxPassProgram);

	context.Disable(CULL_FACE);
	context.Disable(BLEND);
	context.Disable(DEPTH_TEST);

	SkyBoxPassBuffer perFrame = {};
	perFrame.CameraCombined = camera.GetProjectionMatrix() * glm::mat4(glm::mat3(camera.GetViewMatrix()));
	perFrame.CameraPosition = glm::vec4(camera.GetPosition(), 1.0f);
	m_pSkyBoxPassPerFrame->UpdateData(&perFrame);

	SkyBoxPassPerObject perObject;
	perObject.model = glm::mat4(1.0f);
	m_pSkyBoxPassPerObject->UpdateData(&perObject);

	context.SetTexture(&scene.GetSkyBox().GetTexture(), 0);

	context.SetUniformBuffer(m_pSkyBoxPassPerObject, 0);
	context.SetUniformBuffer(m_pSkyBoxPassPerFrame, 1);

	context.DrawIndexedMesh(scene.GetSkyBox().GetMesh());

	context.Enable(DEPTH_TEST);
	context.Enable(CULL_FACE);
}
