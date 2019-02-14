#include <EnginePch.h>
#include <Graphics/Renderers/DebugRenderer.h>
#include <Graphics/Renderers/GLContext.h>
#include <Graphics/Buffers/UniformBuffer.h>
#include <System/Window.h>

DebugRenderer::DebugRenderer()
{
	Create();
}

DebugRenderer::~DebugRenderer()
{
	DeleteSafe(m_pPerFrame);
	DeleteSafe(m_pPerObject);
}

void DebugRenderer::DrawScene(const Scene& scene) const noexcept
{
	DrawDecals(scene);
}

void DebugRenderer::Create() noexcept
{
	m_pDecalProgram = ResourceHandler::GetShader(SHADER::DEBUG_DECALS);

	DebugPerFrame perFrame = {};
	perFrame.ProjectionView = glm::mat4(1.0f);
	m_pPerFrame = new UniformBuffer(&perFrame, 1, sizeof(DebugPerFrame));

	DebugPerObject perObject = {};
	perObject.Model = glm::mat4(1.0f);
	m_pPerObject = new UniformBuffer(&perObject, 1, sizeof(DebugPerObject));

	{
		m_pUnitCubeMesh = ResourceHandler::GetMesh(MESH::CUBE);
	}
}

void DebugRenderer::DrawDecals(const Scene& scene) const noexcept
{
	GLContext& context = GLContext::GetCurrentContext();

	context.Disable(DEPTH_TEST);
	context.Disable(CULL_FACE);
	context.Enable(BLEND);

	context.SetProgram(m_pDecalProgram);

	Window& window = Window::GetCurrentWindow();
	context.SetViewport(window.GetWidth(), window.GetHeight(), 0, 0);
	context.SetFramebuffer(nullptr);

	context.SetUniformBuffer(m_pPerFrame, 0);
	context.SetUniformBuffer(m_pPerObject, 1);

	DebugPerFrame perFrame = {};
	perFrame.ProjectionView = scene.GetCamera().GetCombinedMatrix();
	m_pPerFrame->UpdateData(&perFrame);

	DebugPerObject perObject = {};
	for (size_t i = 0; i < scene.GetGameObjects().size(); i++)
	{
		GameObject& gameobject = *scene.GetGameObjects()[i];
		if (gameobject.HasDecal())
		{
			perObject.Model = gameobject.GetTransform();
			m_pPerObject->UpdateData(&perObject);

			context.DrawIndexedMesh(*m_pUnitCubeMesh);
		}
	}

	//Unbind = no bugs
	context.SetUniformBuffer(nullptr, 0);
	context.SetUniformBuffer(nullptr, 1);

	context.Disable(BLEND);
	context.Enable(DEPTH_TEST);
	context.Enable(CULL_FACE);
}