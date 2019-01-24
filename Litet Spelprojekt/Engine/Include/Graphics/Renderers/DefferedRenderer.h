#pragma once
#include "IRenderer.h"
#include <Graphics/Textures/Framebuffer.h>
#include <Graphics/Shaders/ShaderProgram.h>
#include <Graphics/Buffers/UniformBuffer.h>
#include <Graphics/Geometry/FullscreenTri.h>

struct GPassVSPerFrame
{
	glm::mat4 ViewProjection;
	glm::vec3 CameraPosition;
};

struct GPassVSPerObject
{
	glm::mat4 Model;
};


struct GPassFSPerObject
{
	glm::vec4 Color;
	float HasTexture;
	float HasNormalMap;
};

struct LightPassBuffer
{
	glm::mat4 InverseView;
	glm::mat4 InverseProjection;
	glm::vec3 CameraPosition;
};

class API DefferedRenderer final : public IRenderer
{
public:
	DefferedRenderer(DefferedRenderer&& other) = delete;
	DefferedRenderer(const DefferedRenderer& other) = delete;
	DefferedRenderer& operator=(DefferedRenderer&& other) = delete;
	DefferedRenderer& operator=(const DefferedRenderer& other) = delete;

	DefferedRenderer();
	~DefferedRenderer();

	void DrawScene(const Scene& scene) const override final;

private:
	void Create() noexcept;
	void DepthPrePass(const Scene& scene) const noexcept;
	void GeometryPass(const std::vector<GameObject*>& gameobjects, const Camera& camera) const noexcept;
	void LightPass(const Scene& scene) const noexcept;
	void WaterPass(const Scene& sceen) const noexcept;

private:
	Framebuffer* m_pGBuffer;
	FullscreenTri* m_pTriangle;
	UniformBuffer* m_pGPassVSPerFrame;
	UniformBuffer* m_pGPassVSPerObject;
	UniformBuffer* m_pGPassFSPerObject;
	UniformBuffer* m_pLightPassBuffer;
	ShaderProgram* m_pDepthPrePassProgram;
	ShaderProgram* m_pGeometryPassProgram;
	ShaderProgram* m_pLightPassProgram;
	ShaderProgram* m_pWaterpassProgram;
};