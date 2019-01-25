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

	void OnKeyUp(KEY keycode) override;
	void OnKeyDown(KEY keycode) override;
	void OnMouseMove(const glm::vec2& position) override;
	void OnUpdate(float dtS) override;
	void OnRender(float dtS) override;

private:
	FontRenderer* m_pFontRenderer;
	IRenderer* m_pRenderer;
	Scene* m_pScene;

	IndexedMesh* m_pTestMesh;
	IndexedMesh* m_pWaterMesh;
	IndexedMesh* m_pGroundTestMesh;

	Texture2D* m_pBoatTexture;
	Texture2D* m_pBoatNormalMap;

	Material* m_pBoatMaterial;
	Material* m_pGroundMaterial;
	
	bool cartesianCamera;
};