#pragma once
#include "IRenderer.h"
#include <Graphics/Textures/Framebuffer.h>
#include <Graphics/Shaders/ShaderProgram.h>
#include <Graphics/Buffers/UniformBuffer.h>
#include <Graphics/Geometry/FullscreenTri.h>

struct GPassPerFrame
{
	glm::mat4 ViewProjection;
	glm::vec3 CameraPosition;
};

struct GPassPerObject
{
	glm::mat4 Model;
};

struct LightPassBuffer
{
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
	void Create();
	void GeometryPass(const Scene& scene) const;
	void LightPass(const Scene& scene) const;

private:
	Framebuffer* m_pGBuffer;
	FullscreenTri* m_pTriangle;
	UniformBuffer* m_pGPassVSPerFrame;
	UniformBuffer* m_pGPassVSPerObject;
	UniformBuffer* m_pLightPassBuffer;
	ShaderProgram* m_pGeometryPassProgram;
	ShaderProgram* m_pLightPassProgram;
};