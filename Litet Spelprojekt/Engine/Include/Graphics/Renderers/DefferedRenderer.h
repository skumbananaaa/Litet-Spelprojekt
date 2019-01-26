#pragma once
#include "IRenderer.h"
#include <Graphics/Textures/Framebuffer.h>
#include <Graphics/Shaders/ShaderProgram.h>
#include <Graphics/Buffers/UniformBuffer.h>
#include <Graphics/Geometry/FullscreenTri.h>

#define NUM_DIRECTIONAL_LIGHTS 1
#define NUM_POINT_LIGHTS 8

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

struct DecalPassVSPerFrame
{
	glm::mat4 ViewProj;
};

struct DecalPassVSPerObject
{
	glm::mat4 Model;
};


struct DecalPassFSPerFrame
{
	glm::mat4 InverseView;
	glm::mat4 InverseProjection;
};

struct DecalPassFSPerObject
{
	glm::mat4 InverseModel;
};

//Uniformbuffers requires a 16 multiple so we pad 
//the struct in case we want to add more lights
__declspec(align(16)) struct DirectionalLightBuffer
{
	glm::vec4 Color = glm::vec4(0.0f);
	glm::vec3 Direction = glm::vec3(0.0f);
};

__declspec(align(16)) struct PointLightBuffer
{
	glm::vec4 Color = glm::vec4(0.0f);
	glm::vec3 Position = glm::vec3(0.0f);
};

struct LightPassBuffer
{
	glm::mat4 InverseView;
	glm::mat4 InverseProjection;
	glm::vec3 CameraPosition;
	float pad1;
	DirectionalLightBuffer DirectionalLights[NUM_DIRECTIONAL_LIGHTS];
	PointLightBuffer PointLights[NUM_POINT_LIGHTS];
};

struct WaterPassPerFrame
{
	glm::mat4 CameraCombined;
	glm::vec3 CameraPosition;
	float DistortionMoveFactor;
};

struct WaterPassPerObjectVS
{
	glm::mat4 Model;
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

	void DrawScene(const Scene& scene, float dtS) const override final;

private:
	void Create() noexcept;
	void DepthPrePass(const Scene& scene) const noexcept;
	void DecalPass(const Scene& scene) const noexcept;
	void GeometryPass(const std::vector<GameObject*>& gameobjects, const Camera& camera, const Framebuffer* const pFramebuffer) const noexcept;
	void LightPass(const Camera& camera, const Scene& scene, const Framebuffer* const pFramebuffer, const Framebuffer* const pGBuffer) const noexcept;
	void WaterPass(const Scene& sceen, float dtS) const noexcept;

private:
	Framebuffer* m_pGBuffer;
	Framebuffer* m_pFinalFramebuffer;
	Framebuffer* m_pWaterGBuffer;
	Framebuffer* m_pReflection;
	
	FullscreenTri* m_pTriangle;
	
	UniformBuffer* m_pGPassVSPerFrame;
	UniformBuffer* m_pGPassVSPerObject;
	UniformBuffer* m_pGPassFSPerObject;
	UniformBuffer* m_pLightPassBuffer;
	
	UniformBuffer* m_pDecalVSPerFrame;
	UniformBuffer* m_pDecalVSPerObject;
	UniformBuffer* m_pDecalFSPerFrame;
	UniformBuffer* m_pDecalFSPerObject;

	UniformBuffer* m_pWaterPassPerFrame;
	UniformBuffer* m_pWaterPassPerObject;
	
	IndexedMesh* m_pDecalMesh;
	
	Texture2D* m_pWaterNormalMap;
	Texture2D* m_pWaterDistortionMap;
	
	ShaderProgram* m_pDepthPrePassProgram;
	ShaderProgram* m_pGeometryPassProgram;
	ShaderProgram* m_pDecalsPassProgram;
	ShaderProgram* m_pLightPassProgram;
	ShaderProgram* m_pWaterpassProgram;
};