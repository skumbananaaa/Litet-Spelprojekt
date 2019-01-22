#pragma once

#include <System/Application.h>
#include <Graphics/GameObject.h>
#include <Graphics/Buffers\UniformBuffer.h>
#include <Graphics/Camera.h>
#include <Graphics/Scene.h>
#include <Graphics/Renderers/IRenderer.h>
#include <Graphics/Textures/Framebuffer.h>

#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>

class Game : public Application
{
public:
	Game() noexcept;
	~Game();

	void OnMouseMove(const glm::vec2& position) override;
	void OnUpdate(float dtS) override;
	void OnRender() override;

private:
	ShaderProgram* m_pShaderProgramDefault;
	ShaderProgram* m_pShaderProgramWater;

	IRenderer* m_pRenderer;
	Scene* m_pScene;

	std::vector<UniformBuffer*> m_GameObjectUniforms;

	UniformBuffer* m_pPerFrameUniform;
	float m_PerFrameArray[16 + 4 + 4];

	IndexedMesh* m_pTestMesh;

	IndexedMesh* m_pWaterMesh;
	GameObject* m_pWaterGameObject;
	UniformBuffer* m_pWaterUniform;

	Framebuffer* m_pReflectionFBO;
	Framebuffer* m_pRefractionFBO;
};