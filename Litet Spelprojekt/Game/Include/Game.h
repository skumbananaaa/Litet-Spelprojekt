#pragma once

#include <System/Application.h>
#include <Graphics/GameObject.h>
#include <Graphics/Buffers\UniformBuffer.h>
#include <Graphics/Camera.h>
#include <Graphics/Scene.h>
#include <Graphics/Renderers/IRenderer.h>
#include <Graphics/Textures/Framebuffer.h>
#include <Graphics/Textures/Texture2D.h>
#include <Graphics/Renderers/FontRenderer.h>

#include <GLM/glm.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <math.h>

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
	IndexedMesh* m_pGroundTestMesh;

	Texture2D* m_pBoatTexture;

	Material* m_pBoatMaterial;
	Material* m_pGroundMaterial;

	IndexedMesh* m_pWaterMesh;
	GameObject* m_pWaterGameObject;
	UniformBuffer* m_pWaterUniform;

	TextureParams m_WaterTextureParams;
	Framebuffer* m_pReflectionFBO;
	Framebuffer* m_pRefractionFBO;

	Texture* m_pWaterNormalMap;
	Texture* m_pDUDVTexture;
	float m_DistortionMoveFactor;

	FontRenderer* m_pFontRenderer;
};