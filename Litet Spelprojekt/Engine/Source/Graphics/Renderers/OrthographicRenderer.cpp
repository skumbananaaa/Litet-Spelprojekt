#include <EnginePch.h>
#include <Graphics/Renderers/OrthographicRenderer.h>
#include <System/Window.h>
#include <System/Application.h>

OrthographicRenderer::OrthographicRenderer() noexcept :
	m_pOrthoPerFrame(nullptr),
	m_pOrthoPerObject(nullptr),
	m_pOrthographicProgram(nullptr)
{
	Create();
}

OrthographicRenderer::~OrthographicRenderer()
{
	DeleteSafe(m_pOrthoPerFrame);
	DeleteSafe(m_pOrthoPerObject);
}

void OrthographicRenderer::SetClipDistance(const glm::vec4& plane, uint32 index)
{
}

void OrthographicRenderer::DrawScene(const Scene& scene, float dtS) const
{
	GLContext& context = Application::GetInstance().GetGraphicsContext();

	context.Enable(DEPTH_TEST);
	context.Disable(CULL_FACE);
	context.Disable(BLEND);

	context.SetClearColor(0.392f, 0.584f, 0.929f, 1.0f);
	context.SetClearDepth(1.0f);

	context.Clear(CLEAR_FLAG_COLOR | CLEAR_FLAG_DEPTH);

	context.SetProgram(m_pOrthographicProgram);

	context.SetUniformBuffer(m_pOrthoPerFrame, 0);
	context.SetUniformBuffer(m_pOrthoPerObject, 1);

	OrthoPerFrame perFrame = {};
	perFrame.ViewProjection = scene.GetCamera().GetCombinedMatrix();
	perFrame.CameraPosition = scene.GetCamera().GetPosition();
	perFrame.CameraFront = scene.GetCamera().GetFront();
	m_pOrthoPerFrame->UpdateData(&perFrame);

	OrthoPerObject perObject = {};
	for (uint32 i = 0; i < scene.GetGameObjects().size(); i++)
	{
		GameObject& gameobject = *scene.GetGameObjects()[i];
		if (gameobject.HasMaterial() && gameobject.HasMesh())
		{
			const Material* pMaterial = gameobject.GetMaterial();
			perObject.Model = gameobject.GetTransform();
			perObject.Color = pMaterial->GetColor();

			if (pMaterial->HasDiffuseMap())
			{
				perObject.HasTexture = 1.0f;
				context.SetTexture(pMaterial->GetDiffuseMap(), 0);
			}
			else
			{
				perObject.HasTexture = 0.0f;
			}

			if (pMaterial->HasNormalMap())
			{
				perObject.HasNormalMap = 1.0f;
				context.SetTexture(pMaterial->GetNormalMap(), 1);
			}
			else
			{
				perObject.HasNormalMap = 0.0f;
			}

			m_pOrthoPerObject->UpdateData(&perObject);
			context.DrawIndexedMesh(*gameobject.GetMesh());
		}
	}
}

void OrthographicRenderer::SetWorldBuffer(const Scene& scene, const World* pWorld) const
{
}

void OrthographicRenderer::Create() noexcept
{
	std::cout << "Creating orthographic renderer" << std::endl;

	{
		OrthoPerFrame object = {};
		object.ViewProjection = glm::mat4(1.0f);
		object.CameraPosition = glm::vec3();

		m_pOrthoPerFrame = new UniformBuffer(&object, 1, sizeof(OrthoPerFrame));
	}

	{
		OrthoPerObject object = {};
		object.Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		object.HasNormalMap = 0.0f;
		object.HasTexture = 0.0f;
		object.Model = glm::mat4(1.0f);

		m_pOrthoPerObject = new UniformBuffer(&object, 1, sizeof(OrthoPerObject));
	}

	m_pOrthographicProgram = ResourceHandler::GetShader(SHADER::ORTHOGRAPHIC);
}
